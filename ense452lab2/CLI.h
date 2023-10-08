#include "stdlib.h"
#include "stdint.h"

#ifndef CLI
#define CLI

enum Commands {
	UNKNOWN,
	TURN_ON,
	TURN_OFF,
	STATUS,
	HELP
};

void initCLI(void);

void printScreen(const uint8_t*);

void printScreenOnLine(const uint8_t*);

void clearScrean(void);

void closeCLI(void);

void CLI_Receive(uint8_t* pData, uint16_t size );
void CLI_Transmit(const uint8_t* pData, uint16_t size );

#endif
