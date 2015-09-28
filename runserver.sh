#!/usr/bin/env sh
set -e

# Add the line below in rc.local:
# /home/pi/runserver.sh &
# or add the line below in sudo crontab -e
# @reboot sh /home/pi/runserver.sh &
# and move this script to /home/pi

APP=weatherstation
APP_PATH=/home/pi/xwot-devices/api_servers/$APP/app
cd $APP_PATH
python runserver.py
