#include <stdlib.h>
#include <stm32f10x.h>

 #define NUC_GREEN_ON	0X00000020
 #define NUC_GREEN_OFF	0x00200000 

void init_IO(void);
void delay(int);

int main() {
	init_IO();
	
	while(1) {
		GPIOA->ODR |= GPIO_ODR_ODR5;
		delay(3000000);
		GPIOA->ODR &= ~GPIO_ODR_ODR5;
		delay(3000000);
	}
}

void init_IO() {
		RCC->APB2ENR |=  RCC_APB2ENR_IOPAEN ;
		GPIOA->CRL |= GPIO_CRL_MODE5;
    GPIOA->CRL &= ~GPIO_CRL_CNF5;
	
}

void delay(int t) {
	int i = t;
	while (i != 0) {
		i--;
	}
}
