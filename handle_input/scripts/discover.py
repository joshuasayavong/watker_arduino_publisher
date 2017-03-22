import pygatt
adapter = pygatt.Adapter()
for device in adapter.continuous_discovery():
    print(device)
    i = 0
    while (i<20):
        try:
            device.connect()
            device.set_characteristic('2b00', pygatt.UINT32, 1024)
            device.disconnect()
            break
        except:
            print("Failed")
            i = i + 1
