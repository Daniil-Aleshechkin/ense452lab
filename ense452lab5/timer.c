#include "timer.h"
#include "stm32f10x.h"
#include "stdlib.h"

volatile unsigned int total_time = 0;

void enable_timer(void) {
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;	
	TIM2->PSC = 3599; 
	TIM2->ARR = 9; 
	TIM2->DIER |= TIM_DIER_UIE;
	NVIC_EnableIRQ(TIM2_IRQn); 
	TIM2->CR1 |= TIM_CR1_CEN; 
}

void TIM2_IRQHandler(void) {
	if (TIM2->SR & TIM_SR_UIF) {
		total_time++;
		
		TIM2->SR &= ~TIM_SR_UIF;
	}
}
