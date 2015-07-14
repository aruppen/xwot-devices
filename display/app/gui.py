
import wx
from twisted.internet import wxreactor
wxreactor.install()

from twisted.internet import reactor, task
from twisted.logger import Logger
log = Logger()


import xwot_app
from xwot_app import app

import yadp
from yadp.semantic import client
from yadp import service
from yadp.device import Device

import treq
import json


def find_sensors(_client):
    triples = _client.sparql_query("""
    PREFIX xwot: <http://xwot.lexruee.ch/vocab/core#>
    PREFIX xwot-ext: <http://xwot.lexruee.ch/vocab/core-ext#>
    PREFIX schema: <http://schema.org/>

    SELECT ?device ?temperature ?humidity WHERE {
        ?device a xwot:Device.
        ?device xwot-ext:roomAddress ?room.

        ?device schema:knows ?temperature.
        ?device schema:knows ?humidity.

        ?temperature a xwot-ext:TemperatureSensor.
        ?humidity a xwot-ext:HumiditySensor.

        FILTER(?room = "A410")
    }""")

    triples = [map(str, triple) for triple in triples]
    for triple in triples:
        sensor_device_url, temperature_url, humidity_url = triple
        if xwot_app.resources['sensor_device'] is None:
            xwot_app.resources['sensor_device'] = sensor_device_url
            xwot_app.resources['temperature_sensor'] = temperature_url
            xwot_app.resources['humidity_sensor'] = humidity_url

    if len(triples) == 0:
        xwot_app.resources['sensor_device'] = None
        xwot_app.resources['temperature_sensor'] = None
        xwot_app.resources['humidity_sensor'] = None

    return len(triples) != 0


def _update_value(content, label):
    dic = json.loads(content)
    val = dic['measurement']
    label.SetLabel(str(val))


def update_gui(frame):
    temp_url = xwot_app.resources['temperature_sensor']
    hum_url = xwot_app.resources['humidity_sensor']

    if temp_url:
        deferred_temp_resp = treq.get(temp_url, headers={'Accept': 'application/ld+json'})
        deferred_temp_resp.addCallback(treq.content)
        deferred_temp_resp.addErrback(log.info)
        deferred_temp_resp.addCallback(_update_value, frame.temperature_value)
        deferred_temp_resp.addErrback(log.info)

    if hum_url:
        deferred_hum_resp = treq.get(hum_url, headers={'Accept': 'application/ld+json'})
        deferred_hum_resp.addCallback(treq.content)
        deferred_hum_resp.addErrback(log.info)
        deferred_hum_resp.addCallback(_update_value, frame.humidity_value)
        deferred_hum_resp.addErrback(log.info)

once = False


def loop(_client, _service, _device, frame):
    global once
    send_update = find_sensors(_client)
    if send_update and once is False:
        ##_service.update(_device)
        reactor.callLater(5.0, _service.update, _device)
        once = True

    update_gui(frame)


class MyFrame(wx.Frame):

    def __init__(self):
        super(MyFrame, self).__init__(None, -1, 'Display')
        panel = wx.Panel(self)
        font = wx.Font(30, wx.SWISS, wx.NORMAL, wx.NORMAL)

        self._temperature_text = wx.StaticText(panel, label="temperature: ", pos=(20, 20))
        self.temperature_value = wx.StaticText(panel, label="?", pos=(400, 20))

        self._temperature_text.SetFont(font)
        self.temperature_value.SetFont(font)

        self._humdity_text = wx.StaticText(panel, label="humidity: ", pos=(20, 100))
        self.humidity_value = wx.StaticText(panel, label="?", pos=(400, 100))

        self._humdity_text.SetFont(font)
        self.humidity_value.SetFont(font)

        self.Show()
        self.Maximize(True)



if __name__ == '__main__':
    yadp.debug()
    device = Device(urn='urn:xwot:Device', location=xwot_app.http_addr, descriptions=[xwot_app.yadp_description])
    service = service()
    service.register(device=device, passive=True)

    client = client()
    gui_app = wx.App()
    frame = MyFrame()

    _loop = task.LoopingCall(loop, client, service, device, frame)
    _loop.start(5.0)
    client.browse()


    reactor.registerWxApp(gui_app)
    app.run(host='0.0.0.0', port=xwot_app.port)