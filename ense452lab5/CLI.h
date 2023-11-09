#include "stdlib.h"
#include "stdint.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#ifndef CLI
#define CLI

enum Commands {
	UNKNOWN,
	STATUS,
	HELP,
	FREQUENCY_UP,
	FREQUENCY_DOWN,
};

extern QueueHandle_t xCLIQueue;

#define mainCLI_TASK_PRIORITY (tskIDLE_PRIORITY + 1)
void vCLITask(void* parameters);

void initCLI(void);

void printScreen(const uint8_t*);

void printScreenOnLine(const uint8_t*);

void clearScrean(void);

void closeCLI(void);

void CLI_Transmit(const uint8_t* pData, uint16_t size );

#endif
