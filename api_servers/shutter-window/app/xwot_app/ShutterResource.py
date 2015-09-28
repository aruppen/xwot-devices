#
# Generated by xwot compiler.
#
# Klein xwot application.
#
# Type:       xwot:ContextResource
# Resource:   ShutterResource
# Path:       /window/shutter
#

from xwot_app import app
from xwot.device.shutter import Shutter
from xwot.util.klein import make_response
from xwot.util import deserialize
from xwot.util.klein import cors
from twisted.internet import task, reactor

shutter = Shutter('Window shutter')


#
# GET '/window/shutter'
#
@app.route('/window/shutter', methods=['GET'])
def handle_window_shutter_GET(request):
    cors(request, methods=['GET', 'PUT'])
    return make_response(shutter, request)


#
# PUT '/window/shutter'
#
@app.route('/window/shutter', methods=['PUT'])
def handle_window_shutter_PUT(request):
    cors(request, methods=['GET', 'PUT'])
    data = request.content.read()
    content_type = request.getHeader('Content-Type')
    dic = deserialize(data, content_type=content_type)

    task.deferLater(reactor, 0.2, shutter.update, dic, content_type)
    deferred_response = task.deferLater(reactor, 0, make_response, shutter, request)
    return deferred_response




