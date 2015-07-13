# coding: utf-8
#
# Generated by xwot compiler.
#
# Type:       xwot:ContextResource
# Resource:   SwitchResource
# Path:       /lightbulb/switch
#

from xwot_app import app
from xwot.device.lightbulb import Switch
from xwot.util.klein import make_response
from xwot.util import deserialize

switch = Switch(name='Switch')

#
# GET '/lightbulb/switch'
#
@app.route('/lightbulb/switch', methods=['GET'])
def handle_lightbulb_switch_GET(request):
    return make_response(switch, request)

#
# PUT '/lightbulb/switch'
#
@app.route('/lightbulb/switch', methods=['PUT'])
def handle_lightbulb_switch_PUT(request):
    data = request.data
    content_type = request.getHeader('Content-Type')
    dic = deserialize(data, content_type)
    switch.update(dic, content_type)

    return make_response(switch, request)

