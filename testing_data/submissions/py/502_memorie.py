import sys

tokens = sys.stdin.read().split()
if tokens:
    x = int(tokens[0])
    size = max(100, x * 10 * 1024 * 1024)
    v = bytearray(size)
    for i in range(0, size, 100):
        v[i] = i % 256
    print(v[size // 2])
