#!/usr/bin/python2
# -*- coding: utf-8 -*-
import Tkinter
import random


def rand_word():
    word_list = {
        1: "this",
        2: "index",
        3: "page",
        4: "lists",
        5: "same",
        6: "change",
        7: "intend",
        8: "refer",
        9: "today",
        10: "rail",
        11: "strike",
        12: "stand",
        13: "streets",
        14: "start",
        15: "china",
        16: "gather",
        17: "about",
        18: "debate",
        19: "night",
        20: "after",
        21: "block",
        22: "losing",
        23: "home",
        24: "changes",
        25: "where",
        26: "health",
        27: "replace",
        28: "expect",
        29: "paris",
        30: "fashion",
        31: "bring",
        32: "include"
    }
    index = random.randint(1, 32)
    the_word = word_list[index]
    return the_word


window = Tkinter.Tk()
m_word = ""


##################################################
# click work part
def on_click():
    global window
    window.destroy()


def click_gui():
    global window
    try:
        window.title("Click Task")
    except:
        window = Tkinter.Tk()
        window.title("Click Task")
    window.geometry('300x180+520+320')
    white_label = Tkinter.Label(window, text=" ", font=('Arial', 40))
    white_label.pack(side=Tkinter.TOP)
    global m_button
    m_button = Tkinter.Button(window, text='     ', background="red", command=on_click)
    m_button.pack()
    window.mainloop()
m_button = Tkinter.Button()


##################################################
# input work part
def input_gui():
    global window
    try:
        window.title("Input Task")
    except:
        window = Tkinter.Tk()
        window.title("Input Task")
    window.geometry('400x180+520+320')
    m_label = Tkinter.Label(window, text="Please enter the following word:", font=('Arial', 20))
    m_label.pack(side=Tkinter.TOP)
    white_label = Tkinter.Label(window, text=" ", font=('Arial', 12))
    white_label.pack(side=Tkinter.TOP)
    word_label = Tkinter.Label(window, font=('Arial', 20))
    global m_word
    m_word = rand_word()
    word_label["text"] = m_word
    word_label["fg"] = "red"
    word_label.pack(side=Tkinter.TOP)

    ###########################################
    # validate input
    global m_entry
    m_entry = Tkinter.Entry(window)
    m_entry.pack()

    contents = Tkinter.StringVar()
    contents.set("")
    m_entry.config(textvariable=contents)

    def validate_input(event):
        # print contents.get()
        global m_word
        if contents.get() == m_word:
            global window
            window.destroy()
    m_entry.bind('<Key-Return>', validate_input)
    window.mainloop()

m_entry = Tkinter.Entry()
