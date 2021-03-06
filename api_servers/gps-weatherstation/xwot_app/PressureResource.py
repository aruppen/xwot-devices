#
# Generated by xwot compiler.
#
# Klein xwot application.
#
# Type:       xwot:SensorResource
# Resource:   PressureResource
# Path:       /weatherstation/sensors/pressure
#


from xwot_app import app
from xwot.util.klein import make_response
from . import sensors_dic

pressure_sensor = sensors_dic['pressure']

#
# GET '/weatherstation/sensors/pressure'
#
@app.route('/weatherstation/sensors/pressure', methods=['GET'])
def handle_weatherstation_sensors_pressure_GET(request):
    return make_response(pressure_sensor, request)

