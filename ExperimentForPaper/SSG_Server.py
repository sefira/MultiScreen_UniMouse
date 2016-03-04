#!/usr/bin/python2
# -*- coding: utf-8 -*-
import socket
import os
import subprocess
import threading
import GUIForExperiment

ssg_handle = 0


def open_ssg():
    print("open ssg")
    # os.system("C:\\Users\\sefir\\Desktop\\switchScreenByGaze.exe")
    global ssg_handle
    ssg_path = os.path.split(os.path.realpath(__file__))[0] + "\\SSG\\switchScreenByGaze.exe"
    ssg_handle = subprocess.Popen(ssg_path)


def close_ssg():
    # print("close ssg")
    if isinstance(ssg_handle, subprocess.Popen):
        ssg_handle.terminate()


def click_work():
    GUIForExperiment.click_gui()
    print("click work")


class ThreadForInputWork(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)

    def run(self):
        GUIForExperiment.input_gui()


def input_work():
    # input_work_thread = ThreadForInputWork()
    # input_work_thread.start()
    # input_work_thread.join()
    GUIForExperiment.input_gui()
    print("input work")


# to do different work according to different instructions
def assign_work(instructions):
    my_name = socket.gethostname()
    is_myself = False

    # parse instructions
    # print(instructions)
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
                if my_name[0] in member:
                    # print(my_name)
                    is_myself = True

    # if myself was mentioned , then do it
    if is_myself:
        works = {
            "open": open_ssg,
            "close": close_ssg,
            "click": click_work,
            "input": input_work
            # TODO
        }
        if command in works:
            works[command]()


def main():
    host = ''
    port = 24807
    address = (host, port)
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.bind(address)
    while 1:
        data, sender_address = s.recvfrom(2048)
        if not data:
            break
        # print "got data from", sender_address
        assign_work(data)
    s.close()

click_work()
click_work()
input_work()
input_work()
main()
