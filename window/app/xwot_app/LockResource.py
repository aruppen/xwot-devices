#
# Generated by xwot compiler.
#
# Klein xwot application.
#
# Type:       xwot:ContextResource
# Resource:   LockResource
# Path:       /window/lock
#

from xwot_app import app
from xwot.device.window import Lock
from xwot.util.klein import make_response
from xwot.util import deserialize


lock = Lock(name="Window lock")

#
# GET '/window/lock'
#
@app.route('/window/lock', methods=['GET'])
def handle_window_lock_GET(request):
    return make_response(lock, request)

#
# PUT '/window/lock'
#
@app.route('/window/lock', methods=['PUT'])
def handle_window_lock_PUT(request):
    data = request.content.read()
    content_type = request.getHeader('Content-Type')
    dic = deserialize(data, content_type)
    lock.update(dic, content_type)

    return make_response(lock, request)

