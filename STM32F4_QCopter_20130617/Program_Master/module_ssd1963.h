/* #include "module_ssd1963.h" */

#ifndef __MODULE_SSD1963_H
#define __MODULE_SSD1963_H

#include "stm32f4xx.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
#define TFT_RAM		*(vu16*)((u32)0x60020000)
#define TFT_REG		*(vu16*)((u32)0x60000000)

#define TFT_L 480
#define TFT_W 272

#define TFT_LIGHT PDO(12)	// 
#define TFT_RST		PDO(13)	// 

/* TFT Size&Type */
#define ASCII1206		((u8)0x00)
#define ASCII1608		((u8)0x04)
#define ASCII1608_	((u8)0x05)

/* TFT Colors */
#define WHITE			((u32)0xFFFFFF)	// 白色
#define BLACK			((u32)0x000000)	// 黑色
#define RED				((u32)0xFF0000)	// 紅色
#define GREEN			((u32)0x00FF00)	// 綠色
#define BLUE			((u32)0x0000FF)	// 藍色
#define MAGENTA		((u32)0xFF00FF)	// 品紅色
#define CYAN			((u32)0x00FFFF)	// 青色
#define YELLOW		((u32)0xFFFF00)	// 黃色
#define BROWN			((u32)0x8B2323)	// 棕色
#define BRRED			((u32)0x8B1A1A)	// 棕紅色
#define GRAY			((u32)0xE6E6FA)	// 灰色
#define LGRAY			((u32)0xF0F8FF)	// 淺灰色

/* TFT Commands */
#define Com_Nop										((u16)0x0000)	// No operation
#define Com_SoftReset							((u16)0x0001)	// Software Reset
#define Com_GetRowerMode					((u16)0x000A)	// Get the current power mode
#define Com_GetAddressMode				((u16)0x000B)	// Get the frame buffer to the display panel read order
#define Com_GetDisplayMode				((u16)0x000D)	// The SSD1963 returns the Display Image Mode
#define Com_GetTearEffectStatus		((u16)0x000E)	// Get the Tear Effect status
#define Com_EnterSleepMode				((u16)0x0010)	// Turn off the panel
#define Com_ExitSleepMode					((u16)0x0011)	// Turn on the panel
#define Com_EnterPartial_mode			((u16)0x0012)	// Part of the display area is used for image display
#define Com_EnterNormal_mode			((u16)0x0013)	// The whole display area is used for image display
#define Com_ExitInvert_mode				((u16)0x0020)	// Displayed image colors are not inverted
#define Com_EnterInvert_mode			((u16)0x0021)	// Displayed image colors are inverted
#define Com_SetGammaCurve					((u16)0x0026)	// Selects the gamma curve used by the display panel
#define Com_SetDisplayOff					((u16)0x0028)	// Blanks the display panel
#define Com_SetDisplayOn					((u16)0x0029)	// Show the image on the display panel
#define Com_SetColumnAddress			((u16)0x002A)	// Set the column address
#define Com_SetPageAddress				((u16)0x002B)	// Set the page address
#define Com_WriteMemoryStart			((u16)0x002C)	// Transfer image information from the host processor interface to the SSD1963 starting at the location provided by set_column_address and set_page_address
#define Com_ReadMemoryStart				((u16)0x002E)	// Transfer image data from the SSD1963 to the host processor interface starting at the location provided by set_column_address and set_page_address
#define Com_SetPartialArea				((u16)0x0030)	// Defines the partial display area on the display panel
#define Com_SetScrollArea					((u16)0x0033)	// Defines the vertical scrolling and fixed area on display area
#define Com_SetTearOff						((u16)0x0034)	// Synchronization information is not sent from the SSD1963 to the host processor
#define Com_SetTearOn							((u16)0x0035)	// Synchronization information is sent from the SSD1963 to the host processor at the start of VFP
#define Com_SetAddressMode				((u16)0x0036)	// Set the read order from frame buffer to the display panel
#define Com_SetScrollStart				((u16)0x0037)	// Defines the vertical scrolling starting point
#define Com_ExitIdleMode					((u16)0x0038)	// Full color depth is used for the display panel
#define Com_EnterIdleMode					((u16)0x0039)	// Reduce color depth is used on the display panel
#define Com_WriteMemoryContinue		((u16)0x003C)	// Transfer image information from the host processor interface to the SSD1963 from the last written location
#define Com_ReadMemoryContinue		((u16)0x003E)	// Read image data from the SSD1963 continuing after the last read_memory_continue or read_memory_start
#define Com_SetTearScanline				((u16)0x0044)	// Synchronization information is sent from the SSD1963 to the host processor when the display panel refresh reaches the provided scanline
#define Com_GetDcanline						((u16)0x0045)	// Get the current scan line
#define Com_ReadDdb								((u16)0x00A1)	// Read the DDB from the provided location
#define Com_SetLcdMode						((u16)0x00B0)	// Set the LCD panel mode and resolution
#define Com_GetLcdMode						((u16)0x00B1)	// Get the current LCD panel mode, pad strength and resolution
#define Com_SetHoriPeriod					((u16)0x00B4)	// Set front porch
#define Com_GetHoriPeriod					((u16)0x00B5)	// Get current front porch settings
#define Com_SetVertPeriod					((u16)0x00B6)	// Set the vertical blanking interval between last scan line and next LFRAME pulse
#define Com_GetVertPeriod					((u16)0x00B7)	// Set the vertical blanking interval between last scan line and next LFRAME pulse
#define Com_SetGpioConf						((u16)0x00B8)	// Set the GPIO configuration.  If the GPIO is not used for LCD, set the direction.  Otherwise, they are toggled with LCD signals
#define Com_GetGpioConf						((u16)0x00B9)	// Get the current GPIO configuration
#define Com_SetGpioValue					((u16)0x00BA)	// Set GPIO value for GPIO configured as output
#define Com_GetGpioStatus					((u16)0x00BB)	// Read current GPIO status.  If the individual GPIO was configured as input, the value is the status of the corresponding pin.  Otherwise, it is the programmed value
#define Com_SetPostProc						((u16)0x00BC)	// Set the image post processor
#define Com_GetPostProc						((u16)0x00BD)	// Set the image post processor
#define Com_SetPwmConf						((u16)0x00BE)	// Set the image post processor
#define Com_GetPwmConf						((u16)0x00BF)	// Set the image post processor
#define Com_SetLcdGen0						((u16)0x00C0)	// Set the rise, fall, period and toggling properties of LCD signal generator 0
#define Com_GetLcdGen0						((u16)0x00C1)	// Get the current settings of LCD signal generator 0
#define Com_SetLcdGen1						((u16)0x00C2)	// Set the rise, fall, period and toggling properties of LCD signal generator 1
#define Com_GetLcdGen1						((u16)0x00C3)	// Get the current settings of LCD signal generator 1
#define Com_SetLcdGen2						((u16)0x00C4)	// Set the rise, fall, period and toggling properties of LCD signal generator 2
#define Com_GetLcdGen2						((u16)0x00C5)	// Get the current settings of LCD signal generator 2
#define Com_SetLcdGen3						((u16)0x00C6)	// Set the rise, fall, period and toggling properties of LCD signal generator 3
#define Com_GetLcdGen3						((u16)0x00C7)	// Get the current settings of LCD signal generator 3
#define Com_SetGpio0Rop						((u16)0x00C8)	// Set the GPIO0 with respect to the LCD signal generators using ROP operation.  No effect if the GPIO0 is configured as general GPIO
#define Com_GetGpio0Rop						((u16)0x00C9)	// Get the GPIO0 properties with respect to the LCD signal generators
#define Com_SetGpio1Rop						((u16)0x00CA)	// Set the GPIO1 with respect to the LCD signal generators using ROP operation.  No effect if the GPIO1 is configured as general GPIO.
#define Com_GetGpio1Rop						((u16)0x00CB)	// Get the GPIO1 properties with respect to the LCD signal generators.
#define Com_SetGpio2Rop						((u16)0x00CC)	// Set the GPIO2 with respect to the LCD signal generators using ROP operation.  No effect if the GPIO2 is configured as general GPIO
#define Com_GetGpio2Rop						((u16)0x00CD)	// Get the GPIO2 properties with respect to the LCD signal generators
#define Com_SetGpio3Rop						((u16)0x00CE)	// Set the GPIO3 with respect to the LCD signal generators using ROP operation.  No effect if the GPIO3 is configured as general GPIO
#define Com_GetGpio3Rop						((u16)0x00CF)	// Get the GPIO3 properties with respect to the LCD signal generators
#define Com_SetDbcConf						((u16)0x00D0)	// Set the dynamic back light configuration 
#define Com_GetDbcConf						((u16)0x00D1)	// Get the current dynamic back light configuration 
#define Com_SetDbcTh							((u16)0x00D4)	// Set the threshold for each level of power saving 
#define Com_GetDbcTh							((u16)0x00D5)	// Get the threshold for each level of power saving 
#define Com_SetPll								((u16)0x00E0)	// Start the PLL.  Before the start, the system was operated with the crystal oscillator or clock input
#define Com_SetPllMn							((u16)0x00E2)	// Set the PLL 
#define Com_GetPllMn							((u16)0x00E3)	// Get the PLL settings 
#define Com_GetPllStatus					((u16)0x00E4)	// Get the current PLL status 
#define Com_SetDeepSleep					((u16)0x00E5)	// Set deep sleep mode 
#define Com_SetLshiftFreq					((u16)0x00E6)	// Set the LSHIFT (pixel clock) frequency 
#define Com_GetLshiftFreq					((u16)0x00E7)	// Get current LSHIFT (pixel clock) frequency setting 
#define Com_SetPixelDataInterface	((u16)0x00F0)	// Set the pixel data format of the parallel host processor interface
#define Com_GetPixelDataInterface	((u16)0x00F1)	// Get the current pixel data format settings 

#define RGB(C_R, C_G, C_B)				(((C_R<<16)&0xFF0000)|((C_G<<8)&0x00FF00)|((C_B)&0x0000FF))
/*=====================================================================================================*/
/*=====================================================================================================*/
typedef struct {
	u8 HPW;
	u8 VPW;
	u16 HDP;				// horizontal panel size
	u16 VDP;				// vertical panel size
	u16 HT;					// 
	u16 VT;					// 
	u16 HPS;				// non-display period between the start of the horizontal sync(LLINE) signal and the first display data
	u16 LPS;				// 
	u16 VPS;
	u16 FPS;
	u16 Contrast;		// 對比
	u16 Brightness;	// 亮度
	u16 Saturation;	// 飽和
} TFT_TypeDef;
/*=====================================================================================================*/
/*=====================================================================================================*/
static void TFT_WriteData( u32 );
static void TFT_WriteCom( u32 );
static void TFT_WriteColor( u32 );

void SSD1963_Config( void );
void SSD1963_Init( void );

void TFT_RESET( void );
void TFT_Clear( u32 );

void TFT_DrawPoint( u16, u16, u32 );
void TFT_DrawLine( u16, u16, u16, u16, u32 );
void TFT_DrawLineX( u16, u16, u16, u32 );
void TFT_DrawLineY( u16, u16, u16, u32 );
void TFT_DrawRectangle( u16, u16, u16, u16, u32 );
void TFT_DrawRectangleFull( u16, u16, u16, u16, u32 );
void TFT_DrawCircle( u16, u16, u16, u32 );
void TFT_DrawPicture( u16, u16, u16, u16, u8* );
void TFT_PutChar1206( u16, u16, u8*, u32, u32 );
void TFT_PutChar1608( u16, u16, u8*, u32, u32 );
void TFT_PutChar1608_( u16, u16, u8*, u32, u32 );
void TFT_PutStr( u16, u16, u8*, u8, u32, u32 );
void TFT_PutNum( u16, u16, u8, u8, u32, u32, u32 );
/*=====================================================================================================*/
/*=====================================================================================================*/
#endif
