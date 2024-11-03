
#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM

#define SPI_SCK_Pin GPIO_Pin_4
#define SPI_MISO_Pin GPIO_Pin_5
#define SPI_MOSI_Pin GPIO_Pin_6
#define SPI_CS_Pin GPIO_Pin_7
#define SPI_GPIO GPIOA
#define SPI_RCC RCC_APB2Periph_GPIOA

int i = 0;
void RCC_Config(){
	RCC_APB2PeriphClockCmd(SPI_RCC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}

void GPIO_Config(){
	GPIO_InitTypeDef GPIOType;
	GPIOType.GPIO_Pin = SPI_SCK_Pin | SPI_MOSI_Pin | SPI_CS_Pin;
	GPIOType.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIOType.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init (SPI_GPIO, &GPIOType);
	
	GPIOType.GPIO_Pin = SPI_MISO_Pin;
	GPIOType.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIOType.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init (SPI_GPIO, &GPIOType);
	
}

void TIM_Config(){
	TIM_TimeBaseInitTypeDef TIM;
	TIM.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM.TIM_CounterMode = TIM_CounterMode_Up;
	TIM.TIM_Period = 0xFFFF;
	TIM.TIM_Prescaler = 7200-1;
	
	TIM_TimeBaseInit(TIM2, &TIM);
	TIM_Cmd(TIM2, ENABLE);
}

void delay_ms(uint16_t time){
	TIM_SetCounter(TIM2, 0);
	while (TIM_GetCounter(TIM2) < time*10);
}

void Clock(){
	GPIO_WriteBit(SPI_GPIO, SPI_SCK_Pin, Bit_SET);
	delay_ms(5);
	GPIO_WriteBit(SPI_GPIO, SPI_SCK_Pin, Bit_RESET);
	delay_ms(5);
}

void SPI_Init(){
	GPIO_WriteBit(SPI_GPIO, SPI_CS_Pin, Bit_SET);
	GPIO_WriteBit(SPI_GPIO, SPI_SCK_Pin, Bit_RESET);
	GPIO_WriteBit(SPI_GPIO, SPI_MOSI_Pin, Bit_RESET);
	GPIO_WriteBit(SPI_GPIO, SPI_MISO_Pin, Bit_RESET);
}

void SPI_Master_Transmit (uint8_t bit){
	uint8_t mask = 0x80;
	uint8_t temp;
	GPIO_WriteBit(SPI_GPIO, SPI_CS_Pin, Bit_RESET);
	delay_ms(1);
	
	for(i = 0; i < 8; i++){
		temp = bit & mask;
		if(temp){
			GPIO_WriteBit(SPI_GPIO, SPI_MOSI_Pin, Bit_SET);
			delay_ms(1);
		}else{
			GPIO_WriteBit(SPI_GPIO, SPI_MOSI_Pin, Bit_RESET);
			delay_ms(1);
		}
		bit = bit << 1;
		Clock();	
	}
	GPIO_WriteBit(SPI_GPIO, SPI_CS_Pin, Bit_SET);
	delay_ms(1);
}

uint8_t data[] = {8,4,9,14,47};
int main(){
	RCC_Config();
	GPIO_Config();
	TIM_Config();
	SPI_Init();
	while (1){
			SPI_Master_Transmit(data[0]);
			delay_ms(1000);
			SPI_Master_Transmit(data[1]);
			delay_ms(1000);
			SPI_Master_Transmit(data[2]);
			delay_ms(1000);
			SPI_Master_Transmit(data[3]);
			delay_ms(1000);
			SPI_Master_Transmit(data[4]);
			delay_ms(1000);
	}
}


