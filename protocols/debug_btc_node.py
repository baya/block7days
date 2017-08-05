import socket

nodes = socket.getaddrinfo("seed.bitcoin.sipa.be", None)

node = nodes[0][4][0]

print(node)

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

s.connect((node, 8333))

s.send("aaaaaa")

print(s.recv(1024))
