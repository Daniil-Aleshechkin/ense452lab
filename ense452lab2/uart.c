#include "stm32f10x.h"
#include "uart.h"
#define SERIAL_TIMEOUT 300000

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
	
	USART2->CR1 &= ~(USART_CR1_PCE); /* no parity */
	USART2->CR1 &= ~(USART_CR1_M); /* 8 data bits */
	USART2->CR2 &= ~(USART_CR2_STOP); /* 1 stop bit */
	
}

int sendbyte(uint8_t data) {
		int t;
	
		while((USART2->SR & USART_SR_TXE) == 0x0) {}
			
		USART2->DR = (data & 0xFF);
	
			
		t = 0;
		while((USART2->SR & USART_SR_TC) == 0x0 && t != SERIAL_TIMEOUT) { t++; }
		
		if (t == SERIAL_TIMEOUT) {
			return 1;
		} else {
			return 0;
		}
}

uint8_t getbyte(void) {
		while((USART2->SR & USART_SR_RXNE) == 0x0) {}
	
		return (uint8_t)(USART2->DR & 0xFF);
}

void serial_close(void) {
	/* Turn off USART2 */
	USART2->BRR = 0;
	USART2->CR1 &= ~(USART_CR1_TE | USART_CR1_RE | USART_CR1_UE);
	
	/* Turn off the GPIO ports */
	GPIOA->CRL &= ~(GPIO_CRL_MODE3 | GPIO_CRL_CNF3);
	GPIOA->CRL &= ~(GPIO_CRL_CNF2 | GPIO_CRL_MODE2);
}
