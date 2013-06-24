#include "lcd.h"


/**************************************************************************************************************
 * 函數名：LCD_WriteReg()
 * 輸入  ：uint8_t LCD_Reg 寄存器地址, uint16_t LCD_RegValue 寄存器的值
 * 輸出  ：void
 * 描述  ：寫LCD寄存器函數
 * 調用  ：外部調用        
 *****************************************************************************************************************/
void LCD_WR_REG(uint16_t LCD_Reg)
{
  /*寫寄存器地址*/
  LCD_REG = LCD_Reg;
}


void LCD_WR_DATA(uint16_t LCD_Data)
{
  /*寫寄存器地址*/
  LCD_RAM = LCD_Data;
}
/**************************************************************************************************************
 * 函數名：LCD_ReadReg()
 * 輸入  ：uint8_t LCD_Reg 需要讀取的寄存器地址
 * 輸出  ：uint16_t 寄存器的值
 * 描述  ：讀取LCD寄存器的值
 * 調用  ：外部調用        
 *****************************************************************************************************************/
uint16_t LCD_RD_DATA(void)
{
  return (LCD_RAM);
}


void LCD_Clear(uint16_t Color)
{
  uint32_t i;
	
	LCD_WR_REG(0x36);
	LCD_WR_DATA(0x0);
	LCD_WR_REG(0x2A);	
	LCD_WR_DATA(0);	    
	LCD_WR_DATA(0);
	LCD_WR_DATA((LCD_XMAX>>8)&0X00FF);	    
	LCD_WR_DATA(LCD_XMAX&0X00FF);
 	LCD_WR_REG(0x2b);	
	LCD_WR_DATA(0);	    
	LCD_WR_DATA(0);
	LCD_WR_DATA((LCD_YMAX>>8)&0X00FF);  
	LCD_WR_DATA(LCD_YMAX&0X00FF);
	LCD_WR_REG(0x2c);

	for(i=0;i<384000;i++)
	{    
		LCD_WR_DATA(Color);
	}
}


void LCD_SetCoordinates(uint16_t x, uint16_t y )
{
	LCD_WR_REG(0x36);
	LCD_WR_DATA(0);
	LCD_WR_REG(0x2a);	
	LCD_WR_DATA((x>>8)&0xff);	    
	LCD_WR_DATA(x&0xff);
	LCD_WR_DATA((LCD_XMAX>>8)&0xff);	    
	LCD_WR_DATA(LCD_XMAX&0xff);
  LCD_WR_REG(0x2b);	
	LCD_WR_DATA((y>>8)&0xff);  
	LCD_WR_DATA(y&0xff);
	LCD_WR_DATA((LCD_YMAX>>8)&0xff);  
	LCD_WR_DATA(LCD_YMAX&0xff);
	LCD_WR_REG(0x2c);
}


static void LCD_SetCursor( uint16_t x, uint16_t y )
{				   
	if(x > LCD_XMAX) return ;
	if(y > LCD_YMAX) return ;
	 
	LCD_WR_REG(0x2A);	
	LCD_WR_DATA((x>>8)&0xff);	    
	LCD_WR_DATA(x&0xff);
	LCD_WR_DATA((LCD_XMAX>>8)&0xff);	    
	LCD_WR_DATA(LCD_XMAX&0xff);
  LCD_WR_REG(0x002b);	
	LCD_WR_DATA((y>>8)&0xff);  
	LCD_WR_DATA(y&0x00ff);
	LCD_WR_DATA((LCD_YMAX>>8)&0xff);  
	LCD_WR_DATA(LCD_YMAX&0xff);	
}


uint16_t LCD_GetPoint(uint16_t x,uint16_t y)
{
	LCD_WR_REG(0x002A);	
	LCD_WR_DATA((x>>8)&0x00ff);	    
	LCD_WR_DATA(x&0x00ff);
	LCD_WR_DATA((LCD_XMAX>>8)&0X00FF);	    
	LCD_WR_DATA(LCD_XMAX&0X00FF);
  LCD_WR_REG(0x002b);	
	LCD_WR_DATA((y>>8)&0x00ff);  
	LCD_WR_DATA(y&0x00ff);
	LCD_WR_DATA((LCD_YMAX>>8)&0X00FF);  
	LCD_WR_DATA(LCD_YMAX&0X00FF);
	LCD_WR_REG(0x002e);
	return LCD_RD_DATA();	
}


void LCD_SetPoint(uint16_t x, uint16_t y, uint16_t color)
{
	LCD_WR_REG(0x002A);	
	LCD_WR_DATA((x>>8)&0x00ff);	    
	LCD_WR_DATA(x&0x00ff);
	LCD_WR_DATA((x>>8)&0x00ff);	    
	LCD_WR_DATA(x&0x00ff);
	
  LCD_WR_REG(0x002b);	
	LCD_WR_DATA((y>>8)&0x00ff);  
	LCD_WR_DATA(y&0x00ff);
	LCD_WR_DATA((y>>8)&0x00ff);  
	LCD_WR_DATA(y&0x00ff);
	LCD_WR_REG(0x002c);
	LCD_WR_DATA(color);
}


void LCD_DrawLine( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color )
{
  short dx,dy;
  short temp;
		
  if( x0 > x1 )
  {
    temp = x1;
    x1 = x0;
    x0 = temp;   
  }
  if( y0 > y1 )
  {
    temp = y1;
    y1 = y0;
    y0 = temp;   
  }

  dx = x1-x0;
  dy = y1-y0;

  if( dx == 0 )
  {
    do
    { 
      LCD_SetPoint(x0, y0, color);
      y0++;
    }while( y1 >= y0 ); 
    return; 
  }
  if( dy == 0 )
  {
    do
    {
      LCD_SetPoint(x0, y0, color);
      x0++;
    }
    while( x1 >= x0 ); 
		return;
  }

	/* Bresenham's line algorithm  */
  if( dx > dy )
  {
    temp = 2 * dy - dx;
    while( x0 != x1 )
    {
	    LCD_SetPoint(x0,y0,color);
	    x0++;
	    if( temp > 0 )
	    {
	      y0++;
	      temp += 2 * dy - 2 * dx; 
	 	  }
      else         
      {
			  temp += 2 * dy;
			}       
    }
    LCD_SetPoint(x0,y0,color);
  }  
  else
  {
    temp = 2 * dx - dy;
    while( y0 != y1 )
    {
	 	  LCD_SetPoint(x0,y0,color);     
      y0++;                 
      if( temp > 0 )           
      {
        x0++;               
        temp+=2*dy-2*dx; 
      }
      else
			{
        temp += 2 * dy;
			}
    } 
    LCD_SetPoint(x0,y0,color);
	}
} 


void PutChar( uint16_t Xpos, uint16_t Ypos, uint8_t ASCI, uint16_t charColor, uint16_t bkColor )
{
	uint16_t i, j;
    uint8_t buffer[16], tmp_char;
    //GetASCIICode(buffer,ASCI);
    for( i=0; i<16; i++ )
    {
        tmp_char = buffer[i];
        for( j=0; j<8; j++ )
        {
            if( (tmp_char >> 7 - j) & 0x01 == 0x01 )
            {
                LCD_SetPoint( Xpos + j, Ypos + i, charColor );
            }
            else
            {
                LCD_SetPoint( Xpos + j, Ypos + i, bkColor );
            }
        }
    }
}


void GUI_Text(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint16_t Color, uint16_t bkColor)
{
    uint8_t TempChar;
    do
    {
        TempChar = *str++;  
        PutChar( Xpos, Ypos, TempChar, Color, bkColor );    
        if( Xpos < MAX_X - 8 )
        {
            Xpos += 8;
        } 
        else if ( Ypos < MAX_Y - 16 )
        {
            Xpos = 0;
            Ypos += 16;
        }   
        else
        {
            Xpos = 0;
            Ypos = 0;
        }    
    }
    while ( *str != 0 );
}


/*******************************************************************************
* Function Name  : LCD_Initializtion
* Description    : SSD1963 Resets
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_Initializtion(void)
{
	uint16_t Check,Para[8];
	
	LCD_FSMC_Config();
  delay_nms(5);
	while(1)
	{
		Check = 1;
		LCD_WR_REG(0xE2);		  //Set PLL with OSC = 10MHz (hardware),	Multiplier N = 35, 250MHz < VCO < 800MHz = OSC*(N+1), VCO = 360MHz
		LCD_WR_DATA(0x23);		//參數1 
		LCD_WR_DATA(0x02);		//參數2 Divider M = 2, PLL = 360/(M+1) = 120MHz
		LCD_WR_DATA(0x54);		//參數3 Validate M and N values
		
		LCD_WR_REG(0xE0);		  // Start PLL command
		LCD_WR_DATA(0x01);	  // enable PLL
		delay_nms(2);
		LCD_WR_REG(0xE0);		  // Start PLL command again
		LCD_WR_DATA(0x03);	
		delay_nms(10);
		LCD_WR_REG(0xe4);
		Para[0] = LCD_RD_DATA();
		if(Para[0] != 0x04) Check = 0;
		LCD_WR_REG(0xe3);
		Para[0] = LCD_RD_DATA();
		Para[1] = LCD_RD_DATA();
		Para[2] = LCD_RD_DATA();
		printf("e30=%x\n",Para[0]);
		printf("e31=%x\n",Para[1]);
		printf("e32=%x\n",Para[2]);
		if(Para[0] != 0x23) Check = 0;
		if(Para[1] != 0x02) Check = 0;
		if(Para[2] != 0x04) Check = 0;
		if(Check) break;
 	}
 	LCD_WR_REG(0x01);		//軟復位
 	delay_nms(10);
 	
	//Set LSHIFT freq, i.e. the DCLK with PLL freq 120MHz set previously
	//Typical DCLK for TY700TFT800480 is 33.3MHz(datasheet), experiment shows 30MHz gives a stable result
	//30MHz = 120MHz*(LCDC_FPR+1)/2^20
	//LCDC_FPR = 262143 (0x3FFFF)
	//Time per line = (DISP_HOR_RESOLUTION+DISP_HOR_PULSE_WIDTH+DISP_HOR_BACK_PORCH+DISP_HOR_FRONT_PORCH)/30 us = 1056/30 = 35.2us
	while(1)
	{
		Check = 1;
		LCD_WR_REG(0xE6);
		LCD_WR_DATA(0x03);
		LCD_WR_DATA(0xff);
		LCD_WR_DATA(0xff);
		
		LCD_WR_REG(0xE7);
		Para[0] = LCD_RD_DATA();
		Para[1] = LCD_RD_DATA();
		Para[2] = LCD_RD_DATA();
		if(Para[0] != 0x03) Check = 0;
		if(Para[1] != 0xff) Check = 0;
		if(Para[2] != 0xff) Check = 0;
		if(Check) break;
	}

	//Set LCD mode, varies from individual manufacturer
	while(1)
	{
		Check = 1;
		LCD_WR_REG(0xB0);
		LCD_WR_DATA(0x20);//0x10				            // set 24-bit 
		LCD_WR_DATA(0x80);				                  // set TTL mode
		LCD_WR_DATA((DISP_HOR_RESOLUTION-1)>>8); 		// Set LCD panel size(DISP_HOR_RESOLUTION = 800) H
		LCD_WR_DATA(DISP_HOR_RESOLUTION-1);		      // Set LCD panel size(DISP_HOR_RESOLUTION = 800) L
		LCD_WR_DATA((DISP_VER_RESOLUTION-1)>>8);		// Set LCD panel size(DISP_VER_RESOLUTION = 480) H
		LCD_WR_DATA(DISP_VER_RESOLUTION-1);		      // Set LCD panel size(DISP_VER_RESOLUTION = 480) L
		LCD_WR_DATA(0x00);//0x00				            // RGB sequence	

		LCD_WR_REG(0xB1);
		Para[0] = LCD_RD_DATA();
		Para[1] = LCD_RD_DATA();
		Para[2] = LCD_RD_DATA();
		Para[3] = LCD_RD_DATA();
		Para[4] = LCD_RD_DATA();
		Para[5] = LCD_RD_DATA();
		Para[6] = LCD_RD_DATA();
		if(Para[0] != 0x20) Check = 0;
		if(Para[1] != 0x80) Check = 0;
		if(Para[2] != ((DISP_HOR_RESOLUTION-1)>>8)) Check = 0;
		if(Para[3] != ((DISP_HOR_RESOLUTION-1)&0xff)) Check = 0;
		if(Para[4] != ((DISP_VER_RESOLUTION-1)>>8)) Check = 0;
		if(Para[5] != ((DISP_VER_RESOLUTION-1)&0xff)) Check = 0;
		if(Para[6] != 0x00) Check = 0;
		if(Check) break;
		
	}
	//Set horizontal period
	while(1)
	{
		LCD_WR_REG(0xB4);
		LCD_WR_DATA((HT-1)>>8);
		LCD_WR_DATA(HT-1);
		LCD_WR_DATA((HPS-1)>>8);
		LCD_WR_DATA(HPS-1);
		LCD_WR_DATA(DISP_HOR_PULSE_WIDTH-1);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		
		LCD_WR_REG(0xB5);
		Para[0] = LCD_RD_DATA();
		Para[1] = LCD_RD_DATA();
		Para[2] = LCD_RD_DATA();
		Para[3] = LCD_RD_DATA();
		Para[4] = LCD_RD_DATA();
		Para[5] = LCD_RD_DATA();
		Para[6] = LCD_RD_DATA();
		Para[7] = LCD_RD_DATA();
		if(Para[0] != ((HT-1)>>8)) Check = 0;
		if(Para[1] != ((HT-1)&0xff)) Check = 0;
		if(Para[2] != ((HPS-1)>>8)) Check = 0;
		if(Para[3] != ((HPS-1)&0xff)) Check = 0;
		if(Para[4] != DISP_HOR_PULSE_WIDTH-1) Check = 0;
		if(Para[5] != 0x00) Check = 0;
		if(Para[6] != 0x00) Check = 0;
		if(Para[7] != 0x00) Check = 0;
		if(Check) break;		
	}	
		
	//Set vertical period
	while(1)
	{
		LCD_WR_REG(0xB6);
		LCD_WR_DATA((VT-1)>>8);
		LCD_WR_DATA(VT-1);
		LCD_WR_DATA((VSP-1)>>8);
		LCD_WR_DATA(VSP-1);
		LCD_WR_DATA(DISP_VER_PULSE_WIDTH-1);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		
		LCD_WR_REG(0xB7);
		Para[0] = LCD_RD_DATA();
		Para[1] = LCD_RD_DATA();
		Para[2] = LCD_RD_DATA();
		Para[3] = LCD_RD_DATA();
		Para[4] = LCD_RD_DATA();
		Para[5] = LCD_RD_DATA();
		Para[6] = LCD_RD_DATA();
		if(Para[0] != ((VT-1)>>8)) Check = 0;
		if(Para[1] != ((VT-1)&0xff)) Check = 0;
		if(Para[2] != ((VSP-1)>>8)) Check = 0;
		if(Para[3] != ((VSP-1)&0xff)) Check = 0;
		if(Para[4] != DISP_VER_PULSE_WIDTH-1) Check = 0;
		if(Para[5] != 0x00) Check = 0;
		if(Para[6] != 0x00) Check = 0;
		if(Check) break;
	}
	while(1)
	{
		Check = 1;
		LCD_WR_REG(0xf0);	//設置SSD1963同CPU接口為16bit Set pixel data interface 
		LCD_WR_DATA(0x03);	//16-bit(565 format) data for 16bpp PIC32MX only
		
		LCD_WR_REG(0xf1);		//Set pixel format
		Para[0] = LCD_RD_DATA();
		if(Para[0] != 0x03) Check = 0;
		if(Check) break;

	}
	LCD_Clear(White);
	LCD_WR_REG(0x29);	// Turn on display; show the image on display
	        
	LCD_WR_REG(0xB8);	//設置GPIO配置
	LCD_WR_DATA(0x0F);	//GPIO is controlled by host; 4個IO設置成輸出
	LCD_WR_DATA(0x01);	//GPIO0 使用正常的IO功能
}

