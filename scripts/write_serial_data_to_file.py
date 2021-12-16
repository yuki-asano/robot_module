#!/usr/bin/python
# -*- coding: utf-8 -*-

import os
import sys
import serial

PORT = '/dev/ttyUSB0'
#PORT = 'COM5' # for windows
BAUD = 57600
ser = serial.Serial(PORT, BAUD)

f = open('test.txt', 'w')

try:
    while ser.is_open:
        data = ser.readline()
        if isinstance(data, bytes): # for windows
            data = data.decode().replace('\n', '')
        print(data)
        f.writelines(data)
except KeyboardInterrupt:
    sys.exit

f.close()
