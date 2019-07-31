#ifndef PS_MESSAGE_RECEIVER_H 
#define PS_MESSAGE_RECEIVER_H

#include <Arduino.h>
#include "constants.h"
#include "circular_buffer.h"


class MessageReceiver
{

    public:

        MessageReceiver();
        void reset();
        void read_data();
        String next();
        bool available() const;
        uint32_t get_message_cnt() const;
        uint32_t get_total_message_cnt() const;

    protected:

        CircularBuffer<char,SERIAL_BUFFER_SIZE> serial_buffer_;
        bool overflow_ = false;
        uint32_t message_cnt_ = 0;
        uint32_t total_message_cnt_ = 0;


};

#endif
