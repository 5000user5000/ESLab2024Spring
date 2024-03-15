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
print ("Services...")
for svc in dev.services:
    print (str(svc))
try:
    testService = dev.getServiceByUUID(UUID(0xfff0)) 
    for ch in testService.getCharacteristics():
        print (str(ch))
        print("properties: "+ ch.propertiesToString())
    print("===================") 
    print("test 0xfff0")

    ch = dev.getCharacteristics(uuid=UUID(0xfff4))[0] 
    print("CCCD value original value = :", ch.read())

    #ch.write(b'm', True)
    ch.write(b"0x0002", True)

    # 檢查寫入是否成功
    print("CCCD value set to:", ch.read())

finally:
    dev.disconnect()