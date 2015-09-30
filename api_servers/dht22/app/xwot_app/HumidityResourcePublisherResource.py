#
# Generated by xwot compiler.
#
# Flask xwot application.
#
# Type:       xwot:PublisherResource
# Resource:   HumidityResourcePublisherResource
# Path:       /dht22/humidity/pub
#

from xwot_app import app
from xwot.util.klein import make_response
from xwot.util.klein import cors


#
# GET '/dht22/humidity/pub'
#
@app.route('/dht22/humidity/pub', methods=['GET'])
def handle_dht22_humidity_pub_GET(request):
    cors(request, methods=['GET', 'POST'])
    return "Name: HumidityResourcePublisherResource , Hello at: /dht22/humidity/pub"

#
# POST '/dht22/humidity/pub'
#
@app.route('/dht22/humidity/pub', methods=['POST'])
def handle_dht22_humidity_pub_POST(request):
    cors(request, methods=['GET', 'POST'])
    return "Name: HumidityResourcePublisherResource , Hello at: /dht22/humidity/pub"
