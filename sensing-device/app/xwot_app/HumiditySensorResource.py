# coding: utf-8
#
# Generated by xwot compiler.
#
# Flask xwot application.
#
# Type:       xwot:SensorResource
# Resource:   HumiditySensorResource
# Path:       /sensing-device/humidity
#

from flask import request
from xwot_app import app
from xwot.util.flask import make_response
from model import HumiditySensor

humidity_sensor = HumiditySensor()


#
# GET '/sensor/temperature'
#
@app.route('/sensing-device/humidity', methods=['GET'])
def handle_sensor_humidity_GET():
    return make_response(humidity_sensor)
