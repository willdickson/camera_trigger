#ifndef CAMERA_TRIGGER_H
#define CAMERA_TRIGGER_H

#include <ArduinoJson.h>
#include "constants.h"
#include "message_receiver.h"

class CameraTrigger
{
    public:

        CameraTrigger();

        void initialize();
        void update_on_timer();
        void update_on_serial_event();
        void update_trigger_pins();
        void process_messages();
        void set_timer_callback(void(*func)());

    protected:

        MessageReceiver message_receiver_;
        volatile bool timer_flag_ = false;
        float freq_hz_ = DEFAULT_FREQ_HZ; 
        unsigned long width_us_ = DEFAULT_WIDTH_US;

        static void dummy_callback_(){};
        void (*timer_callback_)() = dummy_callback_;

        void handle_json_message(JsonObject &json_msg, JsonObject &json_rsp);
        void handle_start_cmd(JsonObject &json_msg, JsonObject &json_rsp);
        void handle_stop_cmd(JsonObject &json_msg, JsonObject &json_rsp);
        void handle_set_freq_cmd(JsonObject &json_msg, JsonObject &json_rsp);
        void handle_get_freq_cmd(JsonObject &json_msg, JsonObject &json_rsp);
        void handle_set_width_cmd(JsonObject &json_msg, JsonObject &json_rsp);
        void handle_get_width_cmd(JsonObject &json_msg, JsonObject &json_rsp);

        long period_us();

};

#endif
