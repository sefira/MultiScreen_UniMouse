#!/usr/bin/python2
# -*- coding: utf-8 -*-
import socket


def print_indication():
    indication = """
    1.there are receivers: Berndatte\Davy\Howard\Leonard\Rajesh
    use B\D\H\L\R to represent\n
    2.there are instructions:
    a.open sb.: open SSG(SwitchScreenbyGaze) in machine
        so we can use openBDH or open
    b.close sb.: close SSG(SwitchScreenbyGaze) in machine
    c.click : to create mouse click work
    d.# TODO
    now please input your instructions
    """
    print(indication)


def main():
    print_indication()
    receiver_address = ('255.255.255.255', 24806)
    m_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    m_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    m_socket.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
    while 1:
        data = raw_input()
        if not data:
            break
        m_socket.sendto(data, receiver_address)
    m_socket.close()


main()
