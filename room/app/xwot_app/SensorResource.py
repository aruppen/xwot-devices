#
# Generated by xwot compiler.
#
# Klein xwot application.
#
# Type:       xwot:ServiceResource
# Resource:   SensorResource
# Path:       /room/lightbulb/sensor
#

import xwot_app
from xwot_app import app
import treq
import json
from xwot.model import Service as XWOTService
from xwot.model import BaseModel
from xwot.util import deserialize


class Sensor(XWOTService, BaseModel):

    __mutable_props__ = ['name']
    __expose__ = __mutable_props__ + ['lightbulb_link', 'room_link', 'description', 'unit', 'measures', 'measurement', 'symbol']

    def __init__(self):
        super(Sensor, self).__init__()
        self._dic = {
            'name': None,
            'description': None,
            'measurement': None,
            'unit': None,
            'symbol': None,
            'measures': None
        }
        self._proxy_dic = {}
        self.add_type('xwot-ext:IlluminanceSensor')
        self.add_link('room_link')
        self.add_link('lightbulb_link')

    @property
    def resource_path(self):
        return '/room/lightbulb/sensor'

    @property
    def measurement(self):
        return self._dic['measurement']

    @property
    def unit(self):
        return self._dic['unit']

    @property
    def symbol(self):
        return self._dic['symbol']

    @property
    def measures(self):
        return self._dic['measures']

    @property
    def name(self):
        return self._dic['name']

    @property
    def description(self):
        return self._dic['description']

    @property
    def room_link(self):
        return '/room'

    @property
    def lightbulb_link(self):
        return '/room/lightbulb'

    @property
    def state(self):
        return self._dic['state']

    def parse(self, data, accept):
        dic = json.loads(data)
        self._proxy_dic = dic
        self._dic['name'] = self._proxy_dic.get('name', None)
        self._dic['description'] = self._proxy_dic.get('description', None)
        self._dic['measurement'] = self._proxy_dic.get('measurement', None)
        self._dic['unit'] = self._proxy_dic.get('unit', None)
        self._dic['measures'] = self._proxy_dic.get('measures', None)
        self._dic['symbol'] = self._proxy_dic.get('symbol', None)
        return accept

    def serialize(self, content_type):
        if content_type == 'application/json':
            return self.to_json()

        if content_type == 'application/ld+json':
            return self.to_jsonld()

        if content_type == 'application/xml':
            return self.to_xml()

        return None

sensor = Sensor()

#
# GET '/room/lightbulb/sensor'
#
@app.route('/room/lightbulb/sensor', methods=['GET'])
def handle_room_lightbulb_sensor_GET(request):
    if xwot_app.resources['lightbulb_sensor']:
        accept = request.getHeader('Accept')
        d = treq.get(xwot_app.resources['lightbulb_sensor'], headers={'Accept': 'application/json'})
        request.setHeader('Content-Type', accept)
        d.addCallback(treq.content)
        d.addCallback(sensor.parse, accept)
        d.addCallback(sensor.serialize)

        return d
    else:
        request.setResponseCode(404)
    return ''
