from __future__ import print_function

import pygatt
import logging
import binascii

# The BGAPI backend will attemt to auto-discover the serial device name of the
# attached BGAPI-compatible USB adapter.
adapter = pygatt.GATTToolBackend()
mega_mac = 'C8:A0:30:FA:12:9D'
serial_char_uuid = "0000dfb1-0000-1000-8000-00805f9b34fb"
serial_handle = 25

try:
    adapter.start()
    device = adapter.connect(mega_mac, 10)
    while(True):
        """
        for uuid in device.discover_characteristics().keys():
            try:
                print("ReadUUIDs %s: %s" % (uuid, binascii.hexlify(device.char_read(uuid))))
                #print("ReadUUIDs %s: %s" % (uuid, binascii.hexlify(device.char_read(uuid))))

            except:
                print("UUID unreadable: %s"% uuid)
        """
        
        #value = device.char_read(serial_char_uuid)
        #print(bytearray(value))
        #print(binascii.hexlify(value))
        print("NewVal:")
        value2= device.char_read_handle(serial_handle)
        for val in value2:
            print(val)
        print(binascii.hexlify(value2))
        
finally:
    adapter.stop()
