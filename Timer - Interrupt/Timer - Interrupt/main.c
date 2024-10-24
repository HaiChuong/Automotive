#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM

void RCC_Config(){
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPCEN,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM2EN,ENABLE);
}

void GPIO_Config(){
	GPIO_InitTypeDef GPIOType;
	
	GPIOType.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIOType.GPIO_Pin  = GPIO_Pin_13;
	GPIOType.GPIO_Speed= GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOC, &GPIOType);
}

void Tim_Config(){
	TIM_TimeBaseInitTypeDef Tim;
	
	Tim.TIM_ClockDivision = TIM_CKD_DIV1;
	Tim.TIM_CounterMode		= TIM_CounterMode_Up;
	Tim.TIM_Prescaler			= 7200-1;
	Tim.TIM_Period				= 0xFFFF;
	
	TIM_TimeBaseInit(TIM2, &Tim);
	TIM_Cmd(TIM2,ENABLE);
}

void delay_ms(uint16_t time){
	TIM_SetCounter(TIM2, 0);
	while (TIM_GetCounter(TIM2)<time*10){}
}

int main(){
	RCC_Config();
	GPIO_Config();
	Tim_Config();
	while(1){
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
		delay_ms(1000);
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		delay_ms(1000);
	}
}
