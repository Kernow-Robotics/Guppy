from machine import Pin, PWM
#p10 = machine.Pin(10)
#pwm10 = machine.PWM(p10)
#pwm10.duty(512)
pwm10 = PWM(Pin(10), freq=2000, duty_u16=0)
print("hello")