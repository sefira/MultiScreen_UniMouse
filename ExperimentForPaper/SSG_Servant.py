#!/usr/bin/python2
# -*- coding: utf-8 -*-
import socket
import os
import subprocess
import GUIForExperiment

ssg_handle = 0
m_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)


def open_ssg():
    print("open ssg")
    global ssg_handle
    ssg_path = os.path.split(os.path.realpath(__file__))[0] + "\\switchScreenByGaze.exe"
    ssg_handle = subprocess.Popen(ssg_path)


def close_ssg():
    # print("close ssg")
    if isinstance(ssg_handle, subprocess.Popen):
        ssg_handle.terminate()


def click_work(master_address):
    print("click work")
    GUIForExperiment.click_gui()
    m_socket.sendto("work finished", master_address)


def input_work(master_address):
    GUIForExperiment.input_gui()
    m_socket.sendto("work finished", master_address)
    print("input work")


# to do different work according to different instructions
def assign_work(instructions, master_address):
    my_name = socket.gethostname()
    is_myself = False

    # parse instructions
    print(instructions)
    if len(instructions) < 4:
        return
    else:
        # split into command and member
        the_space = instructions.find(' ')
        if the_space == -1:
            command = instructions[0:]
            is_myself = True
        else:
            command = instructions[0:the_space]
            member = instructions[the_space:]
            if my_name is not None:
                # print(member)
                # print(command)
                # print(my_name)
                if my_name[0].lower() in member.lower():
                    # print(my_name)
                    is_myself = True

    # if myself was mentioned , then do it
    if is_myself:
        if command == "open":
            open_ssg()
        if command == "close":
            close_ssg()
        if command == "click":
            click_work(master_address)
        if command == "input":
            input_work(master_address)


def main():
    host = ''
    port = 24806
    address = (host, port)
    m_socket.bind(address)
    while 1:
        data, master_address = m_socket.recvfrom(2048)
        if not data:
            break
        print "got data from", master_address
        assign_work(data, master_address)
    m_socket.close()

main()
