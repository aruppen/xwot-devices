# coding: utf-8
from twisted.internet import reactor
from twisted.web.server import Site
from twisted.application.service import Application

import yadp
from yadp import service
from yadp.util import web
from xwot.util import local_ip

ip = local_ip()
port = 5000
http_addr = "http://%s:%s/" % (ip, port)
device = yadp.create_device(urn='urn:yadp:web', location=http_addr)

service = service()
service.register(device=device, passive=True)
server = web.create_server()
app = server.app

application = Application('twisted-klein')
reactor.listenTCP(port, Site(app.resource()), interface='0.0.0.0')