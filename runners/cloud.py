import http.server
import ssl
import os

class Handler(http.server.SimpleHTTPRequestHandler):
    def _receive_to(self, path):
        length = int(self.headers.get('Content-Length', 0))
        if length <= 0:
            self.send_response(411)
            self.end_headers()
            return

        os.makedirs(os.path.dirname(path) or '.', exist_ok=True)
        existed = os.path.exists(path)

        remaining = length
        with open(path, 'wb') as f:
            while remaining > 0:
                chunk = self.rfile.read(min(65536, remaining))
                if not chunk:
                    break
                f.write(chunk)
                remaining -= len(chunk)

        if remaining != 0:
            self.send_response(500)
            self.end_headers()
            self.wfile.write(b'FAIL')
            return

        self.send_response(200 if existed else 201)
        self.end_headers()
        self.wfile.write(b'UPDATED' if existed else b'CREATED')

    def do_PUT(self):
        self._receive_to('.' + self.path)

    def do_POST(self):
        self._receive_to('.' + self.path)

port = 443
server_address = ('0.0.0.0', port)
httpd = http.server.HTTPServer(server_address, Handler)

context = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
context.load_cert_chain(certfile="./runners/cert.pem", keyfile="./runners/key.pem")
httpd.socket = context.wrap_socket(httpd.socket, server_side=True)

print(f"Serving securely on https://localhost:{port}")
httpd.serve_forever()