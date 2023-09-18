import machine
import utime
import _thread

led_red = machine.Pin(15, machine.Pin.OUT)
led_amber = machine.Pin(14, machine.Pin.OUT)
led_green = machine.Pin(13, machine.Pin.OUT)
button = machine.Pin(16, machine.Pin.IN, machine.Pin.PULL_DOWN)

global valot_pois
valot_pois = False

def button_reader_thread():
    global valot_pois
    while True:
        if button.value() == 1:
            valot_pois = not valot_pois
            utime.sleep(0.2)
 
_thread.start_new_thread(button_reader_thread, ())
 
while True:
    if valot_pois == True:
        led_red.value(0)
        led_green.value(0)
        led_amber.toggle()
        utime.sleep(1)
    else:
        led_green.value(1)
        led_amber.value(0)
        led_red.value(0)
        utime.sleep(5)
        
        led_green.value(0)
        led_amber.value(1)
        led_red.value(0)
        utime.sleep(1)
        
        led_green.value(0)
        led_amber.value(0)
        led_red.value(1)
        utime.sleep(5)
        
        led_green.value(0)
        led_amber.value(1)
        led_red.value(1)
        utime.sleep(1)

