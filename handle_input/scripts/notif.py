from bluepy import btle
import binascii

class MyDelegate(btle.DefaultDelegate):
    def __init__(self):
        btle.DefaultDelegate.__init__(self)
        # ... initialise here

    def handleNotification(self, cHandle, data):
        print("Notification Recieved")
        print("Handle: %i", cHandle)
        print(data)
        for i in data:
            print(binascii.hexlify(i))
        # ... perhaps check cHandle
        # ... process 'data'


# Initialisation  -------
address = "C8:A0:30:FA:12:9D"
p = btle.Peripheral( address )
p.setDelegate( MyDelegate() )

# Setup to turn notifications on, e.g.
#   svc = p.getServiceByUUID( service_uuid )
#   ch = svc.getCharacteristics( char_uuid )[0]
#   ch.write( setup_data )

# Main loop --------

while True:
    if p.waitForNotifications(1.0):
        # handleNotification() was called
        continue

    print "Waiting..."
    # Perhaps do something else here
