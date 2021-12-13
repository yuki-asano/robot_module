#!/usr/bin/python

import os
import sys
import serial

PORT = '/dev/ttyACM1'
BAUD = 9600
ser = serial.Serial(PORT, BAUD)

f = open('test.txt', 'w')

data_format =['# ', 'elapsed_time ', 'temp' '\n']
f.writelines(data_format)

try:
    while ser.is_open:
        data = ser.readline()
        f.writelines(data)
except KeyboardInterrupt:
    sys.exit

f.close()
