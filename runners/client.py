import http.server
import ssl

# Define port and handler
port = 443
handler = http.server.SimpleHTTPRequestHandler
server_address = ('0.0.0.0', port)

# Create the server
httpd = http.server.HTTPServer(server_address, handler)

# Configure the SSL context
context = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
context.load_cert_chain(certfile="./runners/cert.pem", keyfile="./runners/key.pem")

# Wrap the server's socket with the SSL context
httpd.socket = context.wrap_socket(httpd.socket, server_side=True)

print(f"Serving securely on https://localhost:{port}")
httpd.serve_forever()