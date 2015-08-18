# encoding: utf-8
"""
@author     Alexander Rüedlinger <a.rueedlinger@gmail.com>
@date       18.08.2015

"""

from xwot.i2c.adapter import GPSAdapter
import time
import geocoder

gps = GPSAdapter()

while True:
    print "lat: %s" % gps.latitude
    print "long: %s" % gps.longitude
    print "alt: %s" % gps.altitude
    gps_cord = [gps.latitude, gps.longitude]

    if gps.found:
        g = geocoder.google(gps_cord, method='reverse')
        print "street: %s" % g.street_long
        print "postal: %s" % g.postal
        print "housenumber: %s" % g.housenumber
        print "city: %s" % g.city
        print "country: %s" % g.country_long
        print "state: %s" % g.state_long

    time.sleep(5)
