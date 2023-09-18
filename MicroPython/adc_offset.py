import machine
import utime

potentiometer = machine.ADC(28)

while True:
    val = potentiometer.read_u16()
    print(bin(val))
    print(val)
    utime.sleep(1)
