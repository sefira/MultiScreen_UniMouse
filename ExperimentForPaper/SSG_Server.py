#!/usr/bin/python2
import socket
import getpass
import subprocess

ssg_handle = 0


def open_ssg():
    # print("open ssg")
    # os.system("C:\\Users\\sefir\\Desktop\\switchScreenByGaze.exe")
    global ssg_handle
    host_user_name = getpass.getuser()
    ssg_path = "C:\\Users\\" + host_user_name + "\\Desktop\\SSG\\switchScreenByGaze.exe"
    ssg_handle = subprocess.Popen(ssg_path)


def close_ssg():
    # print("close ssg")
    if isinstance(ssg_handle, subprocess.Popen):
        ssg_handle.terminate()


def click_work():
    print("click work")


# TODO
def assign_work(instructions):
    my_name = socket.gethostname()
    is_myself = False

    # parse instructions
    # print(instructions)
    if len(instructions) < 4:
        return
    else:
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

    # assign work
    if is_myself:
        works = {
            "open": open_ssg,
            "close": close_ssg,
            "click": click_work
            # TODO
        }
        if command in works:
            works[command]()


def main():
    host = ''
    port = 24806
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


main()