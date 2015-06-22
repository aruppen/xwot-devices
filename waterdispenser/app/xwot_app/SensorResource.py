#
# Generated by xwot compiler.
#
# Type:       xwot:SensorResource
# Resource:   Sensor
# Path:       /waterdispenser/sensor
#

from flask import request
from xwot_app import app

from xwot.util.flask import make_response
from xwot.util import deserialize
from xwot.device.waterdispenser import Sensor

sensor = Sensor()

#
# GET '/waterdispenser/sensor'
#
@app.route('/waterdispenser/sensor', methods=['GET'])
def handle_waterdispenser_sensor_GET():
    return make_response(sensor)

