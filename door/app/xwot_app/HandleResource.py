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
from xwot_app import app


#
# GET '/door/handle'
#
@app.route('/door/handle', methods=['GET'])
def handle_door_handle_GET():
    return "Name: HandleResource , Hello at: /door/handle"

#
# POST '/door/handle'
#
@app.route('/door/handle', methods=['POST'])
def handle_door_handle_POST():
    return "Name: HandleResource , Hello at: /door/handle"

#
# PUT '/door/handle'
#
@app.route('/door/handle', methods=['PUT'])
def handle_door_handle_PUT():
    return "Name: HandleResource , Hello at: /door/handle"

