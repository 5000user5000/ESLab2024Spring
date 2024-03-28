from bluepy.btle import Peripheral, UUID
from bluepy.btle import Scanner, DefaultDelegate
import time

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
    accServiceHeartRate = dev.getServiceByUUID(UUID(0x180D)) 
    accService = dev.getServiceByUUID(UUID(0xA000)) 
    for ch in accService.getCharacteristics():
        print (str(ch))
        if(str(ch)=="Characteristic <Heart Rate Measurement>"):
            print("found HEART RATE")
            ch_acc_heartRate = ch
        if(str(ch)=="Characteristic <a001>"):
            print("found button maybe")
            ch_acc_Button = ch
        print("properties: "+ ch.propertiesToString())

    for ch in accServiceHeartRate.getCharacteristics():
        print (str(ch))
        if(str(ch)=="Characteristic <Heart Rate Measurement>"):
            print("found HEART RATE")
            ch_acc_heartRate = ch
        if(str(ch)=="Characteristic <a001>"):
            print("found button maybe")
            ch_acc = ch
        print("properties: "+ ch.propertiesToString())

    if ("READ" in ch_acc_Button.propertiesToString()):
        print("test read")
        while True:
            time.sleep(0.1)
            b_str = ((ch_acc_Button.read()))
            b_str = int.from_bytes(b_str, byteorder='big')
            heart_str = ((ch_acc_heartRate.read()))
            heart_str = int.from_bytes(heart_str, byteorder='big')
            print(f"Button status: {b_str}")
            print(f"HEART RATE: {heart_str}")
            print("")


    # if ("READ" in ch_acc.propertiesToString()):
    #     print("test read")
    #     while True:
    #         time.sleep(0.1)
    #         b_str = ch_acc.read()
    #         print(f"HEART RATE: {b_str}")
finally:
    dev.disconnect()