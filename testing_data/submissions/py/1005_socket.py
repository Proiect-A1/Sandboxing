import socket
import sys

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(("8.8.8.8", 80))
sys.stderr.write("Should not reach here!\n")
