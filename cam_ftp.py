#!/usr/bin/env python
# -*- coding: utf-8 -*-
#xxx
import os
import ftplib
import time
import Adafruit_DHT
import serial
import StringIO

ftp_address = '192.168.1.110'
ftp_user = 'ftp_user'
ftp_password = 'password'
ftp_path = '/ftp/'

str_timestamp = time.strftime("%Y-%m-%d_%H-%M-%S")
file_name = str_timestamp + '.jpg'
file_path = '/tmp/' + file_name

humidity, temperature = Adafruit_DHT.read_retry(Adafruit_DHT.AM2302, '4')

voltage = "0.0"
voltage_unloaded = "0.0"

#try:
with serial.Serial('/dev/ttyS0', 115200, timeout=3.0) as ser:
    ser.reset_input_buffer()
    # Must-have: Dummy read to clean input buffer
    ser.readline()
    line = ser.readline().strip().split(",")
    if len(line) == 2:
        voltage = line[0]
        voltage_unloaded = line[1]
    print("voltage: "+ voltage + " voltage_unloaded: " + voltage_unloaded)
#except:
#    print("serial error")

subtitle = str("%.1f" % temperature) + "°C " + str("%.1f" % humidity) + "%RH Voltage: " + str(voltage) + " Voltage unloaded: " + str(voltage_unloaded)

os.system("fswebcam -d /dev/video0 -r 1920x1080 --subtitle '" + subtitle + "' " + file_path)
 
print ("Starting FTP")
session = ftplib.FTP(ftp_address, ftp_user, ftp_password)

# FTP store images
file = open(file_path,'rb')
session.storbinary('STOR ' + ftp_path + file_name, file)
file.seek(0)
session.storbinary('STOR ' + ftp_path + 'last.jpg', file)
file.close()

# FTP append CSV file
csv_line = str_timestamp + ", " + str("%.1f" % temperature) + ", " + str("%.1f" % humidity) + ", " + voltage + ", " + voltage_unloaded + "\n"
session.storlines('APPE ' + ftp_path + 'log.csv', StringIO.StringIO(csv_line))

# remove file
print("removing file")
os.remove(file_path)

# update python script
update_file = 'update.py'
with open(update_file, 'wb') as f:
    try:
        session.retrbinary('RETR ' + ftp_path + update_file, f.write)
        os.rename(update_file, __file__)
        print("Script updated")
    except:
        print("No update.py file found")
try:
    os.remove(update_file)
except:
    print("")

session.quit()
print ("Finished FTP")
