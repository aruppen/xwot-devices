#
# Generated by xwot compiler.
#
# Flask xwot application.
#
# Type:       xwot:SensorResource
# Resource:   Temperature2Resource
# Path:       /weatherstation/sensors/temperature2
#

from xwot_app import app
from xwot.util.flask import make_response
from . import sensors_dic

temperature_2 = sensors_dic['temperature_2']

#
# GET '/weatherstation/sensors/temperature2'
#
@app.route('/weatherstation/sensors/temperature2', methods=['GET'])
def handle_weatherstation_sensors_temperature2_GET():
    return make_response(temperature_2)

