#include "stm32f10x.h"
#include "timer.h"
#include "stdbool.h"
#include "uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "CLI.h"

#define SERIAL_TIMEOUT 300000
#define TIMOUT 1000

volatile uint8_t recieved_byte;
volatile bool has_unprocessed_byte;

void serial_open(void) 
{
	/* Enable Port A and USART clocks */
			
    RCC->APB2ENR |=  RCC_APB2ENR_IOPAEN;		
		RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	
		GPIOA->CRL &= ~(GPIO_CRL_MODE3 | GPIO_CRL_CNF3);
    GPIOA->CRL |= GPIO_CRL_CNF3_0; /* set floating input */
		
		GPIOA->CRL &= ~(GPIO_CRL_CNF2_0);
    GPIOA->CRL |= GPIO_CRL_CNF2_1 | GPIO_CRL_MODE2; /* alternate function output push pull 50mhz */

	USART2->CR1 |= USART_CR1_UE;
	USART2->BRR |= 0x138; /* ~115200bps, actually 115384 but it's as close as I can get it */
	USART2->CR1 |= USART_CR1_TE | USART_CR1_RE;
	USART2->CR1 |= USART_CR1_UE;
	USART2->CR1 |= USART_CR1_RXNEIE; /* Enable the UART interupt */
	
	USART2->CR1 &= ~(USART_CR1_PCE); /* no parity */
	USART2->CR1 &= ~(USART_CR1_M); /* 8 data bits */
	USART2->CR2 &= ~(USART_CR2_STOP); /* 1 stop bit */
	
	NVIC_EnableIRQ(USART2_IRQn);
	
	enable_timer();
}

int sendbyte(uint8_t data) {		
		unsigned int start_time = total_time;
	
		while((USART2->SR & USART_SR_TXE) == 0x0) {
			if (total_time - start_time > TIMOUT) {
				return 1;
			}
		}
			
		USART2->DR = (data & 0xFF);
	
			
		while((USART2->SR & USART_SR_TC) == 0x0) { 
			if (total_time - start_time > TIMOUT) {
				return 1;
			}
		}
		return 0;
}

/*
uint8_t getbyte(void) {
		while((USART2->SR & USART_SR_RXNE) == 0x0) {}
	
		return (uint8_t)(USART2->DR & 0xFF);
}*/

void serial_close(void) {
	/* Turn off USART2 */
	USART2->BRR = 0;
	USART2->CR1 &= ~(USART_CR1_TE | USART_CR1_RE | USART_CR1_UE);
	
	/* Turn off the GPIO ports */
	GPIOA->CRL &= ~(GPIO_CRL_MODE3 | GPIO_CRL_CNF3);
	GPIOA->CRL &= ~(GPIO_CRL_CNF2 | GPIO_CRL_MODE2);
}

void USART2_IRQHandler(void) {
	if (USART2->SR & USART_SR_RXNE) {
		char nextChar = USART2->DR & 0xFF;
		xQueueSend(xCLIQueue, &nextChar, 0);
	}
}
