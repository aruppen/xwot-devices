#encoding: utf-8
"""
@author     Alexander Rüedlinger <a.rueedlinger@gmail.com>
@date       28.06.2015

"""

from xwot.model import Device as XWOTDevice
from xwot.model import Sensor as XWOTSensor
from xwot.model import BaseModel
from xwot.model import Model
from xwot.i2c.adapter import DHTAdapter


class SensingDevice(XWOTDevice, BaseModel):

    __mutable_props__ = ['name', 'streetAddress', 'roomAddress', 'postalCode', 'addressLocality']
    __expose__ = __mutable_props__ + ['description', 'temperature', 'humidity']

    def __init__(self, name, street_address, postal_code, address_locality, room_address):
        super(SensingDevice, self).__init__()
        self._dic = {
            'name': name,
            'streetAddress': street_address,
            'postalCode': postal_code,
            'addressLocality': address_locality,
            'roomAddress': room_address
        }

        self.add_link('temperature')
        self.add_link('humidity')

    @property
    def name(self):
        return self._dic['name']

    @property
    def description(self):
        return "Hi there my name is %s. I'm a sensing device and currently present in room %s at the location: %s, %s, %s" % \
               (self.name, self.roomAddress, self.streetAddress, self.addressLocality, self.postalCode)

    @property
    def temperature(self):
        return '/sensing-device/temperature'

    @property
    def humidity(self):
        return '/sensing-device/humidity'

    @property
    def streetAddress(self):
        return self._dic['streetAddress']

    @property
    def postalCode(self):
        return self._dic['postalCode']

    @property
    def addressLocality(self):
        return self._dic['addressLocality']

    @property
    def roomAddress(self):
        return self._dic['roomAddress']


class TemperatureSensor(XWOTSensor, Model):

    __expose__ = ['name', 'unit', 'symbol', 'measures', 'measurement', 'back_link']

    def __init__(self, dht_adapter=DHTAdapter()):
        super(TemperatureSensor, self).__init__()
        self._dht_adapter = dht_adapter
        self.add_link('back_link')
        self.add_type('http://xwot.lexruee.ch/vocab/core-ext#TemperatureSensor')

    @property
    def back_link(self):
        return '/sensing-device'

    @property
    def name(self):
        return 'Temperature sensor'

    @property
    def unit(self):
        return 'Celsius'

    @property
    def symbol(self):
        return '°C'

    @property
    def measures(self):
        return 'Measures the temperature.'

    @property
    def description(self):
        return 'A temperature sensor of this sensing device.'

    @property
    def measurement(self):
        val = self._dht_adapter.temperature
        if val is not None:
            return round(val, 2)
        return val

    def handle_update(self, dic):
        pass


class HumiditySensor(XWOTSensor, Model):

    __expose__ = ['name', 'unit', 'symbol', 'measures', 'measurement', 'back_link']

    def __init__(self, dht_adapter=DHTAdapter()):
        super(HumiditySensor, self).__init__()
        self._dht_adapter = dht_adapter
        self.add_link('back_link')
        self.add_type('http://xwot.lexruee.ch/vocab/core-ext#HumiditySensor')

    @property
    def back_link(self):
        return '/sensing-device'

    @property
    def name(self):
        return 'Humidity sensor'

    @property
    def unit(self):
        return 'Percentage'

    @property
    def symbol(self):
        return '%'

    @property
    def measures(self):
        return 'Measures the humidity.'

    @property
    def description(self):
        return 'A humidity sensor of this sensing device.'

    @property
    def measurement(self):
        val = self._dht_adapter.humidity
        if val is not None:
            return round(val, 2)

        return val

    def handle_update(self, dic):
        pass