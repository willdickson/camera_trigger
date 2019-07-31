#include "message_receiver.h"
#include <util/atomic.h>

MessageReceiver::MessageReceiver() 
{ }

void MessageReceiver::reset()
{
    overflow_ = false;
    message_cnt_ = 0;
}


void MessageReceiver::read_data()
{
    while(Serial.available() > 0)
    {
        char byte = Serial.read();
        if (!serial_buffer_.full())
        {
            serial_buffer_.push_back(byte);
            if (byte == '\n')
            {
                message_cnt_++;
                total_message_cnt_++;
            }
        }
        else
        {
            overflow_ = true;
        }
    }
}


String MessageReceiver::next()
{
    String message("");
    if (message_cnt_ > 0)
    {
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {   
            while (!serial_buffer_.empty())
            {
                char byte = serial_buffer_.front();
                serial_buffer_.pop_front();
                if (byte == '\n')
                {
                    break;
                }
                message += String(byte);
            }
            message_cnt_--;
        }
    }
    return message;
}


bool MessageReceiver::available() const
{
    return (message_cnt_ > 0);
}


uint32_t MessageReceiver::get_message_cnt() const
{
    return message_cnt_;
}


uint32_t MessageReceiver::get_total_message_cnt() const
{
    return total_message_cnt_;
}

