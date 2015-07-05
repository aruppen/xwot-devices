# xWoT devices - RESTful API servers

This repository hosts the final RESTful API servers for the developed xWoT devices.


## RESTful APIs

Overview of the implemented RESTful APIs:
 * [light bulb](https://github.com/lexruee/xwot-devices/tree/master/lightbulb)
 * [water dispenser](https://github.com/lexruee/xwot-devices/tree/master/waterdispenser)
 * [weather station](https://github.com/lexruee/xwot-devices/tree/master/weatherstation)
 * [door](https://github.com/lexruee/xwot-devices/tree/master/door)
 * [window](https://github.com/lexruee/xwot-devices/tree/master/window)
 * [sensing-device (uses dht22 sensor)](https://github.com/lexruee/xwot-devices/tree/master/sensing-device)

All APIs support the following content types:
 * application/ld+json
 * application/json
 * application/xml
 * text/html (TODO)
 * text/plain (TODO)

## Basic configuration

### Python / pip / build tools
```
sudo apt-get install python-pip python-dev build-essential libffi-dev
```


### Python dependencies
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
