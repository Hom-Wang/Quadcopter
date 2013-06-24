/* #include "module_xpt2046.h" */

#ifndef __MODULE_XPT2046_H
#define __MODULE_XPT2046_H

#include "stm32f4xx.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
#define TFT_PEN		PAI(12)
#define TFT_TPCS	PBO(12)

#define Touch_ReadX 	0x90
#define Touch_ReadY 	0xD0
#define READ_TIMES		10
#define READ_DISCARD	3
#define PEN_TIMES 		10

#define PEN_CORRECT_X	200
#define PEN_CORRECT_Y	440
/*=====================================================================================================*/
/*=====================================================================================================*/
typedef struct {
	u8	Sta;	// 筆的狀態
	u16 X;		// 對應TFT坐標
	u16 Y;
	u16 ADC_X;		// 原始坐標
	u16 ADC_Y;
	u16 ADC_XMax;
	u16 ADC_XMin;
	u16 ADC_YMax;
	u16 ADC_YMin;
} PEN_Struct;
/*=====================================================================================================*/
/*=====================================================================================================*/
void Touch_Init( PEN_Struct *PEN );
void XPT2046_Config( void );
u16 Touch_Read( u8 );
void Touch_ReadXY( PEN_Struct *PEN );
void Touch_Transform( PEN_Struct *PEN );
void Touch_CorrectPoint( u16 CoordiX, u16 CoordiY, u32 Color );
/*=====================================================================================================*/
/*=====================================================================================================*/
#endif
