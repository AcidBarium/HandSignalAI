from pwmOut import set_pwm
import time
from chuankou import send_byte

for i in range(0,100):
    # set_pwm( gpio_pin=18, frequency= i , duty_cycle=500000)
    # time.sleep(5)
    send_byte(i)
    time.sleep(0.01)