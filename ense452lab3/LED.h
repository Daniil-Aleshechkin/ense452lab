#include "stdbool.h"

#ifndef LED
#define LED

void initLED(void);
void turnOnLED(void);
void turnOffLED(void);
bool getLEDStatus(void);

#endif
