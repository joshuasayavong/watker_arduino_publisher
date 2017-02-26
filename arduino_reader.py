import pygatt
import logging
import binascii

# The BGAPI backend will attemt to auto-discover the serial device name of the
# attached BGAPI-compatible USB adapter.
adapter = pygatt.GATTToolBackend()
bluno_mac = 'C8:A0:30:FA:12:9D'
serial_char_uuid = "0000dfb1-0000-1000-8000-00805f9b34fb"

try:
    adapter.start()
    device = adapter.connect(bluno_mac)
    while(True):
        for uuid in device.discover_characteristics().keys():
            print("ReadUUIDs %s: %s" % (uuid, binascii.hexlify(device.char_read(uuid))))
        #value = device.char_read(serial_char_uuid)
        #print(binascii.hexlify(value))
finally:
    adapter.stop()
