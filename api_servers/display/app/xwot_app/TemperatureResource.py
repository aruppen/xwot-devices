#
# Generated by xwot compiler.
#
# Klein xwot application.
#
# Type:       xwot:ServiceResource
# Resource:   TemperatureResource
# Path:       /display/temperature
#

import xwot_app
from xwot_app import app
import treq
import json
from xwot.model import Sensor as XWOTSensor
from xwot.model import BaseModel
from xwot.util.klein import cors


class TemperatureSensor(XWOTSensor, BaseModel):

    __mutable_props__ = ['name']
    __expose__ = __mutable_props__ + ['display_link', 'unit', 'measures', 'measurement', 'symbol']

    def __init__(self):
        super(TemperatureSensor, self).__init__()
        self._dic = {
            'name': None,
            'measurement': None,
            'unit': None,
            'symbol': None,
            'measures': None
        }
        self._proxy_dic = {}
        self.add_type('xwot-ext:TemperatureSensor')
        self.add_link('display_link')

    @property
    def resource_path(self):
        return '/display/temperature'

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
    def display_link(self):
        return '/display'

    def parse(self, data, accept):
        dic = json.loads(data)
        self._proxy_dic = dic
        self._dic['name'] = self._proxy_dic.get('name', None)
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

temperature_sensor = TemperatureSensor()

#
# GET '/display/temperature'
#
@app.route('/display/temperature', methods=['GET'])
def handle_display_temperature_GET(request):
    cors(request, methods=['GET'])
    if xwot_app.resources['temperature_sensor']:
        accept = request.getHeader('Accept')
        d = treq.get(xwot_app.resources['temperature_sensor'], headers={'Accept': 'application/json'})
        request.setHeader('Content-Type', accept)
        d.addCallback(treq.content)
        d.addCallback(temperature_sensor.parse, accept)
        d.addCallback(temperature_sensor.serialize)

        return d
    else:
        request.setResponseCode(404)
    return ''

