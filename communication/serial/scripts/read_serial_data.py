#!/usr/bin/env python
# -*- coding: utf-8 -*-

import serial

#PORT = '/dev/ttyUSB0' # for linux
#PORT = 'COM4' # for windows(PowerShell)
PORT = '/dev/ttyS4' # for windows(WSL)
BAUD = 57600
ser = serial.Serial(PORT, BAUD)

try:
    while ser.is_open:
        data = ser.readline()
        if isinstance(data, bytes): # for windows
            data = data.decode().replace('\n', '')
        print(data)
except KeyboardInterrupt:
    sys.exit

