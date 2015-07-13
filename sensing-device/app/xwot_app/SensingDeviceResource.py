# coding: utf-8
#
# Generated by xwot compiler.
#
# Klein xwot application.
#
# Type:       xwot:Resource
# Resource:   TemperatureSensorResource
# Path:       /sensor
#

from xwot_app import app
from xwot.util.klein import make_response
from xwot.util import deserialize
from model import SensingDevice

sensing_device = SensingDevice(name='xWoT DHT22 sensing device', street_address="Bd de Perolles 90 - DEPARTEMENT D'INFORMATIQUE",
                               postal_code='1700', address_locality='Fribourg', room_address='A410')
#
# GET '/sensor'
#
@app.route('/sensing-device', methods=['GET'])
def handle_sensor_GET(request):
    return make_response(sensing_device, request)

#
# PUT '/sensor'
#
@app.route('/sensing-device', methods=['PUT'])
def handle_sensor_PUT(request):
    data = request.content.read()
    content_type = request.getHeader('Content-Type')
    dic = deserialize(data, content_type)
    sensing_device.update(dic, content_type)

    return make_response(sensing_device, request)

