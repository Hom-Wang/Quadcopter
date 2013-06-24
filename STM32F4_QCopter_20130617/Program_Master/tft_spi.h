#ifndef __TFT_SPI_H__
#define __TFT_SPI_H__


//與TFT芯片初始化連接引腳	   
#define CLR_TDIN()		GPIO_ResetBits(GPIOD, GPIO_Pin_3)
#define SET_TDIN()		GPIO_SetBits(GPIOD, GPIO_Pin_3)
#define CLR_TCLK()		GPIO_ResetBits(GPIOD, GPIO_Pin_6)
#define SET_TCLK()		GPIO_SetBits(GPIOD, GPIO_Pin_6)
#define CLR_TCS()     {LCD_WR_REG(0xBA);LCD_WR_DATA(0x00);}
#define SET_TCS()     {LCD_WR_REG(0xBA);LCD_WR_DATA(0x04);}

#define TDOUT_IN()  	(GPIOA->IDR&GPIO_Pin_0)   //GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)

void TFT_SPI_Write_Byte(u8 num);
void SPI_SET_CMD(u8 CMD);
void SPI_SET_DATA(u8 DATA);
void TFT_SPI_Init(void);
void TFT_Init_Code(void);

#endif


