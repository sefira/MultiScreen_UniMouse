#!/usr/bin/python2
# -*- coding: utf-8 -*-
import socket
import random
import time


def print_indication():
    indication = """
    1.there are servant: Bernadette\Davy\Howard\Leonard\Rajesh
    use B\D\H\L\R to represent\n
    2.there are instructions:
    a.open sb.: open SSG(SwitchScreenbyGaze) in machine
        so we can use openBDH or open
    b.close sb.: close SSG(SwitchScreenbyGaze) in machine
    c.click : to create mouse click work
    d.input : to create input word work
    now please input your instructions
    """
    print(indication)


def random_member_list():
    member_list = {
        1: "B",  # Bernadette
        2: "D",  # Davy
        3: "H",  # Howard
        4: "L",  # Leonard
        5: "R",  # Rajesh
        6: "B",
        7: "D",
        8: "H",
        9: "L",
        10: "R",
    }
    member_order = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
    random.shuffle(member_order)
    shuffled_member_list = []
    for member_num in member_order:
        shuffled_member_list.append(member_list[member_num])

    return shuffled_member_list


def main():
    servant_address = ('<broadcast>', 24806)
    m_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    m_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    m_socket.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
    while 1:
        data = raw_input()
        if not data:
            break

        # parse the instruction
        the_space = data.find(' ')
        if the_space == -1:
            command = data[0:]
            member = ""
        else:
            command = data[0:the_space]
            member = data[the_space:]

        # send instruction
        if command == "open" or command == "close":
            m_socket.sendto(data, servant_address)
        if command == "click" or command == "input":
            shuffled_member_list = random_member_list()
            start_time = time.time()
            for member in shuffled_member_list:
                data = command + " " + member
                m_socket.sendto(data, servant_address)
                # wait servant finish work
                data, response_address = m_socket.recvfrom(2048)
                # print data
            end_time = time.time()
            print "this task cost %f second" % (end_time - start_time)

    m_socket.close()


print_indication()
main()
