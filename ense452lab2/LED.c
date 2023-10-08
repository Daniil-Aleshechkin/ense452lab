#include "stm32f10x.h"
#include "LED.h"
#include "stdbool.h"

void initLED() {
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; 
	
	GPIOA->CRL &= ~(GPIO_CRL_MODE5 | GPIO_CRL_CNF5);
	GPIOA->CRL |= GPIO_CRL_MODE5_0; /* set push pull output mode */
}

void turnOnLED() {
	GPIOA->ODR |= GPIO_ODR_ODR5;
}

void turnOffLED() {
	GPIOA->ODR &= ~(uint32_t)GPIO_ODR_ODR5;
}

bool getLEDStatus() {
	if ((GPIOA->IDR & GPIO_IDR_IDR5) == 0x00) {
		return false;
	} else {
		return true;
	}
}
