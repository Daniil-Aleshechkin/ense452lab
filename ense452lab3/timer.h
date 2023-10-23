#ifndef TIMER
#define TIMER
extern volatile unsigned int total_time;

void TIM2_IRQHandler(void);
void enable_timer(void);

#endif
