#!/usr/bin/env python2.7
# Interfaces with the Arduino
# by Eric Merrill

import serial
import re
import subprocess
import sys
import os


ser = serial.Serial('/dev/tty.usbmodem1421', 57600)

while True:
    line = ser.readline();

    m = re.search('([0-9])', line)
    if not m:
        print 'not';
        continue

    vote = int(m.group(1))
    print vote;

    #Take Picture
    try:
        file = os.path.join(os.getcwd(), 'TakePic.py')
        subprocess.call('python ' + file + ' ' + `vote`, shell=True)
    except OSError as e:
        print >>sys.stderr, "Execution failed:", e
    