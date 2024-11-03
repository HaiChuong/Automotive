
# SPI




## Software
### *Master*
Định nghĩa các chân SPI và PORT sử dụng để truyền SPI
```
#define SPI_SCK_Pin GPIO_Pin_4
#define SPI_MISO_Pin GPIO_Pin_5
#define SPI_MOSI_Pin GPIO_Pin_6
#define SPI_CS_Pin GPIO_Pin_7
#define SPI_GPIO GPIOA
#define SPI_RCC RCC_APB2Periph_GPIOA
```
Cấp clock cho các ngoại vi cần sử dụng (GPIOA và TIM2)
```
void RCC_Config(){
	RCC_APB2PeriphClockCmd(SPI_RCC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}
```
Cấu hình chế độ cho các chân Master
```
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
```
Chân SPI_MISO_Pin kiểu FLOATING để khi không sử dụng sẽ thả trôi điện áp, ko xác định ở 0 hoặc 1.

Cấu hình tạo delay_ms bằng timer 2.
```
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
```
Tạo hàm clock để tạo clock mỗi khi ghi 1 bit dữ liệu bằng cách cho lần lượt chân SPI_SCK_Pin lên 1, delay, xuống 0, delay.
```
void Clock(){
	GPIO_WriteBit(SPI_GPIO, SPI_SCK_Pin, Bit_SET);
	delay_ms(5);
	GPIO_WriteBit(SPI_GPIO, SPI_SCK_Pin, Bit_RESET);
	delay_ms(5);
}
```
Tạo điều kiện khởi động cho SPI (đặt tất các các bit ở trạng thái nghỉ)
```
void SPI_Init(){
	GPIO_WriteBit(SPI_GPIO, SPI_CS_Pin, Bit_SET);
	GPIO_WriteBit(SPI_GPIO, SPI_SCK_Pin, Bit_RESET);
	GPIO_WriteBit(SPI_GPIO, SPI_MOSI_Pin, Bit_RESET);
	GPIO_WriteBit(SPI_GPIO, SPI_MISO_Pin, Bit_RESET);
}
```
Tạo hàm truyền dữ liệu từ Master qua Slave. Quy trình của hàm bao gồm:
- Truyền lần lượt 8 bit trong byte dữ liệu.
- Kéo chân CS xuống 0 xác định kết nối với Slave. Truyền 1 bit, dịch 1 bit để truyền bit kế tiếp, mỗi bit gửi đi sẽ kèm theo 1 clock.
- Kéo chân CS lên 1 kết thúc kết nối với Slave.
Tạo 1 mask 0x80 để gửi MSB qua trước và dịch bit để truyền bit tiếp theo cho đến LSB, và 1 biến tạm để lưu kết quả giữa dữ liệu và mask. Đồng thời kéo chân CS xuống 0 xác định bắt đầu kết nối với Slave.
```
    uint8_t mask = 0x80;
	uint8_t temp;
	GPIO_WriteBit(SPI_GPIO, SPI_CS_Pin, Bit_RESET);
	delay_ms(1);
```
Tạo hàm truyền byte dữ liệu theo từng bit từ MSB. Sử dụng mask & data lưu vào tempData để đọc từng bit. Nếu bit là 1 thì kéo chân MOSI lên 1 đồng thời dịch bit chuẩn bị cho bit tiếp theo (tương tự bit là 0). Sau khi dịch thì kết thúc lượt truyền bit gửi kèm theo 1 clock. Thực hiện 8 lần cho 1 byte dữ liệu.
```
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
```
Sau khi truyền xong thì kéo chân CS lên 1 kết thúc truyền 1 byte dữ liệu cũng như kết thúc kết nối với Slave.
```
GPIO_WriteBit(SPI_GPIO, SPI_CS_Pin, Bit_SET);
delay_ms(1);
```
Hàm truyền 1 byte hoàn chỉnh:
```
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
```
Bắt đầu truyền dữ liệu ở Master:
```
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
```



### *Slave*
Định nghĩa các chân truyền SPI và PORT. Cấp clock cho ngoại vi, tạo delay tương tự như Master. 
```
#define SPI_SCK_Pin GPIO_Pin_4
#define SPI_MISO_Pin GPIO_Pin_5
#define SPI_MOSI_Pin GPIO_Pin_6
#define SPI_CS_Pin GPIO_Pin_7
#define SPI_GPIO GPIOA
#define SPI_RCC RCC_APB2Periph_GPIOA

int i = 0;
void RCC_Config(){
	RCC_APB2PeriphClockCmd(SPI_RCC , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
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
	while(TIM_GetCounter(TIM2) < time * 10);
}
```
Cấu hình cho chế độ cho các chân Slave
```
void GPIO_Config(){
	GPIO_InitTypeDef GPIOType;
	
	GPIOType.GPIO_Pin = SPI_SCK_Pin | SPI_MOSI_Pin | SPI_CS_Pin;
	GPIOType.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOType.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	
	GPIO_Init(SPI_GPIO, &GPIOType);
	
	GPIOType.GPIO_Pin = SPI_MISO_Pin;
	GPIOType.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOType.GPIO_Mode = GPIO_Mode_Out_PP;
	
	GPIO_Init(SPI_GPIO, &GPIOType);
}
```
Các chân cấu hình ngược lại so với Master

Tạo hàm nhận dữ liệu từ Master qua Slave. Quy trình của hàm bao gồm:
- Kiểm tra liên tục chân CS = 0 sẽ xác nhận Master muốn kết nối với Slave.
- Kiểm tra đã có xung clock từ Master truyền qua chưa. Nếu có sẽ đọc bit và dịch 1 bit cho bit tiếp theo.
- Kiểm tra lại chân CS = 1 xác nhận ngắt kết nối với Master.
Khởi tạo 1 biến để nhận dữ liệu dataReceive và 1 biến để đọc dữ liệu temp từ MOSI ghi vào dataReceive. Đồng thời kiểm tra liên tục chân CS có được kéo xuống 0 chưa.
```
uint8_t dataReceive = 0x00;	
uint8_t temp = 0x00;
while(GPIO_ReadInputDataBit(SPI_GPIO, SPI_CS_Pin));
```
Sau khi xác nhận CS = 0 sẽ đợi xung clock của Master được gửi đến và xác nhận truyền dữ liệu. Khi có clock được gửi đến ta sẽ đọc tín hiệu ở chân MOSI ghi vào temp, sử dụng dataReceive |= temp kết hợp với dịch bit để ghi từng bit dữ liệu vào dataReceive. Thực hiện 8 lân đến khi kết thúc byte dữ liệu. Sau khi truyền sau sẽ chờ clock kết thúc và chờ ngắt kết nối với Master, hàm trả về byte dữ liệu vừa được gửi.
```
while(!GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin));
	for(i = 0; i < 8; i++){ 
		if(GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin)){
			while (GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin)){
				temp = GPIO_ReadInputDataBit(SPI_GPIO, SPI_MOSI_Pin);
			}
			dataReceive <<= 1;
			dataReceive |= temp;
    		}
		while(!GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin)); //chờ clock kết thúc
	}
	while(!GPIO_ReadInputDataBit(SPI_GPIO, SPI_CS_Pin)); // chờ CS = 1 ngắt kết nối với Master
	return dataReceive;
}
```
Hàm nhận dữ liệu ở Slave hoàn chỉnh:
```
uint8_t SPI_Slave_Receive(void){
	uint8_t dataReceive = 0x00;	
	uint8_t temp = 0x00;
	while(GPIO_ReadInputDataBit(SPI_GPIO, SPI_CS_Pin));
	while(!GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin));
	for(i = 0; i < 8; i++){ 
		if(GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin)){
			while (GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin)){
				temp = GPIO_ReadInputDataBit(SPI_GPIO, SPI_MOSI_Pin);
			}
			dataReceive <<= 1;
			dataReceive |= temp;
    		}
		while(!GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin));
	}
	while(!GPIO_ReadInputDataBit(SPI_GPIO, SPI_CS_Pin));
	return dataReceive;
}
```
Bắt đầu nhận dữ liệu ở Slave
```
uint8_t data;
int main(){
	RCC_Config();
	GPIO_Config();
	TIM_Config();
	while(1){
		data = SPI_Slave_Receive();
	}
}
```
## Hardware
### *Master*
Định nghĩa các chân GPIO ở chế độ thay thế SPI1. Các chân SPI1 được xác định ở các chân GPIOA Pin 4,5,6,7.  
![5](https://github.com/user-attachments/assets/f6fdcaff-076a-45bc-98f5-dd4898216f99)
```
#define SPI1_NSS 	GPIO_Pin_4
#define SPI1_SCK	GPIO_Pin_5
#define SPI1_MISO 	GPIO_Pin_6
#define SPI1_MOSI 	GPIO_Pin_7
#define SPI1_GPIO 	GPIOA
```
Cấp clock cho các ngoại vi được sử dụng
```
void RCC_Config(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}
```
Cấu hình cho tạo delay bằng timer 2
```
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
```
Cấu hình GPIO cho các chân SPI1
```
void GPIO_Config(){
	GPIO_InitTypeDef GPIOType;
	
	GPIOType.GPIO_Pin = SPI1_NSS | SPI1_SCK | SPI1_MISO | SPI1_MOSI;
	GPIOType.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIOType.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIOType);
}
```
Cấu hình cho SPI1
```
void SPI_Config(){
	SPI_InitTypeDef SPII;
	
	SPII.SPI_Mode = SPI_Mode_Master;
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
```
Với các tham số SPI được cấu hình:
- SPI_Mode: Chọn chế độ hoạt động của thiết bị (chọn tương ứng SPI_Mode_Master hoặc SPI_Mode_Slave).
- SPI_Direction: Chọn chế độ tương ứng với kiểu truyền của ta.
```
#define SPI_Direction_2Lines_FullDuplex ((uint16_t)0x0000)
#define SPI_Direction_2Lines_RxOnly     ((uint16_t)0x0400)
#define SPI_Direction_1Line_Rx          ((uint16_t)0x8000)
#define SPI_Direction_1Line_Tx          ((uint16_t)0xC000)
```
-  SPI_BaudRatePrescaler: Hệ số chia cấp clock cho SPI1
```
#define SPI_BaudRatePrescaler_2         ((uint16_t)0x0000)
#define SPI_BaudRatePrescaler_4         ((uint16_t)0x0008)
#define SPI_BaudRatePrescaler_8         ((uint16_t)0x0010)
#define SPI_BaudRatePrescaler_16        ((uint16_t)0x0018)
#define SPI_BaudRatePrescaler_32        ((uint16_t)0x0020)
#define SPI_BaudRatePrescaler_64        ((uint16_t)0x0028)
#define SPI_BaudRatePrescaler_128       ((uint16_t)0x0030)
#define SPI_BaudRatePrescaler_256       ((uint16_t)0x0038)
```
- SPI_CPOL: Cấu hình cực tính của SCK . Có 2 chế độ:
```
#define SPI_CPOL_Low  // Cực tính mức 0 khi SCK ko truyền xung
#define SPI_CPOL_High // Cực tính mức 1 khi SCK ko truyền xung
```
- SPI_CPHA: Cấu hình chế độ hoạt động của SCK. Có 2 chế độ:
```
#define SPI_CPHA_1Edge  // Tín hiệu truyền đi ở cạnh xung đầu tiên.                
#define SPI_CPHA_2Edge //  Tín hiệu truyền đi ở cạnh xung thứ hai.
            
```
- SPI_DataSize: Kích thước dữ liệu truyền đi
```
#define SPI_DataSize_16b                ((uint16_t)0x0800)
#define SPI_DataSize_8b                 ((uint16_t)0x0000)
```
- SPI_FirstBit: Chọn thứ tự truyền theo LSB hay MSB 
```
#define SPI_FirstBit_MSB                ((uint16_t)0x0000)
#define SPI_FirstBit_LSB                ((uint16_t)0x0080)
```
- SPI_NSS: Cấu hình chân SS là điều khiển bằng thiết bị hay phần mềm.
```
#define SPI_NSS_Soft                    ((uint16_t)0x0200)
#define SPI_NSS_Hard                    ((uint16_t)0x0000)
```
Các hàm thông dụng khi truyền SPI:
- SPI_I2S_SendData(SPI_TypeDef* SPIx, uint16_t Data): Truyền dữ liệu. Nếu gửi dữ liệu 8 bit thì 8 bit đầu sẽ là bit trống.
- SPI_I2S_ReceiveData(SPI_TypeDef* SPIx): Nhận dữ liệu. Hàm trả về dữ liệu 8 hoặc 16 bit.
- SPI_I2S_GetFlagStatus(SPI_TypeDef* SPIx, uint16_t SPI_I2S_FLAG): Trả về giá trị cờ trong thanh ghi. Các cờ thông dụng như: 
*SPI_I2S_FLAG_TXE*: Cờ sẽ lên 1 khi truyền xong data trong buffer

*SPI_I2S_FLAG_RXNE*: Cờ sẽ lên 1 khi nhận xong data 

*SPI_I2S_FLAG_BSY*: Cờ sẽ lên 1 khi đường truyền đang bận

**Hàm truyền data từ Master đến Slave:**

Kéo chân CS xuống 0 xác nhận kết nối với Slave. Kiểm tra đường truyền rảnh sẽ bắt đầu truyền dữ liệu. Chờ thiết bị gửi xong dữ liệu thì kéo chân CS lên 1 kết thúc kết nối với Slave.

```
void SPI_Send1Byte(uint8_t data){
	GPIO_WriteBit(GPIOA, SPI1_NSS, Bit_RESET);
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1, data);
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
	GPIO_WriteBit(GPIOA,SPI1_NSS, Bit_SET);	
}
```
Bắt đầu truyền dữ liệu ở Master
```
uint8_t data[] = {1,3,6,9,75};
int main(){
	RCC_Config();
	GPIO_Config();
	TIM_Config();
	SPI_Config();
	while(1){
		for(i = 0;i<5;i++){
			SPI_Send1Byte(data[i]);
			delay_ms(1000);
		}
	}
}
```
### *Slave*
Định nghĩa, cấp clock cho các ngoại vi, tạo hàm delay, cấu hình GPIO cho các chân SPI1, cấu hình cho SPI1 tương tự như với Master
```
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
```
**Hàm nhận data từ Master đến Slave**

Kiểm tra đường truyền có đang rảnh ko, tiến hành nhận dữ liệu. Sau nhận nhận dữ liệu thì kiểm tra dữ liệu được nhận thành công chưa và trả về dữ liệu vừa được nhận.
```
uint8_t SPI_Receive1Byte(void){
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);  
		temp = (uint8_t)SPI_I2S_ReceiveData(SPI1);
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    return temp;
}
```
Bắt đầu nhận dữ liệu ở Slave
```
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
```
### *Slave*
Định nghĩa các chân truyền SPI và PORT. Cấp clock cho ngoại vi, tạo delay tương tự như Master. 
```
#define SPI_SCK_Pin GPIO_Pin_4
#define SPI_MISO_Pin GPIO_Pin_5
#define SPI_MOSI_Pin GPIO_Pin_6
#define SPI_CS_Pin GPIO_Pin_7
#define SPI_GPIO GPIOA
#define SPI_RCC RCC_APB2Periph_GPIOA

int i = 0;
void RCC_Config(){
	RCC_APB2PeriphClockCmd(SPI_RCC , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
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
	while(TIM_GetCounter(TIM2) < time * 10);
}
```
Cấu hình cho chế độ cho các chân Slave
```
void GPIO_Config(){
	GPIO_InitTypeDef GPIOType;
	
	GPIOType.GPIO_Pin = SPI_SCK_Pin | SPI_MOSI_Pin | SPI_CS_Pin;
	GPIOType.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOType.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	
	GPIO_Init(SPI_GPIO, &GPIOType);
	
	GPIOType.GPIO_Pin = SPI_MISO_Pin;
	GPIOType.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOType.GPIO_Mode = GPIO_Mode_Out_PP;
	
	GPIO_Init(SPI_GPIO, &GPIOType);
}
```
Các chân cấu hình ngược lại so với Master

Tạo hàm nhận dữ liệu từ Master qua Slave. Quy trình của hàm bao gồm:
- Kiểm tra liên tục chân CS = 0 sẽ xác nhận Master muốn kết nối với Slave.
- Kiểm tra đã có xung clock từ Master truyền qua chưa. Nếu có sẽ đọc bit và dịch 1 bit cho bit tiếp theo.
- Kiểm tra lại chân CS = 1 xác nhận ngắt kết nối với Master.
Khởi tạo 1 biến để nhận dữ liệu dataReceive và 1 biến để đọc dữ liệu temp từ MOSI ghi vào dataReceive. Đồng thời kiểm tra liên tục chân CS có được kéo xuống 0 chưa.
```
uint8_t dataReceive = 0x00;	
uint8_t temp = 0x00;
while(GPIO_ReadInputDataBit(SPI_GPIO, SPI_CS_Pin));
```
Sau khi xác nhận CS = 0 sẽ đợi xung clock của Master được gửi đến và xác nhận truyền dữ liệu. Khi có clock được gửi đến ta sẽ đọc tín hiệu ở chân MOSI ghi vào temp, sử dụng dataReceive |= temp kết hợp với dịch bit để ghi từng bit dữ liệu vào dataReceive. Thực hiện 8 lân đến khi kết thúc byte dữ liệu. Sau khi truyền sau sẽ chờ clock kết thúc và chờ ngắt kết nối với Master, hàm trả về byte dữ liệu vừa được gửi.
```
while(!GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin));
	for(i = 0; i < 8; i++){ 
		if(GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin)){
			while (GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin)){
				temp = GPIO_ReadInputDataBit(SPI_GPIO, SPI_MOSI_Pin);
			}
			dataReceive <<= 1;
			dataReceive |= temp;
    		}
		while(!GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin)); //chờ clock kết thúc
	}
	while(!GPIO_ReadInputDataBit(SPI_GPIO, SPI_CS_Pin)); // chờ CS = 1 ngắt kết nối với Master
	return dataReceive;
}
```
Hàm nhận dữ liệu ở Slave hoàn chỉnh:
```
uint8_t SPI_Slave_Receive(void){
	uint8_t dataReceive = 0x00;	
	uint8_t temp = 0x00;
	while(GPIO_ReadInputDataBit(SPI_GPIO, SPI_CS_Pin));
	while(!GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin));
	for(i = 0; i < 8; i++){ 
		if(GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin)){
			while (GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin)){
				temp = GPIO_ReadInputDataBit(SPI_GPIO, SPI_MOSI_Pin);
			}
			dataReceive <<= 1;
			dataReceive |= temp;
    		}
		while(!GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin));
	}
	while(!GPIO_ReadInputDataBit(SPI_GPIO, SPI_CS_Pin));
	return dataReceive;
}
```
Bắt đầu nhận dữ liệu ở Slave
```
uint8_t data;
int main(){
	RCC_Config();
	GPIO_Config();
	TIM_Config();
	while(1){
		data = SPI_Slave_Receive();
	}
}
```