#
# Generated by xwot compiler.
#
# Flask xwot application.
#
# Type:       xwot:SensorResource
# Resource:   HumidityResource
# Path:       /weatherstation/sensors/humidity
#

from xwot_app import app
from xwot.util.flask import make_response
from . import sensors

humidity_sensor = sensors['humidity']

#
# GET '/weatherstation/sensors/humidity'
#
@app.route('/weatherstation/sensors/humidity', methods=['GET'])
def handle_weatherstation_sensors_humidity_GET():
    return make_response(humidity_sensor)

