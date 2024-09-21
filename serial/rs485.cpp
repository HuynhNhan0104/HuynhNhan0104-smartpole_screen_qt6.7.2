#include "rs485.h"
#include <stdio.h>
#include <string.h>


#define		CRC_START_16		0x0000
#define		CRC_START_MODBUS	0xFFFF
#define		CRC_POLY_16		    0xA001

static void             init_crc16_tab( void );

static bool             crc_tab16_init          = false;
static uint16_t         crc_tab16[256];

uint16_t rs485_mes_to_buffer(uint8_t *buf, const rs485_message *msg)
{
    buf[0] = msg->slave_adr;
    buf[1] = msg->func_code;
    memcpy(buf + 2, msg->data, msg->len);
    buf[msg->len + 2] = (uint8_t)(msg->checksum & 0xFF);
    buf[msg->len + 3] = (uint8_t)(msg->checksum >> 8);
    return (uint16_t)(msg->len + 4);
}


/*
 * uint16_t crc_16( const unsigned char *input_str, size_t num_bytes );
 *
 * The function crc_16() calculates the 16 bits CRC16 in one pass for a byte
 * string of which the beginning has been passed to the function. The number of
 * bytes to check is also a parameter. The number of the bytes in the string is
 * limited by the constant SIZE_MAX.
 */

uint16_t crc_16( const unsigned char *input_str, size_t num_bytes ) {

	uint16_t crc;
	const unsigned char *ptr;
	size_t a;

	if ( ! crc_tab16_init ) init_crc16_tab();

	crc = CRC_START_16;
	ptr = input_str;

	if ( ptr != NULL ) for (a=0; a<num_bytes; a++) {

		crc = (crc >> 8) ^ crc_tab16[ (crc ^ (uint16_t) *ptr++) & 0x00FF ];
	}

	return crc;

}  /* crc_16 */

/*
 * uint16_t crc_modbus( const unsigned char *input_str, size_t num_bytes );
 *
 * The function crc_modbus() calculates the 16 bits Modbus CRC in one pass for
 * a byte string of which the beginning has been passed to the function. The
 * number of bytes to check is also a parameter.
 */

uint16_t crc_modbus( const uint8_t *input_str, size_t num_bytes ) {

	uint16_t crc;
	const uint8_t *ptr;
	size_t a;

	if ( ! crc_tab16_init ) init_crc16_tab();

	crc = CRC_START_MODBUS;
	ptr = input_str;

	if ( ptr != NULL ) for (a=0; a<num_bytes; a++) {

		crc = (crc >> 8) ^ crc_tab16[ (crc ^ (uint16_t) *ptr++) & 0x00FF ];
	}

	return crc;

}  /* crc_modbus */

/*
 * uint16_t update_crc_16( uint16_t crc, unsigned char c );
 *
 * The function update_crc_16() calculates a new CRC-16 value based on the
 * previous value of the CRC and the next byte of data to be checked.
 */

uint16_t update_crc_16( uint16_t crc, unsigned char c ) {

	if ( ! crc_tab16_init ) init_crc16_tab();

	return (crc >> 8) ^ crc_tab16[ (crc ^ (uint16_t) c) & 0x00FF ];

}  /* update_crc_16 */

/*
 * static void init_crc16_tab( void );
 *
 * For optimal performance uses the CRC16 routine a lookup table with values
 * that can be used directly in the XOR arithmetic in the algorithm. This
 * lookup table is calculated by the init_crc16_tab() routine, the first time
 * the CRC function is called.
 */

static void init_crc16_tab( void ) {

	uint16_t i;
	uint16_t j;
	uint16_t crc;
	uint16_t c;

	for (i=0; i<256; i++) {

		crc = 0;
		c   = i;

		for (j=0; j<8; j++) {

			if ( (crc ^ c) & 0x0001 ) crc = ( crc >> 1 ) ^ CRC_POLY_16;
			else                      crc =   crc >> 1;

			c = c >> 1;
		}

		crc_tab16[i] = crc;
	}

	crc_tab16_init = true;

}  /* init_crc16_tab */



void update_crc16(uint16_t &crc, uint8_t byte){
    crc ^= byte;
    for(int bit = 0; bit < 8; bit++)
    {
        if( crc & 0x0001 )
        {
            crc >>= 1;
            crc ^= 0xA001;
        }
        else
        {
            crc >>= 1;
        }
    }
}
uint16_t crc_modbus(const rs485_message *message){
    uint16_t crc = 0xffff;
    update_crc16(crc,message->slave_adr);
    update_crc16(crc,message->func_code);
    for (int i = 0; i < message->len; ++i){
        update_crc16(crc,message->data[i]);
    }
    return crc;
}

uint16_t crc_modbus(const rs485_rsp *message){
    uint16_t crc = 0xffff;
    update_crc16(crc,message->slave_adr);
    update_crc16(crc,message->func_code);
	update_crc16(crc,message->effective);
    for (int i = 0; i < message->len; ++i){
        update_crc16(crc,message->data[i]);
    }
    return crc;
}