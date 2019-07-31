camera_trigger
--------
Python library which enables control of camera trigger pulses produced by an Arduino.

Example
--------

``` python
import time
from camera_trigger import CameraTrigger

trig = CameraTrigger('/dev/ttyUSB0')
trig.set_freq(50)   # frequency (Hz)
trig.set_width(50)  # pulse width (us)

print('start')
trig.start()
time.sleep(10.0)

print('stop')
trig.stop()

```


Installation
------------

```
#!bash

$ pip install camera_trigger 


```


Links
-----

* Download https://github.com/willdickson/autostep
* Documentation [TODO] 


