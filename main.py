from machine import Pin,PWM
import utime

MID = 1500000
MIN = 1000000
MAX = 2000000
TOTAL = 3000000

led = Pin(25,Pin.OUT)
pwmL = PWM(Pin(0))
pwmR = PWM(Pin(1))
pwmL.freq(50)
pwmR.freq(50)

def ardmap(x, in_min, in_max, out_min, out_max):
    return (x - in_min) * (out_max - out_min) // (in_max - in_min) + out_min

def drive(left, right):
    # power is expected between -100 and 100
    powerL = ardmap(left, -100, 100, MIN, MAX)
    powerR = TOTAL-ardmap(right, -100, 100, MIN, MAX)
    pwmL.duty_ns(powerL)
    pwmR.duty_ns(powerR)

while True:
    drive(50, 50)
    utime.sleep(1)
    drive(50, -50)
    utime.sleep(1)
