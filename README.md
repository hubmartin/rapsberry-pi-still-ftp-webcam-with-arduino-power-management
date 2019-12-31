# Solar-powered Raspberry Pi picture camera with Arduino for power-on/off

This project contains Python script that will take a still picture from Raspberry Pi camera after boot and transfer it over network to the FTP server.

The Rpi is periodically turned on and off by Arduino and P-FET. This way the node can work from batteries.

If there is a `update.py` file in the FTP directory, the script in Rpi is automatically updated and next time is run this updated version.