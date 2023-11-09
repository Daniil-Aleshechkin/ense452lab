#include "CLI.h"
#include "LED.h"
#include "util.h"
extern const char* USER_PROMPT;
const char* USER_PROMPT = "Enter a command (help displays a list): ";

int main() {
	enum Commands inCommand;
	initCLI();
	initLED();
	turnOnLED();
	clearScrean();
	
	while(1) {
		delay(100000);
		printScreenOnLine(USER_PROMPT);
		inCommand = readCommand(); 
		processCommand(inCommand);
	}
}