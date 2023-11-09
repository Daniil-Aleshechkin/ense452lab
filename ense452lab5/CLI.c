#include "CLI.h"
#include "stdio.h"
#include "stdint.h"
#include "LED.h"
#include "util.h"
#include "uart.h"
#include "stdlib.h"
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "main.h"

#define COMMAND_STRING_BUFFER_SIZE 30

void CLI_Receive(uint8_t* pData, char nextCharacter, int* bufferIndex);

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


void CLI_Receive(uint8_t* pData, char nextCharacter, int* bufferIndex) {	
	if (nextCharacter == 0x7F) { /* Backspace handling */
		if (*bufferIndex == 0) {
			return;
		}
		
		sendbyte(nextCharacter);

		(*bufferIndex)--;
		return;
	}
	
	/* If the buffer is full. We need one extra char for null byte */
	if (((*bufferIndex) + 1) == (COMMAND_STRING_BUFFER_SIZE - 1))  {
		return;
	}
	
	sendbyte(nextCharacter);
	
	pData[(*bufferIndex)] = nextCharacter;
	(*bufferIndex)++;
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
#define TURN_ON_STRING "turnon"
#define TURN_OFF_STRING "turnoff"
#define FREQUENCY_UP_STRING "fup"
#define FREQUENCY_DOWN_STRING "fdown"
#define STATUS_STRING "status"
#define HELP_STRING "help"

enum Commands parseReturnCommand(char*);
void toLowerCase(char*);

void toLowerCase(char* str) {
    int i = 0;
    while (str[i] != 0x00) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            str[i] = str[i] + 32;
        }
        i++;
    }
}

enum Commands parseReturnCommand(char* commandInput) {
	if(strcmp(FREQUENCY_UP_STRING, commandInput) == 0x00) {
		return FREQUENCY_UP;
	} else if (strcmp(FREQUENCY_DOWN_STRING, commandInput) == 0x00) {
		return FREQUENCY_DOWN;
	} else if (strcmp(HELP_STRING, commandInput) == 0x00) {
		return HELP;
	} else if (strcmp(STATUS_STRING, commandInput) == 0x00) {
		return STATUS;
	} else {
		return UNKNOWN;
	}
}


void vCLITask(void* parameters) {
	char recievedChar;
	uint8_t* commandStringBuffer = (uint8_t*)malloc(sizeof(char) * COMMAND_STRING_BUFFER_SIZE);
	int bufferIndex = 0;
	enum Commands command;
	
	for(;;){
		if (xQueueReceive(xCLIQueue, &recievedChar, 0) == pdTRUE) {
			if (recievedChar == 0x0D) {
				// Send command code here
				toLowerCase((char *)commandStringBuffer);
				command = parseReturnCommand((char *)commandStringBuffer);
				xQueueSend(xBlinkyQueue, &command, 0);
				
				commandStringBuffer[bufferIndex] = 0x00;
				bufferIndex = 0x00;
			} else {
				CLI_Receive(commandStringBuffer, recievedChar, &bufferIndex);
			}
		}
	}
}
