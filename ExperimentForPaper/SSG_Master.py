#!/usr/bin/python2
# -*- coding: utf-8 -*-
import socket
import random
import time


def print_indication():
    indication = """
    1.there are servant: Amy\Bernadette\Davy\Howard\Leonard\Penny\Rajesh\Tom
    use A\B\D\H\L\P\R\T to represent\n
    2.there are instructions:
    a.open sb.: open SSG(SwitchScreenbyGaze) in machine
        so we can use "open BDH" or "open" to open somebody or all
    b.close sb.: close SSG(SwitchScreenbyGaze) in machine
        as well as open sb.
    c.click : to create mouse click work
    d.input : to create input word work
    now please input your instructions
    """
    print(indication)


def random_member_list():
    member_list = {
        1: "A",  # Amy
        2: "B",  # Bernadette
        3: "D",  # Davy
        4: "H",  # Howard
        5: "L",  # Leonard
        6: "P",  # Penny
        7: "R",  # Rajesh
        8: "T",  # Tom
        
        9: "A",  # Amy
        10: "B",  # Bernadette
        11: "D",  # Davy
        12: "H",  # Howard
        13: "L",  # Leonard
        14: "P",  # Penny
        15: "R",  # Rajesh
        16: "T",  # Tom
    }
    # member_order = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16]
    member_order = [1, 2, 3, 4, 5, 6, 7, 8]
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
