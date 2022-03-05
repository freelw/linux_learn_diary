# python 3
from socketserver import BaseRequestHandler, UDPServer
import time
import threading
import sys

client_address = None
client_sock = None
class TimeHandler(BaseRequestHandler):
    def handle(self):
        global client_address
        global client_sock
        print('Got connection from', self.client_address)
        # Get message and client socket
        msg, sock = self.request
        resp = time.ctime()
        client_address = self.client_address
        client_sock = sock
        sock.sendto(resp.encode('ascii'), self.client_address)

def loop(interval):
    global client_address
    global client_sock
    print ("ok interval", interval)
    while True:
        print(client_address)
        if client_address:
            print('send hb')
            client_sock.sendto('hb\n'.encode('ascii'), client_address)
        time.sleep(interval)
        # print('hb')

if __name__ == '__main__':
    print (sys.argv)
    interval = 30
    if len(sys.argv) > 1:
        interval = int(sys.argv[1])
    threading.Thread(target=loop, args=(interval,)).start()
    serv = UDPServer(('0.0.0.0', 30001), TimeHandler)
    serv.serve_forever()