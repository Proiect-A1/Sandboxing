#!/usr/bin/env python3
"""
Reproduce the logless 100%-CPU epoll spin in the sandbox server.

The server registers client fds with EPOLLIN | EPOLLET and only acts when the
EPOLLIN bit is set. An fd that becomes ready with EPOLLHUP/EPOLLERR but WITHOUT
EPOLLIN falls through the `if (events_mask & EPOLLIN)` gate: the server never
reads it, never removes it, and epoll re-reports the (unmaskable) HUP/ERR on
every epoll_wait -> tight spin, no "Request received" log, no new accepts.

Wire format the server expects: [4B json_len LE][json][4B code_len LE][code]
The server's first state reads sizeof(int) = 4 bytes (the json length prefix).

This script offers several attack modes. The most reliable trigger for a
HUP/ERR-without-EPOLLIN condition is an abortive close (RST) on a socket where
the server is parked waiting for MORE header bytes than we sent, so there is no
readable payload edge pending -- only the error condition.

Usage:
    python3 spin_repro.py <host> <port> [mode] [count]

Modes:
    rst-partial   (default) connect, send a partial 4B length prefix, then RST.
    rst-empty     connect, send nothing, then RST immediately.
    rst-overclaim send a length prefix promising 100 bytes, send 0 body, RST.
    half-close    connect, send partial prefix, shutdown(SHUT_WR) -> FIN (EOF).
    flood-rst     spray `count` RST-partial connections fast (default 200).

After running, check the server from the host:

    PID=<server pid>
    for i in 1 2 3; do cat /proc/$PID/stat | awk '{print $14,$15}'; sleep 1; done
    # utime/stime jumping ~100/sec total => spinning
    cat /proc/$PID/status | grep State          # expect R (running)
    # then confirm accept is dead:
    python3 spin_repro.py <host> <port> probe
"""

import socket
import struct
import sys
import time


def _rst_socket():
    """A TCP socket whose close() sends RST instead of FIN (SO_LINGER, timeout 0)."""
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # linger on, timeout 0 => abortive close => RST
    s.setsockopt(socket.SOL_SOCKET, socket.SO_LINGER, struct.pack("ii", 1, 0))
    return s


def rst_partial(host, port):
    s = _rst_socket()
    s.connect((host, port))
    # server's first state wants 4 bytes; send only 2, so it's parked waiting
    s.send(b"\x02\x00")
    time.sleep(0.2)
    s.close()  # RST -> server fd goes to error state
    print("rst-partial: sent 2/4 prefix bytes, then RST")


def rst_empty(host, port):
    s = _rst_socket()
    s.connect((host, port))
    time.sleep(0.1)
    s.close()  # RST with no data sent at all
    print("rst-empty: connected, sent nothing, then RST")


def rst_overclaim(host, port):
    s = _rst_socket()
    s.connect((host, port))
    # full 4-byte prefix claiming 100 bytes of json, then send no body, then RST
    s.send(struct.pack("<I", 100))
    time.sleep(0.2)
    s.close()
    print("rst-overclaim: sent len=100 prefix, no body, then RST")


def half_close(host, port):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((host, port))
    s.send(b"\x02\x00")          # partial prefix
    s.shutdown(socket.SHUT_WR)   # FIN: server read() should return 0 (clean EOF)
    print("half-close: sent 2/4 prefix, then SHUT_WR (FIN). "
          "Tests the read()==0 path, not the HUP-without-IN path.")
    time.sleep(2)
    s.close()


def flood_rst(host, port, count):
    print(f"flood-rst: firing {count} RST-partial connections...")
    for i in range(count):
        try:
            s = _rst_socket()
            s.connect((host, port))
            s.send(b"\x02\x00")
            s.close()
        except Exception as e:
            print(f"  client-side failed at {i}: {e}")
            break
        if i % 25 == 0:
            print(f"  {i} sent")
        time.sleep(0.01)
    print("flood-rst done")


def probe(host, port):
    """Send ONE complete, valid tiny frame and see if the server responds.
    If the server is spinning, the connection will hang / time out: no accept,
    no read, no reply."""
    print("probe: sending a complete tiny frame, expecting the server to "
          "accept+read (watch its logs)...")
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.settimeout(5)
    try:
        s.connect((host, port))
    except Exception as e:
        print(f"  connect failed: {e}  -> listener may be dead")
        return
    # minimal frame: json_len=2, json="{}", code_len=0
    frame = struct.pack("<I", 2) + b"{}" + struct.pack("<I", 0)
    try:
        s.sendall(frame)
        print("  frame sent; if server is healthy you should see 'Request "
              "received' in its log. If it's spinning, nothing happens.")
    except Exception as e:
        print(f"  send failed: {e}")
    finally:
        s.close()


MODES = {
    "rst-partial": lambda h, p, c: rst_partial(h, p),
    "rst-empty": lambda h, p, c: rst_empty(h, p),
    "rst-overclaim": lambda h, p, c: rst_overclaim(h, p),
    "half-close": lambda h, p, c: half_close(h, p),
    "flood-rst": lambda h, p, c: flood_rst(h, p, c),
    "probe": lambda h, p, c: probe(h, p),
}


def main():
    if len(sys.argv) < 3:
        print(__doc__)
        print(f"\nmodes: {', '.join(MODES)}")
        sys.exit(1)

    host = sys.argv[1]
    port = int(sys.argv[2])
    mode = sys.argv[3] if len(sys.argv) > 3 else "rst-partial"
    count = int(sys.argv[4]) if len(sys.argv) > 4 else 200

    if mode not in MODES:
        print(f"unknown mode {mode!r}; choose from: {', '.join(MODES)}")
        sys.exit(1)

    MODES[mode](host, port, count)


if __name__ == "__main__":
    main()