#include <stdlib.h>
#include "tft_spi.h" 


void TFT_SPI_Write_Byte(u8 num)    
{  
	u8 count=0;  
	
	for(count=0;count<8;count++)  
	{ 
		CLR_TCLK();//上升沿有效	 
		if(num&0x80)SET_TDIN(); 
		else CLR_TDIN();   
		num<<=1;    
		SET_TCLK();   		
	} 			    
}


void SPI_SET_CMD(u8 CMD)	  
{ 
	CLR_TCS(); 
	CLR_TCLK(); 
	CLR_TDIN();
	SET_TCLK();         //給1個時鐘	
	TFT_SPI_Write_Byte(CMD);
	SET_TCS();         
}
  
void SPI_SET_DATA(u8 DATA)	  
{ 	 
	CLR_TCS(); 
	CLR_TCLK(); 
	SET_TDIN();
	SET_TCLK();         //給1個時鐘	
	TFT_SPI_Write_Byte(DATA);
	SET_TCS();   
}


void TFT_SPI_Init(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
	
	LCD_Initializtion();
	
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOD, ENABLE);
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_6;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
  	SET_TCS();
		GPIO_SetBits(GPIOD, GPIO_Pin_6);                                   //TCLK高
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;                          //DOUT PA0 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
}


void TFT_Init_Code(void)
{
	u8 id=0;
	
	TFT_SPI_Init();
	delay_nms(6); 
	
	//Set_EXTC
	SPI_SET_CMD(0xB9);
	SPI_SET_DATA(0xFF);
	SPI_SET_DATA(0x83);
	SPI_SET_DATA(0x63);
	
	//Set_POWER
	SPI_SET_CMD(0xB1);
	SPI_SET_DATA(0x81);
	SPI_SET_DATA(0x30);
	SPI_SET_DATA(0x07);
	SPI_SET_DATA(0x34);
	SPI_SET_DATA(0x02);
	SPI_SET_DATA(0x13);
	SPI_SET_DATA(0x11);
	SPI_SET_DATA(0x00);
	SPI_SET_DATA(0x3A);
	SPI_SET_DATA(0x42);
	SPI_SET_DATA(0x3F);
	SPI_SET_DATA(0x3F);

	//Sleep Out
	SPI_SET_CMD(0x11);
	delay_nms(120);
	
	//Set COLMOD
	SPI_SET_CMD(0x3A);
	SPI_SET_DATA(0x70);
	
	//Set_RGBIF
	SPI_SET_CMD(0xB3);
	SPI_SET_DATA(0x01);
	
	//Set_CYC
	SPI_SET_CMD(0xB4);
	SPI_SET_DATA(0x08);
	SPI_SET_DATA(0x12);
	SPI_SET_DATA(0x72);
	SPI_SET_DATA(0x12);
	SPI_SET_DATA(0x06);
	SPI_SET_DATA(0x03);
	SPI_SET_DATA(0x54);
	SPI_SET_DATA(0x03);
	SPI_SET_DATA(0x4E);
	SPI_SET_DATA(0x00);
	SPI_SET_DATA(0x00);

	//Set_PTBA
	SPI_SET_CMD(0xBF);
	SPI_SET_DATA(0x00);
	SPI_SET_DATA(0x10);
	
	//Set_VCOM
	SPI_SET_CMD(0xB6);
	SPI_SET_DATA(0x0E);
	
	//Set_PANEL
	SPI_SET_CMD(0xCC);
	SPI_SET_DATA(0x03);
	delay_nms(5);

	//Set Gamma 2.2
	SPI_SET_CMD(0xE0);
	SPI_SET_DATA(0x00);
	SPI_SET_DATA(0x80);
	SPI_SET_DATA(0x00);
	SPI_SET_DATA(0x8C);
	SPI_SET_DATA(0x13);
	SPI_SET_DATA(0xA7);
	SPI_SET_DATA(0x05);
	SPI_SET_DATA(0x0D);
	SPI_SET_DATA(0x10);
	SPI_SET_DATA(0xD4);
	SPI_SET_DATA(0x17);
	SPI_SET_DATA(0x55);
	SPI_SET_DATA(0x16);
	SPI_SET_DATA(0x07);
	SPI_SET_DATA(0x03);
	SPI_SET_DATA(0x00);
	SPI_SET_DATA(0x80);
	SPI_SET_DATA(0x00);
	SPI_SET_DATA(0x8C);
	SPI_SET_DATA(0x13);
	SPI_SET_DATA(0xA7);
	SPI_SET_DATA(0x05);
	SPI_SET_DATA(0x0D);
	SPI_SET_DATA(0x10);
	SPI_SET_DATA(0xD4);
	SPI_SET_DATA(0x17);
	SPI_SET_DATA(0x55);
	SPI_SET_DATA(0x16);
	SPI_SET_DATA(0x07);
	SPI_SET_DATA(0x03);
	delay_nms(5);

	//Display On
	SPI_SET_CMD(0x21);    //一定要加，開始沒有加顯示反色
	SPI_SET_CMD(0x29);
	
	SPI_SET_CMD(0x36);    //改變R B 色，字符正反
	SPI_SET_DATA(0x02);    
}

