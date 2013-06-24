/* #include "module_bmp085.h" */

#ifndef __MODULE_BMP085_H
#define __MODULE_BMP085_H

#include "stm32f4xx.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
#define BMP085_I2C_ADDR		((u8)0xEE)

#define BMP085_CTRL				((u8)0xF4)
#define BMP085_UT					((u8)0x2E)
#define BMP085_UP_ULR			((u8)0x00)	// ((u8)0x34)	// Ultra Low Power
#define BMP085_UP_SR			((u8)0x01)	// ((u8)0x74)	// Standard
#define BMP085_UP_HR			((u8)0x02)	// ((u8)0xB4)	// High Resolution
#define BMP085_UP_UHR			((u8)0x03)	// ((u8)0xF4)	// Ultra High Resolution

#define BMP085_DATA_MSB		((u8)0xF6)
#define BMP085_DATA_LSB		((u8)0xF7)
#define BMP085_DATA_XLSB	((u8)0xF8)

#define BMP085_AC1_H			((u8)0xAA)
#define BMP085_AC1_L			((u8)0xAB)
#define BMP085_AC2_H			((u8)0xAC)
#define BMP085_AC2_L			((u8)0xAD)
#define BMP085_AC3_H			((u8)0xAE)
#define BMP085_AC3_L			((u8)0xAF)
#define BMP085_AC4_H			((u8)0xB0)
#define BMP085_AC4_L			((u8)0xB1)
#define BMP085_AC5_H			((u8)0xB2)
#define BMP085_AC5_L			((u8)0xB3)
#define BMP085_AC6_H			((u8)0xB4)
#define BMP085_AC6_L			((u8)0xB5)
#define BMP085_B1_H				((u8)0xB6)
#define BMP085_B1_L				((u8)0xB7)
#define BMP085_B2_H				((u8)0xB8)
#define BMP085_B2_L				((u8)0xB9)
#define BMP085_MB_H				((u8)0xBA)
#define BMP085_MB_L				((u8)0xBB)
#define BMP085_MC_H				((u8)0xBC)
#define BMP085_MC_L				((u8)0xBD)
#define BMP085_MD_H				((u8)0xBE)
#define BMP085_MD_L				((u8)0xBF)
/*=====================================================================================================*/
/*=====================================================================================================*/
typedef struct {
	s16 AC1;
	s16 AC2;
	s16 AC3;
	u16 AC4;
	u16 AC5;
	u16 AC6;
	s16 B1;
	s16 B2;
	s16 MB;
	s16 MC;
	s16 MD;
	s32 UT;
	s32 UP;
	s16 OSS;
	s32 Temp;
	s32 Press;
} BMP085_Struct;
/*====================================================================================================*/
/*====================================================================================================*/
void BMP085_Init( u8* );
void BMP085_ReadCof( u8* );
void BMP085_Read( BMP085_Struct* );
/*====================================================================================================*/
/*====================================================================================================*/
#endif
