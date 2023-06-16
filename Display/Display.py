import tkinter as tk
import threading
import time
import numpy as np

def read_file(filename):
    while True:
        with open(filename, 'r') as file:
            line = file.readline()
        if (not line) or (line == '\n'):
            time.sleep(0.01)
            continue
        yield line

def get_predict_value():
    filename = 'Display.txt'
    file_reader = read_file(filename)
    a.set(3)
    while True:
        for line in file_reader:
            if line == "R\n": # Rock
                a.set(0)
            elif line == "P\n": #Paper
                a.set(2)
            elif line == "S\n": #Scicssor
                a.set(1)
            elif line == "1\n":
                a.set(3)

    
def update_image():
    if a.get() == 0:
        image_label.configure(image=R_image)
    elif a.get() == 1:
        image_label.configure(image=S_image)
    elif a.get() == 2:
        image_label.configure(image=P_image)
    elif a.get() == 3:
        image_label.configure(image=RPS_image)
    

window = tk.Tk()

a = tk.IntVar()

RPS_image = tk.PhotoImage(file='RPS.png')
R_image = tk.PhotoImage(file='R.png')
P_image = tk.PhotoImage(file='P.png')
S_image = tk.PhotoImage(file='S.png')

image_label = tk.Label(window, image=None)
image_label.pack()

ml_thread = threading.Thread(target=get_predict_value)
ml_thread.start()

a.trace('w', lambda *args: update_image())

window.protocol("WM_DELETE_WINDOW", window.quit)

window.mainloop()
