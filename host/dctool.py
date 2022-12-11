#!/usr/bin/python3

import socket
import argparse

class DCLoadConnection:
    def __init__(self, ip, port=53535):
        self.ip = ip
        self.port = port
        self.sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
        self.sock.settimeout(5)

    def __send(self, data):
        return self.sock.sendto(data, (self.ip, self.port))

    def __recv(self):
        return self.sock.recv(1028)

    def get_version(self):
        self.__send(b"VERS")
        bytes = self.__recv()
        return bytes[12:-1].decode('utf-8')

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("ip_address", help="Specify the Target Dreamcast IP Address")
    parser.add_argument("--target-version", help="Get dcload Version Information", action="store_true")
    args = parser.parse_args()

    dc = DCLoadConnection(args.ip_address)

    try:

        if args.target_version:
            version = dc.get_version()
            print(version)

    except TimeoutError:
        print("Timed out trying to communicate with  " + args.ip_address)
        exit(-1)


