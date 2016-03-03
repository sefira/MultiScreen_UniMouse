import Tkinter
import random


window = Tkinter.Tk()


def on_click():
    window.destroy()


def click_gui():
    window.title("Click Task")
    window.geometry('300x180')
    white_label = Tkinter.Label(window, text = " ", font = ('Arial', 40))
    white_label.pack(side = Tkinter.TOP)
    m_button = Tkinter.Button(window, text = '     ', command = on_click)
    m_button["background"] = "red"
    m_button.pack()
    window.mainloop()


def rand_word():
    word_list = {
        1:"this",
        2:"index",
        3:"page",
        4:"lists",
        5:"same",
        6:"change",
        7:"intend",
        8:"refer",
        9:"today",
        10:"rail",
        11:"strike",
        12:"stand",
        13:"streets",
        14:"start",
        15:"china",
        16:"gather",
        17:"about",
        18:"debate",
        19:"night",
        20:"after",
        21:"block",
        22:"losing",
        23:"home",
        24:"changes",
        25:"where",
        26:"health",
        27:"replace",
        28:"expect",
        29:"paris",
        30:"fashion",
        31:"bring",
        32:"include"
    }
    index = random.randint(1, 32)
    the_word = word_list[index]
    return the_word


def input_gui():
    window.title("Input Task")
    window.geometry('400x180')
    m_label = Tkinter.Label(window, text = "Please enter the following word:", font = ('Arial', 20))
    m_label.pack(side = Tkinter.TOP)
    white_label = Tkinter.Label(window, text = " ", font = ('Arial', 12))
    white_label.pack(side = Tkinter.TOP)
    m_word = Tkinter.Label(window, font = ('Arial', 20))
    m_word["text"] = rand_word()
    m_word["fg"] = "red"
    m_word.pack(side = Tkinter.TOP)
    m_entry = Tkinter.Entry(window, bd = 5)
    m_entry.pack(side = Tkinter.TOP)
    window.mainloop()
