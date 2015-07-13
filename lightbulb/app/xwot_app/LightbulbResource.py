# coding: utf-8
#
# Generated by xwot compiler.
#
# Type:       xwot:DeviceResource
# Resource:   LightbulbResource
# Path:       /lightbulb
#

from xwot_app import app
from xwot.util.klein import make_response
from xwot.util import deserialize
from xwot.device.lightbulb import LightBulb

lightbulb = LightBulb(name='xWoT Light bulb', street_address="Bd de Perolles 90 - DEPARTEMENT D'INFORMATIQUE",
                      postal_code='1700', address_locality='Fribourg', room_address='A410')

#
# GET '/lightbulb'
#
@app.route('/lightbulb', methods=['GET'])
def handle_lightbulb_GET(request):
    return make_response(lightbulb, request)

#
# PUT '/lightbulb'
#
@app.route('/lightbulb', methods=['PUT'])
def handle_lightbulb_PUT(request):
    data = request.data
    content_type = request.getHeader('Content-Type')
    dic = deserialize(data, content_type)
    status = lightbulb.update(dic, content_type)

    return make_response(lightbulb, request=request, status=status)

