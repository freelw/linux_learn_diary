a=""

import socket
import time

def main ():
    udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    cnt = 0
    while True:
        udp_socket.sendto(a, ("127.0.0.1", 20001))
        print cnt
        cnt += 1 
        time.sleep(0.05)
    udp_socket.close()


if __name__ == '__main__':
    main()
