from time import sleep
import spidev
import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BCM)
GPIO.setup(27,GPIO.OUT)

spi = spidev.SpiDev()

spi.open(0,0)
spi.max_speed_hz = 1000
#spi.lsbfirst = True // not supported on RPi

# 0xA0 (inverted LED off)
# 0x20 (inverted LED on)
# 0x40 (inverted echo)

to_send = [0x40,0x00]

GPIO.output(27,True)
print ("sending")
resp = spi.xfer(to_send)
print(resp)              
GPIO.output(27,False)

sleep(0.01)

to_send[0] = 0x20
print ("sending")
resp = spi.xfer(to_send)
print(resp)  

sleep(5)

to_send[0] = 0xA0
print ("sending")
resp = spi.xfer(to_send)
print(resp)  


GPIO.cleanup()
spi.close()         # clean up
print ("All cleaned up.")
