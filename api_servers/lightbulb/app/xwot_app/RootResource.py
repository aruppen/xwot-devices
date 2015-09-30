# coding: utf-8
#
# Generated by compiler compiler.
#
# Type:       xwot:Entity
# Resource:   RootResource
# Path:       /
#

import xwot_app
from xwot_app import app
from xwot.util.klein import cors


@app.route('/')
def home(request):
    request.setHeader('Content-Type', 'application/ld+json')
    cors(request, methods=['GET', 'OPTIONS'])
    return xwot_app.jsonld_description_str

#
# OPTIONS '/'
#
@app.route('/', methods=['OPTIONS'])
def home_OPTIONS(request):
    cors(request, methods=['GET', 'OPTIONS'])
    request.setHeader('Allow', 'GET, OPTIONS')