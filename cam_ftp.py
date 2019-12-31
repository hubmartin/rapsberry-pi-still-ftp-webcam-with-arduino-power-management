#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import ftplib
import time
import Adafruit_DHT

ftp_address = '192.168.1.40'
ftp_user = 'ftpuser'
ftp_password = 'password'

file_name = time.strftime("%Y%m%d-%H%M%S") + '.jpg'
file_path = '/var/tmp/' + file_name

humidity, temperature = Adafruit_DHT.read_retry(Adafruit_DHT.AM2302, '4')

subtitle = str("%.1f" % temperature) + "°C " + str("%.1f" % humidity) + "%RH"

os.system("fswebcam -d /dev/video0 -r 1920x1080 --subtitle '" + subtitle + "' " + file_path)
 
print ("Starting FTP")
session = ftplib.FTP(ftp_address, ftp_user, ftp_password)

file = open(file_path,'rb')
session.storbinary('STOR ' + file_name, file)
file.seek(0)
session.storbinary('STOR last.jpg', file)
file.close()

# remove file
print("removing file")
os.remove(file_path)

# update python script
update_file = 'update.py'
with open(update_file, 'wb') as f:
    try:
        session.retrbinary('RETR ' + update_file, f.write)
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
