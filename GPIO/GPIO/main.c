#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC

void delay(uint32_t time){
	for(int i = 0; i < time; i++){}
}

void RCC_Config(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA ,ENABLE);
}

void GPIO_Config(){
	GPIO_InitTypeDef GPIOType;
	GPIOType.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIOType.GPIO_Pin = GPIO_Pin_13| GPIO_Pin_5 |GPIO_Pin_6 |GPIO_Pin_7| GPIO_Pin_8;
	GPIOType.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOC, &GPIOType);
	
//	GPIOType.GPIO_Mode = GPIO_Mode_IPU;
// GPIOType.GPIO_Pin = GPIO_Pin_0;
// GPIOType.GPIO_Speed = GPIO_Speed_50MHz;

// GPIO_Init(GPIOA, &GPIOType);
	
}
void nhayDen(uint8_t num){
	uint16_t Ledval;
		for(int i = 0; i<num; i++){
			Ledval = 0x0010;
			for(int i = 0; i<4;i++){
				Ledval = Ledval<<1;
				GPIO_SetBits(GPIOC, Ledval);
				delay(3000000);		
			}
			Ledval = 0x0010;
			for(int i = 0;i<4;i++){
				Ledval = Ledval<<1;
				GPIO_WriteBit(GPIOC, Ledval, Bit_RESET);
				delay(3000000);
		}
	}
}

int main(){
	RCC_Config();
	GPIO_Config();
	
		nhayDen(6);
	return 0;
}
