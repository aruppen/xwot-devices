# xwot-devices

Collection of firmwares and API servers for xwot devices.

This repository hosts the following resources for the xWoT devices of the [Software Engineering Research Group](https://diuf.unifr.ch/drupal/softeng/):
 * [RESTful API servers](https://github.com/lexruee/xwot-devices/tree/master/api_servers)
 * [firmwares](https://github.com/lexruee/xwot-devices/tree/master/firmware)
 * [schematic files](https://github.com/lexruee/xwot-devices/tree/master/schematic-files)
 * [xwot files](https://github.com/lexruee/xwot-devices/tree/master/xwot-files)


## RESTful API Servers

Overview of the implemented RESTful API servers:
 * basic devices:
     * [light bulb](https://github.com/lexruee/xwot-devices/tree/master/api_servers/lightbulb)
     * [water dispenser](https://github.com/lexruee/xwot-devices/tree/master/api_servers/waterdispenser)
     * [weather station](https://github.com/lexruee/xwot-devices/tree/master/api_servers/weatherstation)
     * [door](https://github.com/lexruee/xwot-devices/tree/master/api_servers/door)
     * [window](https://github.com/lexruee/xwot-devices/tree/master/api_servers/window)
     * [sensing-device (uses dht22 sensor)](https://github.com/lexruee/xwot-devices/tree/master/api_servers/sensing-device)
 * composite devices:
     * [room](https://github.com/lexruee/xwot-devices/tree/master/api_servers/room)
     * [display](https://github.com/lexruee/xwot-devices/tree/master/api_servers/display)

All APIs support the following content types:
 * application/ld+json
 * application/json
 * application/xml
 * text/html (TODO)
 * text/plain (TODO)

## Hardware
### Arduino firmware

The firmware for the xwot devices / arduino microcontrollers are available in the directory [firmware](https://github.com/lexruee/xwot-devices/tree/master/firmware).

### Schematic files

The schematic files can be found in the directory [schematic-files](https://github.com/lexruee/xwot-devices/tree/master/schematic-files).

## Basic configuration

### Python / pip / build tools
```
sudo apt-get install python-pip python-dev build-essential libffi-dev
```


### Python dependencies

#### twisted / klein / treq
```
sudo pip install twisted klein treq
```

#### yadp
```
git clone https://github.com/lexruee/yadp
```

```
sudo python setup.py install
```

#### xwot.py
```
git clone https://github.com/lexruee/xwot.py
```

```
sudo python setup.py install
```

### I2C

Some useful resources about the I2C bus:
* [Wikipedia I2C](https://en.wikipedia.org/wiki/I%C2%B2C)
* [Linux kernel smbus-protocol](https://www.kernel.org/doc/Documentation/i2c/smbus-protocol)
* [I2C python documentation](http://wiki.erazor-zone.de/wiki:linux:python:smbus:doc)
* [Using the I2C Interface](http://www.raspberry-projects.com/pi/programming-in-python/i2c-programming-in-python/using-the-i2c-interface-2)
* [Connecting an Arduino to a Raspberry Pi using I2C](http://blog.retep.org/2014/02/15/connecting-an-arduino-to-a-raspberry-pi-using-i2c/)

Install i2c-tools, libi2c-dev and python-smbus via:

```
sudo apt-get install python-smbus i2c-tools libi2c-dev
```

To use i2c without root right please add the corresponding user to the group i2c.

Example: add user pi to i2c group
```
sudo usermod -a -G i2c pi
```

Reboot:
```
sudo reboot
```

## Raspberry pi configuration

See the [adafruit i2c config guide](https://learn.adafruit.com/adafruits-raspberry-pi-lesson-4-gpio-setup/configuring-i2c).

### I2C
Edit:
```
sudo nano /etc/modules
```

and these two lines:

```
i2c-bcm2708
i2c-dev
```

Edit:
```
/boot/config.txt
```

and add these two lines:

```
dtparam=i2c1=on
dtparam=i2c_arm=on
```

## Running xWoT applications


```
git clone https://github.com/lexruee/xwot-devices
```

```
cd xwot-devices/{xwot-device}/app
```

Before running `runserver.py` please install all necessary dependencies:

```
sudo pip install -r requirements.txt
```

Finally, run it via:
```
python runserver.py
```

### twistd deployment
Running the server as daemon:
```
twistd -y runserver.tac
```

Stopping the server:
```
sudo kill `cat twistd.pid`
```



## Handy tools

Because the xwot applications are RESTful they are browsable through so called REST clients:

* [Advanced REST client / chrome](https://chrome.google.com/webstore/detail/advanced-rest-client/hgmloofddffdnphfgcellkdfbfbjeloo)



## License & Copyright

```
xwot-devices  - Collection of firmwares and API servers for xwot devices
Copyright (C) 2015  Alexander Rüedlinger

xwot-devices is licensed under the GPL 3.0.

```