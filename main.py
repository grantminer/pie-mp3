import os
import serial
import re
import time
import math
import numpy as np
from Controller import MovementLogic
from seesBlack import seesBlack

try:
    ser = serial.Serial("COM4", 9600)
except:
    ser = serial.Serial("COM5", 9600)

endline = re.compile(r'endL')
endkey = re.compile(r'DONE')

THRESHOLD = 512

def receive_data():
    # Receive data over serial from arduino.
    # Constantly receiving output to update wheel speed and black threshold

    # Outputs: 
    #   ser_list = list of all parameters

    line = ""

    while True:
        bytesToRead = ser.inWaiting()
        contents = ser.readline(bytesToRead).decode('utf-8')

        line += contents
        # if endkey.search(line):
        #     return "0, 0, 0", True # Fix output logic
        
        if endline.search(line):
            line = line[:len(line)-4]

            ser_list = line.split(", ")
            
            return ser_list

def write_data(left, right, left_speed, right_speed, threshold):

    # Determines how robot should move and writes to serial appropriately
    # 
    # Inputs:
    #   left: Boolean that represents whether left sensor sees black
    #   right: Boolean that represents whether right sensor sees black
    #   newThreshold: integer that represents where the cutoff from white to black should be

    left_speed, right_speed = MovementLogic(left, right, left_speed, right_speed, threshold)

    ser.write(left_speed + ", " + right_speed)



print("Driving...")

while True:
    ser_out = receive_data()

    write_data(ser_out[0], ser_out[1], ser_out[2], ser_out[3], ser_out[4])

