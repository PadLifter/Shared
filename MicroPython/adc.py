import machine
import utime

potentiometer = machine.ADC(28)

conversion_factor = 3.3 / (65535)

while True:
    voltage = potentiometer.read_u16() * conversion_factor
    print(str(round(voltage, 2)) + " V")
    utime.sleep(1)