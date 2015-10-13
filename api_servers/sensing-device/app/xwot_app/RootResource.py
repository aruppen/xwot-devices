# coding: utf-8
#
# Generated by xwot compiler.
#
# Klein xwot application.
#
# Type:       xwot:Entity
# Resource:   Entity
# Path:       /
#

import xwot_app
from xwot_app import app
from xwot.util.klein import cors
from twisted.web.static import File

@app.route('/')
def home(request):
    request.setHeader('Content-Type', 'application/ld+json')
    cors(request, methods=['GET'])
    return xwot_app.jsonld_description_str

@app.route('/static/', branch=True)
def static_files(request):
    return File("./static")