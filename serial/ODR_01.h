#ifndef ODR_01_H_
#define ODR_01_H_

#include <stdint.h>
#include <iostream>
#include <mutex>
#include <string.h>
#include "rs485.h"

class ODR_Interface
{

public:
    typedef enum {
        MIN_ADD = 500,
        HUMI = 500,
        TEMP,
        NOISE,
        PM25,
        PM10,
        ATM,
        LUXH,
        LUXL,
        MAX_ADD
    }Register_address;

    const char name_value[MAX_ADD-MIN_ADD][50]{
        "Humidity",
        "Temperature",
        "Noise value",
        "PM2.5 value",
        "PM10 value",
        "Atmospheric pressure value",
        "Ambient light(High Byte)",
        "Ambient light(Low Byte)"
    };

    uint16_t get_msg_encode(uint8_t *msg, uint16_t add);

    float getValue(uint16_t add, const float &value);
    float getValue(uint16_t add);

    void setValue(uint16_t add, const int &value);

    ODR_Interface() = delete;
    ODR_Interface(uint8_t ID);
    ~ODR_Interface();
private:
    std::mutex _lock;
    float _value[MAX_ADD-MIN_ADD] = {0};
    uint8_t ID;
    
};


#endif // ODR_01_H_
