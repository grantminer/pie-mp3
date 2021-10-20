import serial
import re
from Controller import MovementLogic
from seesBlack import seesBlack
# import keyboard
from take_input import live_edit

# try:
    # ser = serial.Serial("COM4", 9600)
ser = serial.Serial("/dev/cu.usbmodem14501", 9600)
    # ser = serial.Serial("/dev/ttyACM0", 9600)
# except:
#     ser = serial.Serial("COM5", 9600)

startline = re.compile(r'startArduino')
endline = re.compile(r'endArduino')

THRESHOLD = 350

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
        
        if startline.search(line):

            ser_list = line.split(",")
            line = line[1:len(line)-1]
            
            return ser_list

def write_data(left, right, left_speed, right_speed, threshold):

    # Determines how robot should move and writes to serial appropriately
    # 
    # Inputs:
    #   left: Boolean that represents whether left sensor sees black
    #   right: Boolean that represents whether right sensor sees black
    #   newThreshold: integer that represents where the cutoff from white to black should be

    left_bool, right_bool = seesBlack(left, right, threshold)

    left_speed1, right_speed1 = MovementLogic(left_bool, right_bool, left_speed, right_speed)

    if not (left_speed1 is left_speed and right_speed1 is right_speed):
        ser.write("startPython," + left_speed1 + "," + right_speed1 + ",endPython")

threshold = THRESHOLD

print("Driving...")

while True:

    ser_out = receive_data()

    # if keyboard.read_key() == " ":
    #     threshold = live_edit()

    write_data(ser_out[0], ser_out[1], ser_out[2], ser_out[3], threshold)

