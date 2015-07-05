#
# Generated by xwot compiler.
#
# Flask xwot application.
#
# Type:       xwot:ContextResource
# Resource:   HandleResource
# Path:       /window/handle
#

from flask import request
from xwot_app import app
from xwot.device.window import Handle
from xwot.util.flask import make_response
from xwot.util import  deserialize

handle = Handle(name="Window handle")

#
# GET '/window/handle'
#
@app.route('/window/handle', methods=['GET'])
def handle_window_handle_GET():
    return make_response(handle)

#
# PUT '/window/handle'
#
@app.route('/window/handle', methods=['PUT'])
def handle_window_handle_PUT():
    data = request.data
    content_type = request.headers.get('Content-Type')
    dic = deserialize(data, content_type)
    handle.update(dic, content_type)

    return make_response(handle)

