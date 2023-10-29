#include "CLI.h"
#include "stdint.h"
#include "LED.h"
#include "util.h"
#include "uart.h"
#include "stdlib.h"
#include "string.h"

void moveCursor(unsigned int);

void initCLI() {
		clockInit();
		serial_open();
}

void moveCursor(unsigned int line) {
	char ansiMoveCode[9];
	snprintf(ansiMoveCode, sizeof(ansiMoveCode), "\x1b[%d;0H", line);
	CLI_Transmit((const uint8_t *)ansiMoveCode, sizeof(ansiMoveCode));
}

void closeCLI() {
	serial_close();
}

static const char* SAVE_POS = "\x1b[s"; 
static const char* LOAD_POS = "\x1b[u"; 
static const char* CLEAR_7_LINES = "\x1b[7M";
static const char* INSERT_7_LINES = "\x1b[7L";

void printScreen(const uint8_t* pData) {
	uint16_t size = 0;
	while (pData[size] != 0x00) { size++; }
	
	CLI_Transmit((const uint8_t*) SAVE_POS, sizeof(SAVE_POS));
	moveCursor(1);
	sendbyte(0x0a);
	CLI_Transmit((const uint8_t*) CLEAR_7_LINES, sizeof(CLEAR_7_LINES));
	CLI_Transmit((const uint8_t*) INSERT_7_LINES, sizeof(INSERT_7_LINES));
	CLI_Transmit(pData, size);
	CLI_Transmit((const uint8_t*) LOAD_POS, sizeof(LOAD_POS));
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
		if (!has_unprocessed_byte) { continue; }
		
		nextCharacter = recieved_byte;
		
		/* Byte is being processed */
		has_unprocessed_byte = false;
		
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

const static char* SET_SCROLL_WINDOW = "\x1b[5;r";
const static char* CLEAR = "\x1b[2J";
const static char* CLEAR_LINE = "\x1b[2K";

void clearScrean() {
	/* Clear the terminal */
	CLI_Transmit((const uint8_t*) CLEAR, sizeof(CLEAR));
	CLI_Transmit((const uint8_t*) CLEAR_LINE, sizeof(CLEAR_LINE));
	
	/* Reset the cursor */
	moveCursor(1);
	
	/* Create the scroll window */
	CLI_Transmit((const uint8_t*)SET_SCROLL_WINDOW, sizeof(SET_SCROLL_WINDOW));
	
	/* Move cusor to correct pos */
	moveCursor(6);
	
}
