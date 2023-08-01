from machine import Pin,PWM
import utime

MID = 1500000
MIN = 1000000
MAX = 2000000
TOTAL = 3000000

# led = Pin(2,Pin.OUT)
pwmL = PWM(Pin(2))
pwmR = PWM(Pin(19))
pwmL.freq(50)
pwmR.freq(50)

def ardmap(x, in_min, in_max, out_min, out_max):
    return (x - in_min) * (out_max - out_min) // (in_max - in_min) + out_min

def drive(left, right):
    # power is expected between -100 and 100
    powerL = TOTAL-ardmap(left, -100, 100, MIN, MAX)
    powerR = ardmap(right, -100, 100, MIN, MAX)
    pwmL.duty_ns(powerL)
    pwmR.duty_ns(powerR)

# drive(0, 0)
# utime.sleep(0.5)
while True:
    print('Forward')
    drive(100, 100)
    utime.sleep(0.5)
    print('Stop')
    drive(100, 0)
    utime.sleep(1)
    # print('Backward')
    # drive(-100, -100)
    # utime.sleep(0.5)
    # print('Stop')
    # drive(0, 0)
    # utime.sleep(0.5)
