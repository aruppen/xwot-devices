import xwot_device
from xwot.util.vocab import Xsd
from xwot.util.vocab import SchemaOrg
from xwot.util.vocab import Hydra
from xwot.model.examples import LightBulb as Model

annotator = xwot_device.annotator

MODEL = Model()

class LightBulb(object):

    __expose__ = ['name', 'state', 'sensor']

    def __init__(self):
        self._model = MODEL
        self._name = "xWoT Lightbulb"

    @property
    def name(self):
        return self._name

    @property
    def state(self):
        return self._model.state

    @property
    def sensor(self):
        return '/lightbulb/sensor'

    def update(self, dic):
        lightbulb_dic = dic
        if dic.get(self.__class__.__name__, False):
            lightbulb_dic = dic.get(self.__class__.__name__, False)

        if lightbulb_dic:
            if lightbulb_dic.get('state', None) == 'on':
                self._model.switch_on()

            if lightbulb_dic.get('state', None) == 'off':
                self._model.switch_off()

            if lightbulb_dic.get('name', False):
                self._name = lightbulb_dic.get('name')


klass_lightbulb = annotator.klass(LightBulb)
#klass_lightbulb.add_context('http://xwot.lexruee.ch/contexts/xwot.jsonld')
#klass_lightbulb.add_context('http://xwot.lexruee.ch/contexts/xwot-ext.jsonld')
klass_lightbulb.describe_class(title='LightBulb', description='A lightbulb which can be turned on or off.',
                               operations=['lightbulb_get', 'lightbulb_post', 'lightbulb_put'])
klass_lightbulb.describe_property('name', description='The name of this lightbulb.', range=Xsd.string(),
                                  iri=SchemaOrg.name())
klass_lightbulb.describe_property('state', description='The state of this lightbulb which be either on or off.',
                                  range=Xsd.string(), required=True)
klass_lightbulb.describe_property('sensor', description='A light sensor which is attached to this Lightbulb.',
                                  type=Hydra.Link(), operations=['sensor_get'], readonly=True, writeonly=False)


class IlluminanceSensor(object):

    __expose__ = ['measurement', 'unit', 'measures']

    def __init__(self):
        self._model = MODEL

    @property
    def measurement(self):
        return self._model.illuminance

    @property
    def unit(self):
        return 'lx'

    @property
    def measures(self):
        return 'Illuminance'

klass_sensor = annotator.klass(IlluminanceSensor)
klass_sensor.describe_class(title='IlluminanceSensor', operations=['sensor_get'],
                            description='A sensor which measures the illuminance of this lightbulb.')
klass_sensor.describe_property('measurement', description='The measurement of this illuminance sensor',
                               range=Xsd.string())

klass_sensor.describe_property('unit', description='The unit of the measurement.',
                               range=Xsd.string())

klass_sensor.describe_property('measures', description='What kind of value this sensor measures.',
                               range=Xsd.string())

annotator.documentation(entrypoint=xwot_device.entry_point, title='My Api Doc', description='Api documentation for a xWoT Lightbulb.',
                        vocab_url=xwot_device.vocab_url)