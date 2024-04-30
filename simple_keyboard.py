import cv2
import serial
import time
import keyboard
import SerialModule as sm


ser = sm.initConnection("COM5", 9600)
while True:

    if keyboard.is_pressed('w'): sm.sendData(ser,[90],3)
    elif keyboard.is_pressed('s'): sm.sendData(ser,[0],3)
    elif keyboard.is_pressed('a'): sm.sendData(ser,[115],3)
    elif keyboard.is_pressed('d'): sm.sendData(ser,[75],3)
    elif keyboard.is_pressed('q'): break
