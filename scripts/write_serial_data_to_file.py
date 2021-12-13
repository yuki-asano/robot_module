#!/usr/bin/python

import os
import sys
import serial

PORT = '/dev/ttyUSB0'
BAUD = 57600
ser = serial.Serial(PORT, BAUD)

f = open('test.txt', 'w')

try:
    while ser.is_open:
        data = ser.readline()
        f.writelines(data)
except KeyboardInterrupt:
    sys.exit

f.close()
