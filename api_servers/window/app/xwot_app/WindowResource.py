#
# Generated by xwot compiler.
#
# Klein xwot application.
#
# Type:       xwot:DeviceResource
# Resource:   WindowResource
# Path:       /window
#

from xwot_app import app
from xwot.device.window import Window
from xwot.util.klein import make_response
from xwot.util import deserialize


window = Window(name='Window', street_address="Bd de Perolles 90 - DEPARTEMENT D'INFORMATIQUE",
                postal_code='1700', address_locality='Fribourg', room_address='A410')

#
# GET '/window'
#
@app.route('/window', methods=['GET'])
def handle_window_GET(request):
    return make_response(window, request)

#
# PUT '/window'
#
@app.route('/window', methods=['PUT'])
def handle_window_PUT(request):
    data = request.content.read()
    content_type = request.getHeader('Content-Type')
    dic = deserialize(data, content_type)
    window.update(dic, content_type)

    return make_response(window, request)

