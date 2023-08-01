from machine import Pin,PWM
import utime

# led = Pin(2,Pin.OUT)
motor1a = Pin(4, Pin.OUT)
motor1b = Pin(5, Pin.OUT)
motor2a = Pin(6, Pin.OUT)
motor2b = Pin(7, Pin.OUT)

def ardmap(x, in_min, in_max, out_min, out_max):
    return (x - in_min) * (out_max - out_min) // (in_max - in_min) + out_min

def forward():
   motor1b.low()
   motor1a.high()
   motor2b.low()
   motor2a.high()

def backward():
   motor1a.low()
   motor1b.high()
   motor2a.low()
   motor2b.high()

def stop():
   motor1a.low()
   motor1b.low()
   motor2a.low()
   motor2b.low()

# forward()
# utime.sleep(1)
# stop()
# utime.sleep(1)
# backward()
# utime.sleep(1)

while True:
   forward()
   utime.sleep(0.2)

   # stop()
   # utime.sleep(2)

   backward()
   utime.sleep(0.2)

   # stop()
   # utime.sleep(1)
