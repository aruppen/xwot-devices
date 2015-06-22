# xWoT devices - RESTful applications

This repository hosts the final RESTful applications for the developed xWoT devices.

Overview of the implemented RESTful applications:
 * light bulb
 * water dispenser
 * weather station
 * door


## Basic configuration

### Python / pip / build tools
```
sudo apt-get install python-pip python-dev build-essential
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

## Raspberry pi configuration

See the [adafruit i2c config guide](https://learn.adafruit.com/adafruits-raspberry-pi-lesson-4-gpio-setup/configuring-i2c).

### I2C
Edit:
```
sudo nano /etc/modules
```

and these two lines:

```
i2c-bcm2708&nbsp;
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
