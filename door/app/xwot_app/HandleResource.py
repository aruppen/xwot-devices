#
# Generated by xwot compiler.
#
# Flask xwot application.
#
# Type:       xwot:ContextResource
# Resource:   HandleResource
# Path:       /door/handle
#

from flask import request
from xwot.util.flask import make_response
from xwot.util import deserialize
from xwot.device.door import Handle
from xwot_app import app
import time

handle = Handle(name='Door handle')

#
# GET '/door/handle'
#
@app.route('/door/handle', methods=['GET'])
def handle_door_handle_GET():
    return make_response(handle)

#
# PUT '/door/handle'
#
@app.route('/door/handle', methods=['PUT'])
def handle_door_handle_PUT():
    data = request.data
    content_type = request.headers.get('Content-Type')
    dic = deserialize(data, content_type)
    handle.update(dic, content_type)
    time.sleep(0.2)

    return make_response(handle)

