#
# Generated by xwot compiler.
#
# Klein xwot application.
#

import xwot_app
from xwot_app import app

import yadp
yadp.debug()

from yadp import service
from yadp.device import Device
from yadp.semantic import client
from twisted.internet import task

client = client()

device = Device(urn='urn:xwot:Device', location=xwot_app.http_addr, descriptions=[xwot_app.yadp_description])

service = service()
service.register(device=device, passive=True)


def find_window(client):
    triples = client.sparql_query("""
    PREFIX xwot: <http://xwot.lexruee.ch/vocab/core#>
    PREFIX xwot-ext: <http://xwot.lexruee.ch/vocab/core-ext#>
    PREFIX schema: <http://schema.org/>

    SELECT ?window ?lock ?handle WHERE {
        ?window a xwot-ext:Window.
        ?window xwot-ext:roomAddress ?room.

        ?window schema:knows ?lock.
        ?window schema:knows ?handle.

        ?handle a xwot-ext:Handle.
        ?lock a xwot-ext:Lock.

        FILTER(?room = "%s")
    }""" % xwot_app.room_address)

    triples = [map(str, triple) for triple in triples]
    for triple in triples:
        window_url, lock_url, handle_url = triple
        if xwot_app.resources['window'] is None:
            xwot_app.resources['window'] = window_url
            xwot_app.resources['window_lock'] = lock_url
            xwot_app.resources['window_handle'] = handle_url

    if len(triples) == 0:
        xwot_app.resources['window'] = None
        xwot_app.resources['window_lock'] = None
        xwot_app.resources['window_handle'] = None

    return len(triples) != 0


def find_door(client):
    triples = client.sparql_query("""
    PREFIX xwot: <http://xwot.lexruee.ch/vocab/core#>
    PREFIX xwot-ext: <http://xwot.lexruee.ch/vocab/core-ext#>
    PREFIX schema: <http://schema.org/>

    SELECT ?door ?lock ?handle WHERE {
        ?door a xwot-ext:Door.
        ?door xwot-ext:roomAddress ?room.

        ?door schema:knows ?lock.
        ?door schema:knows ?handle.

        ?handle a xwot-ext:Handle.
        ?lock a xwot-ext:Lock.

        FILTER(?room = "%s")
    }""" % xwot_app.room_address)

    triples = [map(str, triple) for triple in triples]
    for triple in triples:
        door_url, lock_url, handle_url = triple
        if xwot_app.resources['door'] is None:
            xwot_app.resources['door'] = door_url
            xwot_app.resources['door_lock'] = lock_url
            xwot_app.resources['door_handle'] = handle_url

    if len(triples) == 0:
        xwot_app.resources['door'] = None
        xwot_app.resources['door_lock'] = None
        xwot_app.resources['door_handle'] = None

    return len(triples) != 0


def find_lightbulb(client):
    triples = client.sparql_query("""
    PREFIX xwot: <http://xwot.lexruee.ch/vocab/core#>
    PREFIX xwot-ext: <http://xwot.lexruee.ch/vocab/core-ext#>
    PREFIX schema: <http://schema.org/>

    SELECT ?lightbulb ?switch ?sensor WHERE {
        ?lightbulb a xwot-ext:LightBulb.
        ?lightbulb xwot-ext:roomAddress ?room.

        ?lightbulb schema:knows ?switch.
        ?lightbulb schema:knows ?sensor.

        ?switch a xwot-ext:Switch.
        ?sensor a xwot-ext:IlluminanceSensor.

        FILTER(?room = "%s")
    }""" % xwot_app.room_address)
    triples = [map(str, triple) for triple in triples]
    for triple in triples:
        lightbulb_url, switch_url, sensor_url = triple
        if xwot_app.resources['lightbulb'] is None:
            xwot_app.resources['lightbulb'] = lightbulb_url
            xwot_app.resources['lightbulb_sensor'] = sensor_url
            xwot_app.resources['lightbulb_switch'] = switch_url

    if len(triples) == 0:
        xwot_app.resources['lightbulb'] = None
        xwot_app.resources['lightbulb_sensor'] = None
        xwot_app.resources['lightbulb_switch'] = None

    return len(triples) != 0

once = False
def loop(_client, _service, _device):
    global once
    send_update = find_door(_client) and find_lightbulb(_client) and find_window(_client)
    if send_update and once is False:
        _service.update(_device)
        once = True

_loop = task.LoopingCall(loop, client, service, device)
_loop.start(5.0)
client.browse()


if __name__ == '__main__':
    app.run(host='localhost', port=xwot_app.port)

