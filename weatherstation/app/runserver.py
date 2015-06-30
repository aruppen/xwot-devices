#
# Generated by xwot compiler.
#
# Flask xwot application.
#

import xwot_app
from xwot_app import app

import yadp
yadp.debug()

from yadp import service
from yadp.device import Device

device = Device(urn='urn:xwot:Device', location=xwot_app.http_addr, descriptions=[xwot_app.yadp_description])

service = service()
service.register(device=device, passive=True)


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=xwot_app.port, debug=True)

