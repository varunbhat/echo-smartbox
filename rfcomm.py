#!/usr/bin/env python

import serial
from threading import Thread
from time import sleep


import sys
from select import select

ser = serial.Serial('/dev/ttyACM0', 9600,timeout=2)
#ser = serial.Serial('/dev/rfcomm0', 9600,timeout=1)




def printSentHex(dat):
    for i in range(len(dat)):
        print hex(ord(dat[i])),

def serialRead():
    s = ser.read(5000)
    return str(s)

def sendCmd(cmdDat,proceedingData=None):
#    print "mydata is", printSentHex(temp)
    ser.write(cmdDat)
    if not(proceedingData == None):
        ser.write(proceedingData)
        
#    rlist, _, _ = select([sys.stdin], [], [], timeout)
        
        


if __name__ == "__main__":
    while 1:            
        print """
Enter one of the following options:
1.echo.setRtcTime(); 
2.echo.setRtcTime(1); 
3.echo.pop(); 
4.echo.setTvChannel();
5.echo.getRtcTime(); 
6.echo.clearMemory();
7.echo.readRaw();
8.echo.begin();              
"""
        x = int(raw_input())
#        x = 4
        if x == 1:
            sendCmd('\x33\xa0',"Jan 23 201313:48:24")
        elif x == 2:
            sendCmd('\x34\xa0')
        elif x == 3:
            sendCmd('\x19\x91')
        elif x == 4:
            sendCmd('\x38\x39','\xC0\xE8')
        elif x == 5:
            sendCmd('\x45\x21')
        elif x == 6:
            sendCmd('\x51\x4a')
        elif x == 7:
            sendCmd('\xc3\xab')
        elif x == 8:
            sendCmd('\x33\xa0')
        elif x == 9:
            sendCmd('\xaa\xbb')
        print "Sent"
        
        print "Received Data:", serialRead()
#        ser.flush()
#        s = ser.read()
#        print s,