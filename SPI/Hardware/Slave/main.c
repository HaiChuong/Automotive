#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_spi.h"              // Keil::Device:StdPeriph Drivers:SPI
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM

#define SPI1_NSS 	GPIO_Pin_4
#define SPI1_SCK	GPIO_Pin_5
#define SPI1_MISO 	GPIO_Pin_6
#define SPI1_MOSI 	GPIO_Pin_7
#define SPI1_GPIO 	GPIOA

int i = 0;
uint8_t temp;
void RCC_Config(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}

void GPIO_Config(){
	GPIO_InitTypeDef GPIOType;
	
	GPIOType.GPIO_Pin = SPI1_NSS | SPI1_SCK | SPI1_MISO | SPI1_MOSI;
	GPIOType.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIOType.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIOType);
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

void SPI_Config(){
	SPI_InitTypeDef SPII;
	
	SPII.SPI_Mode = SPI_Mode_Slave;
	SPII.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPII.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPII.SPI_CPOL = SPI_CPOL_Low;
	SPII.SPI_CPHA = SPI_CPHA_1Edge;
	SPII.SPI_DataSize = SPI_DataSize_8b;
	SPII.SPI_FirstBit = SPI_FirstBit_LSB;
	SPII.SPI_NSS = SPI_NSS_Soft;
	
	SPI_Init(SPI1, &SPII);
	SPI_Cmd(SPI1, ENABLE);
}
/*
void SPI_Send1Byte(uint8_t data){
	GPIO_WriteBit(GPIOA, SPI1_NSS, Bit_RESET);
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1, data);
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
	GPIO_WriteBit(GPIOA,SPI1_NSS, Bit_SET);	
}
*/

uint8_t SPI_Receive1Byte(void){
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);  
		temp = (uint8_t)SPI_I2S_ReceiveData(SPI1);
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    return temp;
}


uint8_t data;
int main(){
	RCC_Config();
	GPIO_Config();
	TIM_Config();
	SPI_Config();
	while(1){
		data = SPI_Receive1Byte();
	}
}
