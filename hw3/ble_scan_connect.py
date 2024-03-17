from bluepy.btle import Peripheral, UUID
from bluepy.btle import Scanner, DefaultDelegate

class ScanDelegate(DefaultDelegate): 
    def __init__(self):
        DefaultDelegate.__init__(self)
    def handleDiscovery(self, dev, isNewDev, isNewData):
        if isNewDev:
                print ("Discovered device", dev.addr)
        elif isNewData:
            print ("Received new data from", dev.addr)

class MyDelegate(DefaultDelegate):
    def __init__(self):
        DefaultDelegate.__init__(self)

    def handleNotification(self, cHandle, data):
        print("Received data: {}".format(data))


scanner = Scanner().withDelegate(ScanDelegate())
devices = scanner.scan(1.0)
n=0
addr = []

myDevice = 0
devices =  sorted(devices, key=lambda x: x.rssi, reverse=True)
for dev in devices:
    print ("%d: Device %s (%s), RSSI=%d dB" % (n, dev.addr, dev.addrType, dev.rssi))
    addr.append(dev.addr)
    n += 1
    for (adtype, desc, value) in dev.getScanData():
        print (" %s = %s" % (desc, value))

print(myDevice)
number = input('Enter your device number: ')
print ('Device', number)
num = int(number)
print (addr[num])

print ("Connecting...")
dev = Peripheral(addr[num], 'random')
dev.setDelegate(MyDelegate())

print ("Services...")
for svc in dev.services:
    print (str(svc))
try:
    testService = dev.getServiceByUUID(UUID(0x180D)) 
    for ch in testService.getCharacteristics():
        print (str(ch))
        print("properties: "+ ch.propertiesToString())
    print("===================") 

    ch = dev.getCharacteristics(uuid=UUID(0x2A37))[0]
    print("ch:", ch)
    cccd =  ch.getDescriptors(UUID(0x2902))[0]
    cccd.write(b"\x01\x00", withResponse=True) # enable notification
    #cccd.write(b"\x02\x00", withResponse=True) # enable indicate
    while True:
        if dev.waitForNotifications(1.0):
            print("Waiting...")

finally:
    dev.disconnect()