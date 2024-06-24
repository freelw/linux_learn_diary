#!/usr/bin/python
#coding=UTF-8

"""
TCP/IP Server sample
"""

import socket
import threading

bind_ip = "0.0.0.0"
bind_port = 58200

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

server.bind((bind_ip, bind_port))
            
server.listen(5)

print "[*] Listening on %s:%d" % (bind_ip, bind_port)

def handle_client(client_socket):
    while True:
        request = client_socket.recv(1024)
        print "[*] Received: %s" % request
        client_socket.send("ACK!")
    

client, addr = server.accept()
print "[*] Acepted connection from: %s:%d" % (addr[0],addr[1])
handle_client(client)
