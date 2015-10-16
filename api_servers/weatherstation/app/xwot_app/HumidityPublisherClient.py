#
# Generated by xwot compiler.
#
# Klein xwot application.
#
# Type:       xwot:PublisherResource
# Resource:   openClosePublisherClient
# Path:       /windows/windows/pub/<clientid>
#

from xwot_app import app
from xwot_app import app
from xwot.util.klein import make_response
from xwot.util import deserialize
from xwot.util.klein import cors
from twisted.internet import task, reactor


from twisted.web.server import NOT_DONE_YET
import logging
from templates.tpl_client import ClientElement
from twisted.web.template import Element, renderer, XMLFile, flattenString
from  xwot.util.SubscriberDB import SubscriberDB

## OPTIONS '/windows/windows/pub/<clientid>'
#
@app.route('/weatherstation/sensors/humidity/pub/<clientid>', methods=['OPTIONS'])
def handle_windows_windows_pub_clientid_OPTIONS3(request, clientid):
    cors(request, methods=['GET', 'PUT', 'OPTIONS'])
    request.setHeader('Allow', 'GET, PUT, OPTIONS')

## GET '/windows/windows/pub/<clientid>'
#
@app.route('/weatherstation/sensors/humidity/pub/<clientid>', methods=['GET'])
def handle_windows_windows_pub_clientid_GET3(request, clientid):
    logging.debug(request.requestHeaders)
    cors(request, methods=['GET', 'PUT', 'OPTIONS'])
    accept_type = request.requestHeaders.getRawHeaders('Accept')[0].split(',')[0]
    client = SubscriberDB.getClient(clientid)
    if not None:
        if 'application/json' in accept_type:
            request.setHeader('Content-Type', 'application/json; charset=UTF-8')
            request.setResponseCode(200)
            return str('{"id":"%s", "uri":"%s", "method":"%s", "accept":"%s"}' % (client[0], client[1], client[2], client[3]))
        elif accept_type == 'application/xml':
            request.setHeader('Content-Type', 'application/xml; charset=UTF-8')
            request.setResponseCode(200)
            return str('<client><id>%s</id><uri>%s</uri><method>%s</method><accept>%s</accept></client>' % (client[0], client[1], client[2], client[3]))
        else:
            request.write('<!DOCTYPE html>\n')
            flattenString(request, ClientElement(client[0], client[1], client[2], client[3], '')).addCallback(request.write)
            request.finish()

## PUT '/windows/windows/pub/<clientid>'
#
@app.route('/weatherstation/sensors/humidity/pub/<clientid>', methods=['PUT'])
def handle_windows_windows_pub_clientid_PUT3(request, clientid):
    logging.debug(request.requestHeaders)
    cors(request, methods=['GET', 'PUT', 'OPTIONS'])
    request.setHeader('Allow', 'GET, PUT, OPTIONS')
    accept_type = request.requestHeaders.getRawHeaders("Accept")[0].split(
    )[0]
    #TODO Update the client in the databse
    client = SubscriberDB.getClient(clientid)
    if not None:
        if "application/json" in accept_type:
            request.setHeader("Content-Type", "application/json; charset=UTF-8")
            request.setResponseCode(200)
            return str('{"id":"%s", "uri":"%s", "method":"%s", "accept":"%s"}' % (client[0], client[1], client[2], client[3]))
        elif "application/xml" in accept_type:
            request.setHeader("Content-Type", "application/xml; charset=UTF-8")
            request.setResponseCode(200)
            return str('<client><id>%s</id><uri>%s</uri><method>%s</method><accept>%s</accept></client>' % (client[0], client[1], client[2], client[3]))
        else:
            request.write("<!DOCTYPE html>\n")
            flattenString(request, ClientElement(client[0], client[1], client[2], client[3], )).addCallback(request.write)
            request.finish()

## DELETE '/windows/windows/pub/<clientid>'
#
@app.route('/weatherstation/sensors/humidity/pub/<clientid>', methods=['DELETE'])
def handle_windows_windows_pub_clientid_DELETE3(request, clientid):
    cors(request, methods=['GET', 'PUT', 'OPTIONS'])
    request.setHeader('Allow', 'GET, PUT, OPTIONS')
    logging.debug(request.requestHeaders)
    accept_type = request.requestHeaders.getRawHeaders("Accept")[0].split(
    )[0]
    client = SubscriberDB.getClient(clientid)
    SubscriberDB.deleteQuery(clientid)
    if not None:
        if "application/json" in accept_type:
            request.setHeader("Content-Type", "application/json; charset=UTF-8")
            request.setResponseCode(200)
            return str('{"id":"%s", "uri":"%s", "method":"%s", "accept":"%s"}' % (client[0], client[1], client[2], client[3]))
        elif "application/xml" in accept_type:
            request.setHeader("Content-Type", "application/xml; charset=UTF-8")
            request.setResponseCode(200)
            return str('<client><id>%s</id><uri>%s</uri><method>%s</method><accept>%s</accept></client>' % (client[0], client[1], client[2], client[3]))
        else:
            request.write("<!DOCTYPE html>\n")
            flattenString(request, ClientElement(client[0], client[1], client[2], client[3], )).addCallback(request.write)
            request.finish()

