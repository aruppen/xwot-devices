#
# Generated by xwot compiler.
#
# Klein xwot application.
#
# Type:       xwot:ServiceResource
# Resource:   WindowLockResource
# Path:       /room/window/lock
#

import xwot_app
from xwot_app import app
import treq
import json
from xwot.model import Service as XWOTService
from xwot.model import BaseModel
from xwot.util import deserialize


class WindowLock(XWOTService, BaseModel):

    __mutable_props__ = ['state', 'name', 'description']
    __expose__ = __mutable_props__ + ['window_link', 'room_link']

    def __init__(self):
        super(WindowLock, self).__init__()
        self._dic = {
            'state': None,
            'name': None,
            'description': None
        }
        self._proxy_dic = {}
        self.add_type('xwot-ext:Lock')
        self.add_link('room_link')
        self.add_link('window_link')

    @property
    def resource_path(self):
        return '/room/window/lock'

    @property
    def name(self):
        return self._dic['name']

    @property
    def description(self):
        return self._dic['description']

    @property
    def room_link(self):
        return '/room'

    @property
    def window_link(self):
        return '/room/window'

    @property
    def state(self):
        return self._dic['state']

    def parse(self, data, accept):
        dic = json.loads(data)
        self._proxy_dic = dic
        self._dic['state'] = self._proxy_dic.get('state', None)
        self._dic['name'] = self._proxy_dic.get('name', None)
        self._dic['description'] = self._proxy_dic.get('description', None)
        return accept

    def serialize(self, content_type):
        if content_type == 'application/json':
            return self.to_json()

        if content_type == 'application/ld+json':
            return self.to_jsonld()

        if content_type == 'application/xml':
            return self.to_xml()

        return None

lock = WindowLock()

#
# GET '/room/window/lock'
#
@app.route('/room/window/lock', methods=['GET'])
def handle_room_window_lock_GET(request):
    if xwot_app.resources['window_lock']:
        accept = request.getHeader('Accept')
        d = treq.get(xwot_app.resources['window_lock'], headers={'Accept': 'application/json'})
        request.setHeader('Content-Type', accept)
        d.addCallback(treq.content)
        d.addCallback(lock.parse, accept)
        d.addCallback(lock.serialize)

        return d
    else:
        request.setResponseCode(404)
    return ''

#
# PUT '/room/window/lock'
#
@app.route('/room/window/lock', methods=['PUT'])
def handle_room_window_lock_PUT(request):
    if xwot_app.resources['window_lock']:
        data = request.content.read()
        content_type = request.getHeader('Content-Type')
        dic = deserialize(data, content_type)
        lock.update(dic, content_type)
        _data = lock.serialize('application/json')

        accept = request.getHeader('Accept')
        d = treq.put(xwot_app.resources['window_lock'], data=_data, headers={'Content-Type': 'application/json'})
        request.setHeader('Content-Type', accept)
        d.addCallback(treq.content)

        return lock.serialize(accept)
    else:
        request.setResponseCode(404)
    return ''
