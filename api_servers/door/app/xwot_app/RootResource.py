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

@app.route('/')
def home(request):
    cors(request, methods=['GET'])
    request.setHeader('Content-Type', 'application/ld+json')
    return xwot_app.jsonld_description_str


#
# OPTIONS '/'
#
@app.route('/', methods=['OPTIONS'])
def home_OPTIONS(request):
    cors(request, methods=['GET'])
    request.setHeader('Allow', 'GET')