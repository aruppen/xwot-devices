# coding: utf-8
#
# Generated by xwot compiler.
#
# Type:       xwot:ContextResource
# Resource:   SwitchResource
# Path:       /lightbulb/switch
#

from flask import request
from xwot_app import app
from xwot.util.flask import make_response
from xwot.util import deserialize
from xwot.device.lightbulb import Switch
import time

switch = Switch(name='Switch')

#
# GET '/lightbulb/switch'
#
@app.route('/lightbulb/switch', methods=['GET'])
def handle_lightbulb_switch_GET():
    return make_response(switch)


#
# POST '/lightbulb/switch'
#
@app.route('/lightbulb/switch', methods=['POST'])
def handle_lightbulb_switch_POST():
    data = request.data
    content_type = request.headers.get('content-type')
    dic = deserialize(data, content_type)
    switch.update(dic, content_type)
    time.sleep(0.2)

    return make_response(switch)

#
# PUT '/lightbulb/switch'
#
@app.route('/lightbulb/switch', methods=['PUT'])
def handle_lightbulb_switch_PUT():
    data = request.data
    content_type = request.headers.get('content-type')
    dic = deserialize(data, content_type)
    switch.update(dic, content_type)
    time.sleep(0.2)

    return make_response(switch)

