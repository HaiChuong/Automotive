
# Interrupt - Timer

Ngắt là 1 cơ chế cho phép vi điều khiển tạm dừng chương trình chính để xử lý 1 sự kiện hoặc yêu cầu.

## Các ngắt thông dụng
Có 4 loại ngắt thông dụng được sử dụng: Reset, Ngắt ngoài, Timer, Ngắt truyền thông

Mỗi ngắt sẽ có 1 trình phục vụ ngắt riêng. Trình phục vụ ngắt (Interrupt Service Routine - ISR) là một đoạn chương trình được thực hiện khi ngắt xảy ra.

Địa chỉ trong bộ nhớ của ISR được gọi là vector ngắt

PC (Program counter) là thanh ghi luôn chỉ đến lệnh tiếp theo trong chương trình.



### Ngắt ngoài
Xảy ra khi có thay đổi điện áp trên các chân GPIO được cấu hình làm ngõ vào ngắt. Có 4 dạng:
- LOW: kích hoạt ngắt liên tục khi chân ở mức thấp.
- HIGH: Kích hoạt liên tục khi chân ở mức cao.
- RISING: Kích hoạt khi trạng thái trên chân chuyển từ thấp lên cao.
- FALLING: Kích hoạt khi trạng thái trên chân chuyển từ cao xuống thấp.
### Ngắt Timer
Sử dụng các timer của vi điều khiển để điều khiển ngắt. Ngắt sẽ xảy ra khi timer tràn tuỳ theo chương trình ta lập trình cho timer. Sau mỗi lần tràn, cần phải reset giá trị thanh ghi để có thể tạo ngắt tiếp theo.
### Ngắt truyền thông
Xảy ra khi có sự kiện truyền/nhận dữ liệu giữa MCU và các thiết bị khác, thường sử dụng cho các giao thức như UART, SPI, I2C để đảm bảo việc truyền/nhận được chính xác.
### Độ ưu tiên ngắt
Các ngắt có độ ưu tiên khác nhau, quyết định ngắt nào được thực thi khi nhiều ngắt xảy ra đồng thời.

Trên STM32, ngắt có số ưu tiên càng thấp thì có quyền càng cao.

Stack Pointer là thanh ghi trỏ tới đỉnh của vùng stack chứa các địa chỉ trả về của các hàm

## Timer
Timer là 1 mạch digital logic có vai trò đếm mỗi chu kỳ clock (đếm lên hoặc đếm xuống).

Timer còn có thể hoạt động ở chế độ nhận xung clock từ các tín hiệu ngoài. Ngoài ra còn các chế độ khác như PWM, định thời...

STM32F103 có 7 timer
![3](https://github.com/user-attachments/assets/25e81d80-1223-4627-b8a6-13278b29be99)
Cấu hình cho timer tuỳ thuộc vào sử dụng timer nào và nhu cầu sử dụng.
```
typedef struct
{
  uint16_t TIM_Prescaler;         /*!< Specifies the prescaler value used to divide the TIM clock.
                                       This parameter can be a number between 0x0000 and 0xFFFF */

  uint16_t TIM_CounterMode;       /*!< Specifies the counter mode.
                                       This parameter can be a value of @ref TIM_Counter_Mode */

  uint16_t TIM_Period;            /*!< Specifies the period value to be loaded into the active
                                       Auto-Reload Register at the next update event.
                                       This parameter must be a number between 0x0000 and 0xFFFF.  */ 

  uint16_t TIM_ClockDivision;     /*!< Specifies the clock division.
                                      This parameter can be a value of @ref TIM_Clock_Division_CKD */

} TIM_TimeBaseInitTypeDef;
```
Ví dụ tạo hàm delay 1ms bằng TIM2 (vì TIM_Prescaler chỉ nhận giá trị đến 65536 nên chỉ có thể đặt 7200 và nhân 10 bằng delay_ms)
```
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
```
