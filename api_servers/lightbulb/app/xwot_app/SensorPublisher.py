#
# Generated by xwot compiler.
#
# Klein xwot application.
#
# Type:       xwot:PublisherResource
# Resource:   openClosePublisher
# Path:       /windows/windows/pub
#

from xwot_app import app
from xwot_app import app
from xwot.util.klein import make_response
from xwot.util import deserialize
from xwot.util.klein import cors
from xwot.device.lightbulb import Sensor




import json
import logging
from templates.tpl_client import ClientElement
from templates.tpl_clients import ClientsElement
from twisted.web.template import flattenString
from autobahn.twisted.resource import WebSocketResource, HTTPChannelHixie76Aware
from xwot.util.WebSocketSupport import xWoTStreamerProtocol
from xwot.util.WebSocketSupport import xWoTBroadcastFactory
from  xwot.util.SubscriberDB import SubscriberDB
from xwot.util.SampleHardware import SampleHardware

#
# GET '/windows/windows/pub'
#
@app.route('/lightbulb/sensor/pub', methods=['GET'])
def handle_windows_windows_pub_GET2(request):
    cors(request, methods=['GET', 'PUT', 'OPTIONS'])
    request.setHeader('Allow', 'GET, PUT, OPTIONS')
    dbclient = SubscriberDB.getAllClients()
    logging.debug(request.requestHeaders)
    accept_type = request.requestHeaders.getRawHeaders("Accept")[0]
    clients = ""
    if not None:
        if accept_type == "application/json":
            request.setHeader("Content-Type", "application/json; charset=UTF-8")
            request.setResponseCode(200)
            for cl in dbclient:
                clients += str('{"id":"%s", "uri":"%s", "method":"%s", "accept":"%s"}, ' % (cl[0], cl[1], cl[2], cl[3]))
            return str('{"clients": {"client":[%s]}}' % (clients[:-2]))
        elif accept_type == "application/xml":
            request.setHeader("Content-Type", "application/xml; charset=UTF-8")
            request.setResponseCode(200)
            for cl in dbclient:
                clients += str('<client><id>%s</id><uri>%s</uri><method>%s</method><accept>%s</accept></client> ' % (cl[0], cl[1], cl[2], cl[3]))
            return str('<clients>%s</clients>' % (clients))
        else:
            request.write("<!DOCTYPE html>\n")
            flattenString(request, ClientsElement(dbclient)).addCallback(request.write)
            request.finish()

#
# POST '/windows/windows/pub'
#
@app.route('/lightbulb/sensor/pub', methods=['POST'])
def handle_windows_windows_pub_POST2(request):
    cors(request, methods=['GET', 'PUT', 'OPTIONS'])
    request.setHeader('Allow', 'GET, PUT, OPTIONS')
    json_data = json.loads(request.content.getvalue())
    logging.debug(json_data)
    logging.debug(request.requestHeaders)
    accept_type = request.requestHeaders.getRawHeaders("Accept")[0]
    lastrowid = SubscriberDB.insertClient(json_data['uri'], json_data['method'], json_data['accept'], '1')
    if not None:
        if accept_type == "application/json":
            request.setHeader("Content-Type", "application/json; charset=UTF-8")
            request.setResponseCode(200)
            return str('{"id":"%s", "uri":"%s", "method":"%s", "accept":"%s"}' % (lastrowid, json_data['uri'], json_data['method'], json_data['accept']))
        elif accept_type == "application/xml":
            request.setHeader("Content-Type", "application/xml; charset=UTF-8")
            request.setResponseCode(200)
            return str('<client><id>%s</id><uri>%s</uri><method>%s</method><accept>%s</accept></client>' % (lastrowid, json_data['uri'], json_data['method'], json_data['accept']))
        else:
            request.write("<!DOCTYPE html>\n")
            flattenString(request, ClientElement(lastrowid, json_data['uri'], json_data['method'], json_data['accept'], '')).addCallback(request.write)
            request.finish()

#
# WS '/windows/windows/pub'
#
@app.route('/lightbulb/sensor/pub/', branch=True)
def handle_windows_windows_pub_ws2(request):
    ServerFactory = xWoTBroadcastFactory
    sensor = Sensor()
    factory = ServerFactory("ws://localhost:5000/", sensor, debug = False,  debugCodePaths = False)
    factory.protocol = xWoTStreamerProtocol
    factory.setProtocolOptions(allowHixie76 = True)
    return WebSocketResource(factory)

#
# OPTIONS '/windows/windows/pub'
#
@app.route('/lightbulb/sensor/pub', methods=['OPTIONS'])
def handle_windows_windows_pub_OPTIONS2(request):
    cors(request, methods=['GET', 'PUT', 'OPTIONS'])
    request.setHeader('Allow', 'GET, PUT, OPTIONS')

