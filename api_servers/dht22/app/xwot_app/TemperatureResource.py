#
# Generated by xwot compiler.
#
# Flask xwot application.
#
# Type:       xwot:SensorResource
# Resource:   TemperatureResource
# Path:       /dht22/temperature
#

from flask import request
from xwot_app import app
from xwot.model import Sensor, Model
from xwot.util.flask import make_response
import json
from Arduino_Monitor import SerialData as DataGen

data = DataGen()

class DHT_Temperature(Sensor, Model):
    __mutable_props__ = []
    __expose__ = __mutable_props__ + ["name", "measurement", "unit", "precision", "backLink"]

    def __init__(self):
        super(DHT_Temperature, self).__init__()
        self.add_link("backLink")
        self.add_type("xwot-ext:TemperatureSensor")

    @property
    def resource_path(self):
        return "/dht22/temperature"

    @property
    def name(self):
        return "Temperature"

    @property
    def measurement(self):
        mm = data.next()
        json_data = json.loads(mm)
        return json_data["temperature"]

    @property
    def unit(self):
        return "Celsisus"

    @property
    def precision(self):
        return "2"

    @property
    def backLink(self):
        return "/dht22"

    def handle_update(self, dic):
        pass

dht_temp = DHT_Temperature()

#
# GET '/dht22/temperature'
#
@app.route('/dht22/temperature', methods=['GET'])
def handle_dht22_temperature_GET():
    return make_response(dht_temp)

