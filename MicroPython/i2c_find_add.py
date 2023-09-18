import machine
from machine import I2C, Pin


dev = I2C(1, freq=400000, scl=Pin(19), sda=Pin(18))
devices = dev.scan()
for device in devices: print(device)
#address = 72

if len(devices) == 0:
    print("No i2c device !")
else:
    print('i2c devices found:',len(devices))
for device in devices:
    print("Decimal address: ",device," | Hexa address: ",hex(device))
