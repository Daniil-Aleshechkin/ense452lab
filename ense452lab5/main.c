#include "stdio.h"
#include "CLI.h"
#include "LED.h"
#include "util.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "string.h"
#include "main.h"

# define mainBLINKY_TASK_PRIORITY (tskIDLE_PRIORITY + 2)
# define mainCLI_TASK_PRIORITY (tskIDLE_PRIORITY + 1)

extern const char* USER_PROMPT;
const char* USER_PROMPT = "Enter a command (help displays a list): ";

static void vBlinkTask(void* parameters);

QueueHandle_t xCLIQueue;
QueueHandle_t xBlinkyQueue;

extern const char* HELP_TEXT;
const char* HELP_TEXT = "Commands: \
												 Fup: Increases the blinking frequency \
												 Fdown: Decreases the blinking frequency \
												 Help: Prints command list \
												 Status: Indicates LED status";

extern const char* LED_ON_STATUS_TEXT;
extern const char* LED_OFF_STATUS_TEXT;
const char* LED_ON_STATUS_TEXT  = "LED is ON";
const char* LED_OFF_STATUS_TEXT  = "LED is OFF";

extern const char* TURN_ON_LED_TEXT;
extern const char* TURN_OFF_LED_TEXT;
const char* TURN_ON_LED_TEXT = "LED turning on";
const char* TURN_OFF_LED_TEXT = "LED turning off";

extern const char* UNKNOWN_COMMAND_ERROR;
const char* UNKNOWN_COMMAND_ERROR = "Unknown command";

void processCommand(enum Commands command);

static unsigned int frequency = 5;

int main() {
	initCLI();
	initLED();
	turnOnLED();
	clearScrean();
	printScreenOnLine((const uint8_t*)USER_PROMPT);

	xCLIQueue = xQueueCreate(1, sizeof(char));
	xBlinkyQueue = xQueueCreate(1, sizeof(enum Commands));
	
	xTaskCreate(vBlinkTask, "Blinky", configMINIMAL_STACK_SIZE, NULL, mainBLINKY_TASK_PRIORITY, NULL);
	xTaskCreate(vCLITask, "CLI", configMINIMAL_STACK_SIZE, NULL, mainCLI_TASK_PRIORITY, NULL);
	vTaskStartScheduler();
	
	//closeCLI();
}

void processCommand(enum Commands command) {
	char frequencyStatus[50];
	
	switch(command){
		case FREQUENCY_UP:
			snprintf(frequencyStatus, sizeof(frequencyStatus), "Current frequency %d", frequency);
			printScreen((const uint8_t*)frequencyStatus);
			frequency ++;
			break;
		case FREQUENCY_DOWN:
			snprintf(frequencyStatus, sizeof(frequencyStatus), "Current frequency %d", frequency);
			printScreen((const uint8_t*)frequencyStatus);
			frequency--;
			break;
		case HELP:
			printScreen((const uint8_t*)HELP_TEXT);
			break;
		case STATUS:
			if (getLEDStatus()) {
				printScreen((const uint8_t*)LED_ON_STATUS_TEXT);
			} else {
				printScreen((const uint8_t*)LED_OFF_STATUS_TEXT);
			}
			break;
		case UNKNOWN:
			printScreen((const uint8_t*)UNKNOWN_COMMAND_ERROR);
			break;
	}
}

static void vBlinkTask(void * parameters) {
	enum Commands command = 0;
	for(;;){
		if (xQueueReceive(xBlinkyQueue, &command, 0) == pdTRUE) {
			processCommand(command);
			printScreenOnLine((const uint8_t*)USER_PROMPT);
		}
		GPIOA->ODR ^= GPIO_ODR_ODR5;
		vTaskDelay(1000 / frequency);
	}
}
