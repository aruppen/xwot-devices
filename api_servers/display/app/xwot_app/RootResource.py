#
# Generated by compiler compiler.
#
# Klein xwot application.
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
    cors(request, methods=['GET'])
    return xwot_app.jsonld_description_str

