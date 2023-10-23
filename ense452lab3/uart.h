#include "stdint.h"
#include "stdbool.h"

#ifndef UART
#define UART


/** Configure and enable the device. */
void serial_open(void);

/**
Undo whatever serial_open() did, setting the peripheral back to
its reset configuration.
*/
void serial_close(void);
/**
Send an 8-bit byte to the serial port, using the configured
bit-rate, # of bits, etc. Returns 0 on success and non-zero on
failure.
@param b the 8-bit quantity to be sent.
@pre must have already called serial_open()
*/
int sendbyte(uint8_t b);
/**
Gets an 8-bit character from the serial port, and returns it.
@pre must have already called serial_open()
*/
//uint8_t getbyte(void);

extern volatile uint8_t recieved_byte;
extern volatile bool has_unprocessed_byte;

void USART2_IRQHandler(void);
#endif
