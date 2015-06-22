#
# Generated by xwot compiler.
#
# Flask xwot application.
#

import os
from yadp.device import Description

from xwot.util import local_ip
from xwot.util import create_description
from xwot.util import dir_path
from xwot.util import parent_dir_path

# base config
ip = local_ip()
port = 5000
http_addr = "http://%s:%s/" % (ip, port)
module_dir_path = dir_path(__file__)
app_dir_path = parent_dir_path(__file__)
xwot_file = os.path.join(app_dir_path, "device.xwot")

jsonld_description_str = create_description(xwot_file=xwot_file, base=http_addr)
yadp_description = Description(content_type="application/ld+json", content=jsonld_description_str)


from flask import Flask
from flask.ext.twisted import Twisted

app = Flask(__name__)
twisted = Twisted(app)

import RootResource
import WaterDispenserResource
import SensorResource
import ValveResource

