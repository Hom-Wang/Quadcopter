/* #include "system_windows.h" */

#ifndef __SYSTEM_WINDOWS_H
#define __SYSTEM_WINDOWS_H

#include "stm32f4xx.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
#define Windows_CTRL	1
#define Windows_WAVE	2
#define Windows_AHRS	3
#define Windows_IMAGE	4
#define Windows_GPS		5
#define Windows_TEST1	6
#define Windows_TEST2	7

#define Wave_LSM303A	1
#define Wave_L3G4200	2
#define Wave_LSM303M	3
#define Wave_Angular	4
#define Wave_PID			5
#define Wave_SinCos		6

#define Window_W	246
#define Window_L	480
/*=====================================================================================================*/
/*=====================================================================================================*/
void Windows_Init( void );
void Windows_Interface( u8 Windows_Sel );
void Windows_DrawMenu( void );
void Windows_DrawButton( u16 CoordiX, u16 CoordiY, u32 Color );

void Windows_DrawFull( u32 );
void Windows_Change( u8 );

static void Windows_SelMenu_Init( void );
static void Windows_SelMenu( u16, u32, u32 );
static void MenuCtrl_Motor_Init( void );
static void MenuCtrl_Motor( void );
static void MenuCtrl_Power_Init( void );
static void MenuCtrl_Power( void );
static void MenuCtrl_Connection_Init( void );
static void MenuCtrl_Connection( void );
static void MenuCtrl_Mode_Init( void );
static void MenuCtrl_Mode( void );
static void MenuCtrl_WorkTime_Init( void );
static void MenuCtrl_WorkTime( void );
/*=====================================================================================================*/
/*=====================================================================================================*/
#endif
