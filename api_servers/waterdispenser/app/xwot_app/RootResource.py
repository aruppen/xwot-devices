#
# Generated by xwot compiler.
#
# Type:       xwot:Entity
# Resource:   Root
# Path:       /
#

import xwot_app
from xwot_app import app


@app.route('/')
def home(request):
    request.setHeader('Content-Type', 'application/ld+json')
    return xwot_app.jsonld_description_str