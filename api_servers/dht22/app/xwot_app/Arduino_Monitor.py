"""
Listen to serial, return most recent numeric values
Lots of help from here:
http://stackoverflow.com/questions/1093598/pyserial-how-to-read-last-line-sent-from-serial-device
"""

from threading import Thread
import time
import logging
import signal
import sys
import os
import subprocess
from time import sleep
import Adafruit_DHT

#from publisher import Publisher


class SerialData(object):
    def __init__(self):

        self.__temperature = -100
        self.__humidity = 0



    def next(self):
        humidity, temperature = Adafruit_DHT.read_retry(Adafruit_DHT.DHT22, 4)
        self.__humidity = round(humidity, 2)
        self.__temperature = round(temperature, 2)
        return str('{"temperature":"%5.2f","humidity":"%5.2f"}' % (self.__temperature, self.__humidity))



if __name__ == '__main__':
    s = SerialData()

    for i in range(10):
        time.sleep(1)
        print s.next()

