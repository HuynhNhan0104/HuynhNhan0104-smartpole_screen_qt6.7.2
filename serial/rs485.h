#ifndef RS485_H_
#define RS485_H_


#include <stdint.h>
#include <stdio.h>
#include <string.h>
#define MAX_DATA_RS485 252

typedef struct
{
    uint8_t slave_adr;
    uint8_t func_code;
    uint8_t len;
    uint8_t data[MAX_DATA_RS485];
    uint16_t checksum;
} rs485_message;


typedef struct
{
    uint8_t slave_adr;
    uint8_t func_code;
    uint8_t effective;
    uint8_t len;
    uint8_t data[2];
    uint16_t checksum;
} rs485_rsp;


uint16_t rs485_mes_to_buffer(uint8_t *buf, const rs485_message *msg);
uint16_t crc_modbus(const uint8_t *input_str, size_t num_bytes );
uint16_t crc_modbus(const rs485_message *message);
uint16_t crc_modbus(const rs485_rsp *message);

#endif // RS485_H_