/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_usart.h"
#include "module_ssd1963.h"
#include "module_fontlib.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
TFT_TypeDef TFT;
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : SSD1963_Config
**功能 : SSD1963 設定 & 配置
**輸入 : None
**輸出 : None
**使用 : SSD1963_Config();
**=====================================================================================================*/
/*=====================================================================================================*/
void SSD1963_Config( void )
{
	GPIO_InitTypeDef GPIO_InitStruct;
	FSMC_NORSRAMInitTypeDef FSMC_InitStruct;
	FSMC_NORSRAMTimingInitTypeDef FSMC_TimingInitStruct;

	/* FSMC Clk Init *************************************************************/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);

	/* FSMC AF *************************************************************/
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource0,  GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource1,  GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource4,  GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource5,  GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource7,  GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8,  GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9,  GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource7,  GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource8,  GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9,  GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource10, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource12, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource15, GPIO_AF_FSMC);

  /* FSMC_D0  PD14 */	/* FSMC_D1  PD15 */	/* FSMC_D2  PD0  */	/* FSMC_D3  PD1  */
	/* FSMC_D4  PE7  */	/* FSMC_D5  PE8  */	/* FSMC_D6  PE9  */	/* FSMC_D7  PE10 */
	/* FSMC_D8  PE11 */	/* FSMC_D9  PE12 */	/* FSMC_D10 PE13 */	/* FSMC_D11 PE14 */
	/* FSMC_D12 PE15 */	/* FSMC_D13 PD8  */	/* FSMC_D14 PD9  */	/* FSMC_D15 PD10 */
	/* FSMC_RD  PD4  */	/* FSMC_WR  PD5  */	/* FSMC_CS  PD7  */	/* FSMC_RS  PD11 */
  GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_4  | GPIO_Pin_5  | GPIO_Pin_7  |
															GPIO_Pin_8  |	GPIO_Pin_9  | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_14 |
															GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOD, &GPIO_InitStruct);
  GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_7  | GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_10 | GPIO_Pin_11 |
															GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOE, &GPIO_InitStruct);

	/* FSMC_LIG PD12 */	/* FSMC_RST PD13 */
	GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStruct);

	FSMC_TimingInitStruct.FSMC_AddressSetupTime = 0x02;
	FSMC_TimingInitStruct.FSMC_AddressHoldTime = 0x00;
	FSMC_TimingInitStruct.FSMC_DataSetupTime = 0x05;
	FSMC_TimingInitStruct.FSMC_BusTurnAroundDuration = 0x00;
	FSMC_TimingInitStruct.FSMC_CLKDivision = 0x00;
	FSMC_TimingInitStruct.FSMC_DataLatency = 0x00;
	FSMC_TimingInitStruct.FSMC_AccessMode = FSMC_AccessMode_B;

  FSMC_InitStruct.FSMC_Bank = FSMC_Bank1_NORSRAM1;
  FSMC_InitStruct.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
  FSMC_InitStruct.FSMC_MemoryType = FSMC_MemoryType_NOR;
  FSMC_InitStruct.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
  FSMC_InitStruct.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
  FSMC_InitStruct.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
  FSMC_InitStruct.FSMC_WrapMode = FSMC_WrapMode_Disable;
  FSMC_InitStruct.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
  FSMC_InitStruct.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
  FSMC_InitStruct.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
  FSMC_InitStruct.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
  FSMC_InitStruct.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_InitStruct.FSMC_ReadWriteTimingStruct = &FSMC_TimingInitStruct;
	FSMC_InitStruct.FSMC_WriteTimingStruct = &FSMC_TimingInitStruct;

  FSMC_NORSRAMInit(&FSMC_InitStruct);
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);

	TFT_LIGHT = 1;
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : TFT_WriteData
**功能 : Write Data
**輸入 : WriteData
**輸出 : None
**使用 : 
**=====================================================================================================*/
/*=====================================================================================================*/
static void TFT_WriteData( u32 WriteData )
{
	TFT_RAM = WriteData;
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : TFT_WriteCom
**功能 : Write Command or Address
**輸入 : WriteCom
**輸出 : None
**使用 : 
**=====================================================================================================*/
/*=====================================================================================================*/
static void TFT_WriteCom( u32 WriteCom )
{
	TFT_REG = WriteCom;
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : TFT_WriteReg
**功能 : Write Register
**輸入 : WriteCom, WriteData
**輸出 : None
**使用 : 
**=====================================================================================================*/
/*=====================================================================================================*/
// static void TFT_WriteReg( u32 WriteCom, u32 WriteData )
// {
// 	TFT_REG = WriteCom;
// 	TFT_RAM = WriteData;
// }
/*=====================================================================================================*/
/*=====================================================================================================*/
static void TFT_WriteColor( u32 Color )
{
	u8 Color_R, Color_G, Color_B;

	Color_R = (u8)(Color >> 16);
	Color_G = (u8)(Color >> 8);
	Color_B = (u8)Color;

	TFT_WriteData((Color_R << 8) | (Color_G));
	TFT_WriteData((Color_B << 8) | (Color_R));
	TFT_WriteData((Color_G << 8) | (Color_B));
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : SSD1963_CoeffiInit
**功能 : Set SSD1963 Coefficient
**輸入 : None
**輸出 : None
**使用 : SSD1963_CoeffiInit()
**=====================================================================================================*/
/*=====================================================================================================*/
static void SSD1963_CoeffiInit( void )
{
	TFT.HPW = 1;
	TFT.VPW = 10;
	TFT.HDP = TFT_L-1;
	TFT.VDP = TFT_W-1;
	TFT.HT  = 531;
	TFT.HPS = 43;
	TFT.LPS = 8;
	TFT.VT  = 288;
	TFT.VPS = 12;
	TFT.FPS = 4;
	TFT.Contrast = 0x40;
	TFT.Brightness = 0x80;
	TFT.Saturation = 0x40;
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : SSD1963_Init
**功能 : SSD1963 Init
**輸入 : None
**輸出 : None
**使用 : SSD1963_Init()
**=====================================================================================================*/
/*=====================================================================================================*/
void SSD1963_Init( void )
{
	SSD1963_CoeffiInit();

	TFT_RESET();

	TFT_WriteCom(Com_SetPageAddress);
	TFT_WriteData(0x0000);
	Delay_100us(1);

	TFT_WriteCom(Com_SetPllMn);								// PLL multiplier, Use 10M Osc, set PLL clock to 120M
	TFT_WriteData(0x0023);										// M = 59
	TFT_WriteData(0x0002);										// N = 2
	TFT_WriteData(0x0004);										// Use PLL, PLL = OSC*(M+1)/(N+1), 250MHz<Osc*(M+1)<800MHz

	TFT_WriteCom(Com_SetPll);									// Start the PLL
	TFT_WriteData(0x0001);										// Enable PLL
	Delay_100us(2);
	TFT_WriteCom(Com_SetPll);
	TFT_WriteData(0x0003);
	Delay_100us(2);
	TFT_WriteCom(Com_SoftReset);							// Software Reset
	Delay_100us(2);

	TFT_WriteCom(Com_SetLshiftFreq);					// Set the LSHIFT (pixel clock) frequency
	TFT_WriteData(0x0001);
	TFT_WriteData(0x0033);
	TFT_WriteData(0x0032);

	TFT_WriteCom(Com_SetLcdMode);							// Set the LCD panel mode and resolution
	TFT_WriteData(0x0000);
	TFT_WriteData(0x0000);
	TFT_WriteData((TFT.HDP>>8)&0x00FF);				// High byte horizontal panel size
	TFT_WriteData(TFT.HDP&0x00FF);						// Low byte horizontal panel size
	TFT_WriteData((TFT.VDP>>8)&0x00FF);				// High byte of the vertical panel size
	TFT_WriteData(TFT.VDP&0x00FF);						// Low byte of the vertical panel size
	TFT_WriteData(0x0000);										// Even and Odd line RGB sequence for serial TFT interface

	TFT_WriteCom(Com_SetHoriPeriod);					// Set front porch
	TFT_WriteData((TFT.HT>>8)&0x00FF);				// High byte of horizontal total period in pixel clock
	TFT_WriteData(TFT.HT&0x00FF);							// Low byte of horizontal total period in pixel clock
	TFT_WriteData((TFT.HPS>>8)&0x00FF);				// 
	TFT_WriteData(TFT.HPS&0x00FF);						// 
	TFT_WriteData(TFT.HPW);										// 
	TFT_WriteData((TFT.LPS>>8)&0x00FF);				// 
	TFT_WriteData(TFT.LPS&0x00FF);						// 
	TFT_WriteData(0x0000);										// 

	TFT_WriteCom(Com_SetVertPeriod);					// Set the vertical blanking interval
	TFT_WriteData((TFT.VT>>8)&0x00FF);				// 
	TFT_WriteData(TFT.VT&0x00FF);							// 
	TFT_WriteData((TFT.VPS>>8)&0x00FF);				// 
	TFT_WriteData(TFT.VPS&0x00FF);						// 
	TFT_WriteData(TFT.VPW);										// 
	TFT_WriteData((TFT.FPS>>8)&0x00FF);				// 
	TFT_WriteData(TFT.FPS&0x00FF);						// 

	TFT_WriteCom(Com_SetGpioValue);
	TFT_WriteData(0x000F);    								// GPIO[3:0] out 1

	TFT_WriteCom(Com_SetGpioConf);
	TFT_WriteData(0x0007);										// GPIO3=input, GPIO[2:0]=output
	TFT_WriteData(0x0001);										// GPIO0 normal

	TFT_WriteCom(Com_SetAddressMode);					// Rotation
	TFT_WriteData(0x0000);

	TFT_WriteCom(Com_SetPixelDataInterface);	// Pixel data interface
 	TFT_WriteData(0x0002);										// 16-bit packed
// 	TFT_WriteData(0x0003);										// 16-bit 565Mode

	Delay_100us(1);

	TFT_WriteCom(Com_SetPostProc);						// Set the image post processor
	TFT_WriteData(TFT.Contrast);							// Set the contrast value
	TFT_WriteData(TFT.Brightness);						// Set the brightness value
	TFT_WriteData(TFT.Saturation);						// Set the saturation value
	TFT_WriteData(0x0001);										// Post Processor Enable

	TFT_WriteCom(Com_SetDisplayOn);						// Show the image on the display panel
	Delay_100us(2);

	TFT_Clear(BLACK);

	TFT_WriteCom(Com_SetPwmConf);							// Set PWM for B/L
	TFT_WriteData(0x0006);
	TFT_WriteData(0x0080);
	TFT_WriteData(0x0001);
	TFT_WriteData(0x00F0);
	TFT_WriteData(0x0000);
	TFT_WriteData(0x0000);

	TFT_WriteCom(Com_SetDbcConf);
	TFT_WriteData(0x000D);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : TFT_RESET
**功能 : TFT Reset
**輸入 : None
**輸出 : None
**使用 : TFT_RESET()
**=====================================================================================================*/
/*=====================================================================================================*/
void TFT_RESET( void )
{
	TFT_RST = 0;
	Delay_1ms(10);
	TFT_RST = 1;
	Delay_1ms(10);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : TFT_Clear
**功能 : Clear Windows
**輸入 : Color
**輸出 : None
**使用 : TFT_Clear(BLACK)
**=====================================================================================================*/
/*=====================================================================================================*/
void TFT_Clear( u32 Color )
{
	u32 Point = 0;

	TFT_WriteCom(Com_SetColumnAddress);
	TFT_WriteData(0);
	TFT_WriteData(0);
	TFT_WriteData(TFT.HDP>>8);
	TFT_WriteData(TFT.HDP&0x00ff);

	TFT_WriteCom(Com_SetPageAddress);
	TFT_WriteData(0);
	TFT_WriteData(0);
	TFT_WriteData(TFT.VDP>>8);
	TFT_WriteData(TFT.VDP&0x00ff);

	TFT_WriteCom(Com_WriteMemoryStart);
 	TFT_WriteCom(Com_WriteMemoryStart);	

	Point = TFT_L*TFT_W;

	while(Point--)
		TFT_WriteColor(Color);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : TFT_DrawPoint
**功能 : Draw Point
**輸入 : CoordiX, CoordiY, Color
**輸出 : None
**使用 : TFT_DrawPoint(CoordiX, CoordiY, Color)
**=====================================================================================================*/
/*=====================================================================================================*/
void TFT_DrawPoint( u16 CoordiX, u16 CoordiY, u32 Color )
{
	TFT_WriteCom(Com_SetColumnAddress);
	TFT_WriteData(CoordiX>>8);					// 起始地址X
	TFT_WriteData(CoordiX);
	TFT_WriteData(CoordiX>>8);					// 結束地址X
	TFT_WriteData(CoordiX);
  TFT_WriteCom(Com_SetPageAddress);
	TFT_WriteData(CoordiY>>8);					// 起始地址Y
	TFT_WriteData(CoordiY);
	TFT_WriteData(CoordiY>>8);					// 結束地址Y
	TFT_WriteData(CoordiY);
	TFT_WriteCom(Com_WriteMemoryStart);

	TFT_WriteColor(Color);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : TFT_DrawLine
**功能 : Draw Line
**輸入 : StartX, StartY, EndX, EndY, Color
**輸出 : None
**使用 : TFT_DrawLine(StartX, StartY, EndX, EndY, Color)
**=====================================================================================================*/
/*=====================================================================================================*/
void TFT_DrawLine( u16 StartX, u16 StartY, u16 EndX, u16 EndY, u32 Color )
{
	u16 i;
	s16 DeltaX, DeltaY;
	double Slope;

	DeltaX = EndX - StartX;
	DeltaY = EndY - StartY;

	// 計算 Slope
	if(DeltaX == 0)
		Slope = 0;
	else
		Slope = (double)DeltaY/(double)DeltaX;

	DeltaX = fabs(DeltaX);
	DeltaY = fabs(DeltaY);

	// 畫線
	if(EndX<StartX) {
		if(Slope<0) {
			if(DeltaX>DeltaY) {
				for(i=0; i<=DeltaX; i++)
					TFT_DrawPoint(EndX+i, EndY+(s16)(((double)i*Slope)-0.5), Color);
			}
			else {
				for(i=0; i<=DeltaY; i++)
					TFT_DrawPoint(EndX-(s16)(((double)i/Slope)-0.5), EndY-i, Color);
			}
		}
		else {
			if(DeltaX>DeltaY) {
				for(i=0; i<=DeltaX; i++)
					TFT_DrawPoint(EndX+i, EndY+(s16)(((double)i*Slope)+0.5), Color);
			}
			else {
				for(i=0; i<=DeltaY; i++)
					TFT_DrawPoint(EndX+(s16)(((double)i/Slope)+0.5), EndY+i, Color);
			}
		}
	}
	else if(EndX==StartX) {
		if(EndY>StartY) {
			for(i=0; i<=DeltaY; i++)
				TFT_DrawPoint(StartX, StartY+i, Color);
		}
		else {
			for(i=0; i<=DeltaY; i++)
				TFT_DrawPoint(EndX, EndY+i, Color);
		}
	}
	else {
		if(Slope<0) {
			if(DeltaX>DeltaY) {
				for(i=0; i<=DeltaX; i++)
					TFT_DrawPoint(StartX+i, StartY+(s16)(((double)i*Slope)-0.5), Color);
			}
			else {
				for(i=0; i<=DeltaY; i++)
					TFT_DrawPoint(StartX-(s16)(((double)i/Slope)-0.5), StartY-i, Color);
			}
		}
		else {
			if(DeltaX>DeltaY) {
				for(i=0; i<=DeltaX; i++)
					TFT_DrawPoint(StartX+i, StartY+(s16)(((double)i*Slope)+0.5), Color);
			}
			else {
				for(i=0; i<=DeltaY; i++)
					TFT_DrawPoint(StartX+(s16)(((double)i/Slope)+0.5), StartY+i, Color);
			}
		}
	}
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : TFT_DrawLineX
**功能 : Draw X-Axis Line
**輸入 : CoordiX, CoordiY, Length, Color
**輸出 : None
**使用 : TFT_DrawLine(CoordiX, CoordiY, Length, Color)
**=====================================================================================================*/
/*=====================================================================================================*/
void TFT_DrawLineX( u16 CoordiX, u16 CoordiY, u16 Length, u32 Color )
{
	u16 i;

	for(i=0; i<Length; i++)
		TFT_DrawPoint(CoordiX+i, CoordiY, Color);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : TFT_DrawLineY
**功能 : Draw Y-Axis Line
**輸入 : CoordiX, CoordiY, Length, Color
**輸出 : None
**使用 : TFT_DrawLine(CoordiX, CoordiY, Length, Color)
**=====================================================================================================*/
/*=====================================================================================================*/
void TFT_DrawLineY( u16 CoordiX, u16 CoordiY, u16 Length, u32 Color )
{
	u16 i;

	for(i=0; i<Length; i++)
		TFT_DrawPoint(CoordiX, CoordiY+i, Color);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : TFT_DrawRectangle
**功能 : Draw Rectangle
**輸入 : CoordiX, CoordiY, Length, Width, Color
**輸出 : None
**使用 : TFT_DrawRectangle(CoordiX, CoordiY, Length, Width, Color)
**=====================================================================================================*/
/*=====================================================================================================*/
void TFT_DrawRectangle( u16 CoordiX, u16 CoordiY, u16 Length, u16 Width, u32 Color )
{
	u16 i;

	for(i=0; i<=Length; i++) {
		TFT_DrawPoint(CoordiX+i, CoordiY, Color);
		TFT_DrawPoint(CoordiX+i, CoordiY+Width, Color);
	}
	for(i=1; i<Width; i++) {
		TFT_DrawPoint(CoordiX, CoordiY+i, Color);
		TFT_DrawPoint(CoordiX+Length, CoordiY+i, Color);
	}
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : TFT_DrawRectangleFull
**功能 : Draw Rectangle
**輸入 : CoordiX, CoordiY, Length, Width, Color
**輸出 : None
**使用 : TFT_DrawRectangleFull(CoordiX, CoordiY, Length, Width, Color)
**=====================================================================================================*/
/*=====================================================================================================*/
void TFT_DrawRectangleFull( u16 CoordiX, u16 CoordiY, u16 Length, u16 Width, u32 Color )
{
	u16 i, j;

	for(i=0; i<Width; i++)
		for(j=0; j<Length; j++)
			TFT_DrawPoint(CoordiX+j, CoordiY+i, Color);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : TFT_DrawCircle
**功能 : Draw Circle
**輸入 : CoordiX, CoordiY, Radius, Color
**輸出 : None
**使用 : TFT_DrawCircle(CoordiX, CoordiY, Radius, Color)
**=====================================================================================================*/
/*=====================================================================================================*/
void TFT_DrawCircle( u16 CoordiX, u16 CoordiY, u16 Radius, u32 Color )
{
	int a,b;
	int di;

	a=0;
	b=Radius;
	di=3-(Radius<<1);

	while(a<=b) {
		TFT_DrawPoint(CoordiX-b, CoordiY-a, Color);             //3
		TFT_DrawPoint(CoordiX+b, CoordiY-a, Color);             //0
		TFT_DrawPoint(CoordiX-a, CoordiY+b, Color);             //1
		TFT_DrawPoint(CoordiX-b, CoordiY-a, Color);             //7
		TFT_DrawPoint(CoordiX-a, CoordiY-b, Color);             //2
		TFT_DrawPoint(CoordiX+b, CoordiY+a, Color);             //4
		TFT_DrawPoint(CoordiX+a, CoordiY-b, Color);             //5
		TFT_DrawPoint(CoordiX+a, CoordiY+b, Color);             //6
		TFT_DrawPoint(CoordiX-b, CoordiY+a, Color);
		a++;
		// 使用Bresenham算法畫圓
		if(di<0)
			di += 4*a+6;
		else {
			di+=10+4*(a-b);
			b--;
		}
		TFT_DrawPoint(CoordiX+a, CoordiY+b, Color);
	}
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : TFT_DrawPicture
**功能 : Draw Picture
**輸入 : CoordiX, CoordiY, Length, Width, Pic
**輸出 : None
**使用 : TFT_DrawPicture(CoordiX, CoordiY, Length, Width, Pic)
**=====================================================================================================*/
/*=====================================================================================================*/
void TFT_DrawPicture( u16 CoordiX, u16 CoordiY, u16 Length, u16 Width, u8 *Pic )
{
	u32 i;
	u8 ColorR, ColorG, ColorB;

	for(i=0; i<Length*Width; i++)	{
			ColorR = Pic[i];
			ColorG = Pic[i++];
			ColorB = Pic[i++];
			TFT_DrawPoint(CoordiX, CoordiY, RGB(ColorR, ColorG, ColorB));
	}
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : TFT_PutChar1206
**功能 : Put Char
**輸入 : CoordiX, CoordiY, ChWord, FontColor, BackColor
**輸出 : None
**使用 : TFT_PutChar1206(x, y, "C", WHITE, BLACK)
**=====================================================================================================*/
/*=====================================================================================================*/
void TFT_PutChar1206( u16 CoordiX, u16 CoordiY, u8* ChWord, u32 FontColor, u32 BackColor )
{
	u8 Tmp_Char = 0;
	u16 i = 0, j = 0;

	for(i=0; i<12; i++) {
		Tmp_Char = ASCII_12x6[*ChWord-32][i];
		for(j=0; j<6; j++) {
			if((Tmp_Char>>j) & 0x01 == 0x01)
				TFT_DrawPoint(CoordiX+j, CoordiY+i, FontColor);	// 字符顏色
			else
				TFT_DrawPoint(CoordiX+j, CoordiY+i, BackColor);	// 背景顏色
		}
	}
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : TFT_PutChar1608
**功能 : Put Char
**輸入 : CoordiX, CoordiY, ChWord, FontColor, BackColor
**輸出 : None
**使用 : TFT_PutChar1608(x, y, "C", WHITE, BLACK)
**=====================================================================================================*/
/*=====================================================================================================*/
void TFT_PutChar1608( u16 CoordiX, u16 CoordiY, u8* ChWord, u32 FontColor, u32 BackColor )
{
	u8 Tmp_Char = 0;
	u16 i = 0, j = 0;

	for(i=0; i<16; i++) {
		Tmp_Char = ASCII_16x8[*ChWord-32][i];
		for(j=0; j<8; j++) {
			if((Tmp_Char>>(7-j)) & 0x01 == 0x01)
				TFT_DrawPoint(CoordiX+j, CoordiY+i, FontColor);	// 字符顏色
			else
				TFT_DrawPoint(CoordiX+j, CoordiY+i, BackColor);	// 背景顏色
		}
	}
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : TFT_PutChar1608_
**功能 : Put Char
**輸入 : CoordiX, CoordiY, ChWord, FontColor, BackColor
**輸出 : None
**使用 : TFT_PutChar1608(x, y, "C", WHITE, BLACK)
**=====================================================================================================*/
/*=====================================================================================================*/
void TFT_PutChar1608_( u16 CoordiX, u16 CoordiY, u8* ChWord, u32 FontColor, u32 BackColor )
{
	u8 Tmp_Char = 0;
	u16 i = 0, j = 0;

	for(i=0; i<16; i++) {
		Tmp_Char = ASCII_16x8_[*ChWord-32][i];
		for(j=0; j<8; j++) {
			if((Tmp_Char>>j) & 0x01 == 0x01)
				TFT_DrawPoint(CoordiX+j, CoordiY+i, FontColor);	// 字符顏色
			else
				TFT_DrawPoint(CoordiX+j, CoordiY+i, BackColor);	// 背景顏色
		}
	}
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : TFT_PutStr
**功能 : Put String
**輸入 : CoordiX, CoordiY, ChWord, FontStyle, FontColor, BackColor
**輸出 : None
**使用 : TFT_PutStr(x, y, "PUT CHAR", WHITE, BLACK)
**=====================================================================================================*/
/*=====================================================================================================*/
void TFT_PutStr( u16 CoordiX, u16 CoordiY, u8 *ChWord, u8 FontStyle, u32 FontColor, u32 BackColor )
{
	u16 OffsetX = 0;

	switch(FontStyle) {
		case ASCII1206:
			while(*ChWord) {
				TFT_PutChar1206(CoordiX+OffsetX, CoordiY, ChWord, FontColor, BackColor);
				ChWord++;
				OffsetX += 6;
			}
			break;
		case ASCII1608:
			while(*ChWord) {
				TFT_PutChar1608(CoordiX+OffsetX, CoordiY, ChWord, FontColor, BackColor);
				ChWord++;
				OffsetX += 8;
			}
			break;
		case ASCII1608_:
			while(*ChWord) {
				TFT_PutChar1608_(CoordiX+OffsetX, CoordiY, ChWord, FontColor, BackColor);
				ChWord++;
				OffsetX += 8;
			}
			break;
	}
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : TFT_PutNum
**功能 : Put Number
**輸入 : oordiX, CoordiY, Type, Length, NumData, FontColor, BackColor
**輸出 : None
**使用 : TFT_PutNum(CoordiX, CoordiY, Type_D, Length, NumData, WHITE, BLACK)
**=====================================================================================================*/
/*=====================================================================================================*/
void TFT_PutNum( u16 CoordiX, u16 CoordiY, u8 Type, u8 Length, u32 NumData, u32 FontColor, u32 BackColor )
{
	u8 TmpNumber[16] = {0};

	NumToChar(Type, Length, TmpNumber, NumData);
	TFT_PutStr(CoordiX, CoordiY, TmpNumber, ASCII1608, FontColor, BackColor);
}
/*=====================================================================================================*/
/*=====================================================================================================*/
