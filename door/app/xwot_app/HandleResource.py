#
# Generated by xwot compiler.
#
# Klein xwot application.
#
# Type:       xwot:ContextResource
# Resource:   HandleResource
# Path:       /door/handle
#

from xwot.util.klein import make_response
from xwot.util import deserialize
from xwot.device.door import Handle
from xwot_app import app
from twisted.internet import task, reactor

handle = Handle(name='Door handle')

#
# GET '/door/handle'
#
@app.route('/door/handle', methods=['GET'])
def handle_door_handle_GET(request):
    return make_response(handle, request)

#
# PUT '/door/handle'
#
@app.route('/door/handle', methods=['PUT'])
def handle_door_handle_PUT(request):
    data = request.content.read()
    content_type = request.getHeader('Content-Type')
    dic = deserialize(data, content_type)
    task.deferLater(reactor, 0, handle.update, dic, content_type)
    deferred_response = task.deferLater(reactor, 0, make_response, handle, request)

    return deferred_response

