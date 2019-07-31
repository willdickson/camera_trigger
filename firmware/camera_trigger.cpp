#include "camera_trigger.h"
#include <Arduino.h>
#include <TimerOne.h>
#include <Streaming.h>

unsigned long freq_to_period_us(float freq) 
{ }


// Public methods
// ----------------------------------------------------------------------------

CameraTrigger::CameraTrigger()
{}


void CameraTrigger::initialize()
{
    Serial.begin(BAUDRATE);
    message_receiver_.reset();

    for (int i=0; i<NUM_TRIGGER_PIN; i++)
    {
        pinMode(TRIGGER_PIN[i], OUTPUT);
    }
}


void CameraTrigger::update_on_timer()
{
    timer_flag_ = true;
}


void CameraTrigger::update_trigger_pins()
{
    static unsigned long last_trigger_us = micros();

    noInterrupts();
    bool timer_flag_copy = timer_flag_;
    timer_flag_ = false;
    interrupts();

    if (timer_flag_copy)
    {
        for (int i=0; i<NUM_TRIGGER_PIN; i++)
        {
            digitalWrite(TRIGGER_PIN[i],HIGH);
        }
        last_trigger_us = micros();
    }

    if ((micros() - last_trigger_us) >=  width_us_)
    {
        for (int i=0; i<NUM_TRIGGER_PIN; i++)
        {
            digitalWrite(TRIGGER_PIN[i],LOW);
        }
    }
}


void CameraTrigger::update_on_serial_event()
{
    message_receiver_.read_data();
}


void CameraTrigger::process_messages()
{
    if (message_receiver_.available())
    {
        String msg = message_receiver_.next();
        StaticJsonBuffer<JSON_BUFFER_SIZE> json_msg_buffer;
        StaticJsonBuffer<JSON_BUFFER_SIZE> json_rsp_buffer;
        JsonObject &json_msg = json_msg_buffer.parse(msg);
        JsonObject &json_rsp = json_rsp_buffer.createObject();

        if (json_msg.success())
        {
            handle_json_message(json_msg, json_rsp);
        }
        else
        {
            json_rsp["ok"] = false;
            json_rsp["msg"] = "parse error";
        }
        json_rsp.printTo(Serial);
        Serial << endl;
    }
}


void CameraTrigger::set_timer_callback(void(*func)())
{
    timer_callback_ = func;
    Timer1.initialize((unsigned long)(1.0e6/freq_hz_));
    Timer1.attachInterrupt(timer_callback_);
    Timer1.stop();
}


// Protected methods
// ----------------------------------------------------------------------------

void CameraTrigger::handle_json_message(JsonObject &json_msg, JsonObject &json_rsp)
{
    String cmd = json_msg["cmd"] | "";

    if (cmd.equals(""))
    {
        json_rsp["ok"] = false;
        json_rsp["msg"] = "cmd missing";
    }
    else if (cmd.equals("start"))
    {
        handle_start_cmd(json_msg, json_rsp);
    }
    else if (cmd.equals("stop"))
    {
        handle_stop_cmd(json_msg, json_rsp);
    }
    else if (cmd.equals("set-freq"))
    {
        handle_set_freq_cmd(json_msg, json_rsp);
    }
    else if (cmd.equals("get-freq"))
    {
        handle_get_freq_cmd(json_msg, json_rsp);
    }
    else if (cmd.equals("set-width"))
    {
        handle_set_width_cmd(json_msg, json_rsp);
    }
    else if (cmd.equals("get-width"))
    {
        handle_get_width_cmd(json_msg, json_rsp);
    }
    else
    {
        json_rsp["ok"] = false;
        json_rsp["msg"] = "cmd unknown";
    }
}


void CameraTrigger::handle_start_cmd(JsonObject &json_msg, JsonObject &json_rsp)
{
    Timer1.start();
    json_rsp["ok"] = true;
}


void CameraTrigger::handle_stop_cmd(JsonObject &json_msg, JsonObject &json_rsp)
{
    Timer1.stop();
    json_rsp["ok"] = true;
}


void CameraTrigger::handle_set_freq_cmd(JsonObject &json_msg, JsonObject &json_rsp)
{
    float new_freq_hz = json_msg["freq"] | 0.0;

    if (new_freq_hz == 0.0)
    {
        json_rsp["ok"] = false;
        json_rsp["msg"] = "freq missing";
    }
    else
    {
        if ((new_freq_hz >= MIN_FREQ_HZ)  && (new_freq_hz <= MAX_FREQ_HZ))
        {
            Timer1.stop();
            freq_hz_ = new_freq_hz;
            Timer1.setPeriod(period_us());
            Timer1.start();
            json_rsp["ok"] = true;
        }
        else
        {
            json_rsp["ok"] = false;
            json_rsp["msg"] = "freq out of range";
        }
    }
}


void CameraTrigger::handle_get_freq_cmd(JsonObject &json_msg, JsonObject &json_rsp)
{
    json_rsp["ok"] = true;
    json_rsp["freq"] = freq_hz_;
}


void CameraTrigger::handle_set_width_cmd(JsonObject &json_msg, JsonObject &json_rsp)
{
    unsigned long new_width_us = json_msg["width"] | 0;
    if (new_width_us == 0)
    {
        json_rsp["ok"] = false;
        json_rsp["msg"] = "width missing";
    }
    else
    {
        if ((new_width_us >= MIN_WIDTH_US) && (new_width_us <= MAX_WIDTH_US))
        {
            width_us_ = new_width_us;
            json_rsp["ok"] = true;
        }
        else
        {
            json_rsp["ok"] = false;
            json_rsp["msg"] = "width out of range";
        }
    }

}


void CameraTrigger::handle_get_width_cmd(JsonObject &json_msg, JsonObject &json_rsp)
{
    json_rsp["ok"] = true;
    json_rsp["width"] = width_us_;
}



long CameraTrigger::period_us()
{
    long period_us = 0;
    if (freq_hz_ > 0)
    { 
        period_us =  long(1.0e6/freq_hz_);
    }
    return period_us;
}


