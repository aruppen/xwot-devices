from twisted.application.service import Application
import xwot_app
from xwot_app import app

import yadp
yadp.debug()

from yadp import service
from yadp.device import Device

device = Device(urn='urn:xwot:DHT22', location=xwot_app.http_addr, descriptions=[xwot_app.yadp_description])

service = service()
service.register(device=device, passive=True)



application = Application('twisted-flask')
app.run(host='0.0.0.0', port=xwot_app.port, run_reactor=False)
