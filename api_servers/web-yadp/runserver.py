# encoding: utf-8
"""
@author     Alexander Rüedlinger <a.rueedlinger@gmail.com>
@date       22.07.2015

"""
import yadp
from yadp import service
from yadp.util import web
from xwot.util import local_ip

ip = local_ip()
port = 5000
http_addr = "http://%s:%s/" % (ip, port)
device = yadp.create_device(urn='urn:yadp:web', location=http_addr)

if __name__ == '__main__':
    server = web.create_server(server_port=port, host='0.0.0.0')
    service = service()
    service.register(device, passive=True)
    server.run()