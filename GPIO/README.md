
# GPIO

## Cấu hình và sử dụng cho các ngoại vi
Để sử dụng được 1 ngoại vi trên vi điều khiển ta phải thực hiện 3 bước để cấu hình các chức năng ngoại vi ta sẽ sử dụng. 

- Cấp clock cho ngoại vi theo đường bus được cấp của vi điều khiển 
![1](https://github.com/user-attachments/assets/04dbf1e1-67df-4b8e-923f-4dad3e0adacb)

- Cấu hình cho ngoại vi tuỳ theo ngoại vi ta sử dụng.
- Sử dụng ngoại vi theo nhu cầu.



## Khái niệm GPIO
GPIO là một cổng trên vi điều khiển có thể được cấu hình để hoạt động như một đầu vào hoặc đầu ra kỹ thuật số. Điều này có nghĩa là bạn có thể đọc hoặc ghi giá trị nhị phân (0 hoặc 1) vào cổng này. Mỗi chân GPIO trên vi điều khiển có thể được cấu hình làm đầu vào hoặc đầu ra.



## Cấp clock cho GPIO
Module RCC cung cấp các hàm để cấu hình xung clock.
```
RCC_APB2PeriphClockCmd(uint32_t RCC_APB2Periph, FunctionalState NewState)
```
RCC_APB2Periph: Chọn ngoại vi ta cần sử dụng (GPIO).

NewState: ENABLE or DISABLE.
## Cấu hình cho GPIO
Các tham số dùng để cấu hình cho GPIO được tổ chức trong struct GPIO_InitTypeDef.
```
typedef struct
{
  uint16_t GPIO_Pin;             /*!< Specifies the GPIO pins to be configured.
                                      This parameter can be any value of @ref GPIO_pins_define */

  GPIOSpeed_TypeDef GPIO_Speed;  /*!< Specifies the speed for the selected pins.
                                      This parameter can be a value of @ref GPIOSpeed_TypeDef */

  GPIOMode_TypeDef GPIO_Mode;    /*!< Specifies the operating mode for the selected pins.
                                      This parameter can be a value of @ref GPIOMode_TypeDef */
}GPIO_InitTypeDef;
```
Trước tiên khai báo 1 biến của struct GPIO_InitTypeDef và cấu hình cho các chức năng của GPIO, cuối cùng là chọn GPIO sẽ thực thi cấu hình được tạo.

Ví dụ: 
```
void GPIO_Config(){
	GPIO_InitTypeDef GPIOType;

	GPIOType.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIOType.GPIO_Pin = GPIO_Pin_13| GPIO_Pin_5 |GPIO_Pin_6 |GPIO_Pin_7| GPIO_Pin_8;
	GPIOType.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOC, &GPIOType);
}
```
Mỗi chức năng có nhiều lựa chọn tuỳ theo nhu cầu sử dụng.

**GPIO_Mode**
```
typedef enum
{ GPIO_Mode_AIN = 0x0,
  GPIO_Mode_IN_FLOATING = 0x04,
  GPIO_Mode_IPD = 0x28,
  GPIO_Mode_IPU = 0x48,
  GPIO_Mode_Out_OD = 0x14,
  GPIO_Mode_Out_PP = 0x10,
  GPIO_Mode_AF_OD = 0x1C,
  GPIO_Mode_AF_PP = 0x18
}GPIOMode_TypeDef;
```
**GPIO_Pin**
```
#define GPIO_Pin_0                 ((uint16_t)0x0001)  /*!< Pin 0 selected */
#define GPIO_Pin_1                 ((uint16_t)0x0002)  /*!< Pin 1 selected */
#define GPIO_Pin_2                 ((uint16_t)0x0004)  /*!< Pin 2 selected */
#define GPIO_Pin_3                 ((uint16_t)0x0008)  /*!< Pin 3 selected */
#define GPIO_Pin_4                 ((uint16_t)0x0010)  /*!< Pin 4 selected */
#define GPIO_Pin_5                 ((uint16_t)0x0020)  /*!< Pin 5 selected */
#define GPIO_Pin_6                 ((uint16_t)0x0040)  /*!< Pin 6 selected */
#define GPIO_Pin_7                 ((uint16_t)0x0080)  /*!< Pin 7 selected */
#define GPIO_Pin_8                 ((uint16_t)0x0100)  /*!< Pin 8 selected */
#define GPIO_Pin_9                 ((uint16_t)0x0200)  /*!< Pin 9 selected */
#define GPIO_Pin_10                ((uint16_t)0x0400)  /*!< Pin 10 selected */
#define GPIO_Pin_11                ((uint16_t)0x0800)  /*!< Pin 11 selected */
#define GPIO_Pin_12                ((uint16_t)0x1000)  /*!< Pin 12 selected */
#define GPIO_Pin_13                ((uint16_t)0x2000)  /*!< Pin 13 selected */
#define GPIO_Pin_14                ((uint16_t)0x4000)  /*!< Pin 14 selected */
#define GPIO_Pin_15                ((uint16_t)0x8000)  /*!< Pin 15 selected */
#define GPIO_Pin_All               ((uint16_t)0xFFFF)  /*!< All pins selected */
```
**GPIO_Speed**
```
typedef enum
{ 
  GPIO_Speed_10MHz = 1,
  GPIO_Speed_2MHz, 
  GPIO_Speed_50MHz
}GPIOSpeed_TypeDef;
```

## Sử dụng GPIO
Có thể xem danh sách các hàm có thể sử dụng của GPIO thông qua Function (stm32f10x_gpio.c).
![2](https://github.com/user-attachments/assets/b8fb16d4-cfa2-4577-93b7-c68903303a86)
