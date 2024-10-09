#include "stm32f10x.h"                  // Device header

void delay(uint32_t time){
	for(int i = 0; i<time;i++){
	}
}

void RCC_Config(){
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
}

void toggle(){
	if(GPIOC->IDR & (1<<13))
		GPIOC->ODR &= (1<<13);
	else
		GPIOC->ODR |= (1<<13);
}

void GPIO_Config(){
	GPIOC->CRH |= GPIO_CRH_MODE13_0;
	GPIOC->CRH |= GPIO_CRH_MODE13_1;
	GPIOC->CRH &= ~GPIO_CRH_CNF13_0;
	GPIOC->CRH &= ~GPIO_CRH_CNF13_1;

	GPIOA->CRL &= ~GPIO_CRL_MODE0_0;
	GPIOA->CRL &= ~GPIO_CRL_MODE0_1;
	GPIOA->CRL &= ~GPIO_CRL_CNF0_0;
	GPIOA->CRL |= GPIO_CRL_CNF0_1;
	GPIOA->ODR |= GPIO_ODR_ODR0; 
}
int main(){
	if((GPIOA->IDR & (1<<0))==0){
		while((GPIOA->IDR & (1<<0))==0){};
			toggle();
	}
}
	
	
//	while(1){
	//	GPIOC->ODR |= 1<<13;
	//	delay(100000000);
	//	GPIOC->ODR &= ~(1<<13);
	//	delay(100000000);

