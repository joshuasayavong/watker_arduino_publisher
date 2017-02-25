import pygatt

# The BGAPI backend will attemt to auto-discover the serial device name of the
# attached BGAPI-compatible USB adapter.
adapter = pygatt.GATTToolBackend()
bluno_mac = 'C4:BE:84:15:94:6E'
serial_char_uuid = "0000dfb1-0000-1000-8000-00805f9b34fb"

try:
    adapter.start()
    device = adapter.connect(bluno_mac)
    while(true):
        value = device.char_read(serial_char_uuid)
        print(value)
finally:
    adapter.stop()
