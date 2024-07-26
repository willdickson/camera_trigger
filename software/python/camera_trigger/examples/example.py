import time
from camera_trigger import CameraTrigger

trig = CameraTrigger('/dev/ttyUSB0')
trig.set_freq(1000)   # frequency (Hz)
trig.set_width(500)  # pulse width (us)

print('start')
trig.start()
time.sleep(10.0)

print('stop')
trig.stop()


