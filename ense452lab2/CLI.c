#include "CLI.h"
#include "stdint.h"
#include "LED.h"
#include "util.h"
#include "uart.h"
#include "stdlib.h"
#include "string.h"

void initCLI() {
		clockInit();
		serial_open();
}

void closeCLI() {
	serial_close();
}

void printScreen(const uint8_t* pData) {
	uint16_t size = 0;
	while (pData[size] != 0x00) { size++; }
	
	sendbyte(0x0A); /* Line feed */
	CLI_Transmit(pData, size);
	sendbyte(0x0a);
}

void printScreenOnLine(const uint8_t* pData) {
	uint16_t size = 0;
	while (pData[size] != 0x00) { size++; }
	
	CLI_Transmit(pData, size);
}

void CLI_Transmit(const uint8_t* pData, uint16_t size ) {
	int i = 0;
	for(i = 0; i < size; i++) {
		sendbyte(pData[i]);
	}
}

void CLI_Receive(uint8_t* pData, uint16_t size ) {
	char nextCharacter;
	int bufferIndex;
	bufferIndex = 0;
	nextCharacter = 0x00;
	
	
	while (nextCharacter != 0x0D) { /* Carriage return */
		nextCharacter = getbyte();
		
		if (nextCharacter == 0x7F) { /* Backspace handling */
			if (bufferIndex == 0) {
				continue;
			}
			
			sendbyte(nextCharacter);
	
			bufferIndex--;
			continue;
		}
		
		/* If the buffer is full. We need one extra char for null byte */
		if ((bufferIndex + 1) == (size - 1))  {
			continue;
		}
		
		sendbyte(nextCharacter);
		
		if (nextCharacter != 0x0D) {
			pData[bufferIndex] = nextCharacter;
			bufferIndex++;
		}
	}
	pData[bufferIndex] = 0x00;
}

void clearScrean() {
	/* Clear the terminal */
	sendbyte('\033');
	sendbyte('[');
	sendbyte('2');
	sendbyte('J');
	
	/* Reset the cursor */
	sendbyte('\033');
	sendbyte('[');
	sendbyte('H');
}
