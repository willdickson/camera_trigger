#include "camera_trigger.h"

CameraTrigger camera_trigger;

void setup()
{
    camera_trigger.initialize();
    camera_trigger.set_timer_callback(timerEvent);
}


void loop()
{
    camera_trigger.update_trigger_pins();
    camera_trigger.process_messages();
}


void timerEvent()
{
    camera_trigger.update_on_timer();
}


void serialEvent()
{
    camera_trigger.update_on_serial_event();
}


