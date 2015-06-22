#Raspberry Pi - Configuration

See the [adafruit i2c config guide](https://learn.adafruit.com/adafruits-raspberry-pi-lesson-4-gpio-setup/configuring-i2c).

## python / pip
```
sudo apt-get install python-pip python-dev
```


## I2C
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

Finally, install i2c-tools, libi2c-dev and python-smbus via:

```
sudo apt-get install python-smbus i2c-tools libi2c-dev
```
