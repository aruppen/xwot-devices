# coding: utf-8
#
# Generated by xwot compiler.
#
# Klein xwot application.
#

from twisted.internet import reactor
from twisted.web.server import Site
from twisted.application.service import Application
import xwot_app
from xwot_app import app

import yadp
from yadp import service
from yadp.device import Device

device = Device(urn='urn:xwot:Window', location=xwot_app.http_addr, descriptions=[xwot_app.yadp_description])

service = service()
service.register(device=device, passive=True)

application = Application('twisted-klein')
reactor.listenTCP(xwot_app.port, Site(app.resource()), interface='0.0.0.0')
        