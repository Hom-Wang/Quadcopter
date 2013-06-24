/* #include "Module_KeyBoard.h" */

#ifndef __MODULE_KEYBOARD_H
#define __MODULE_KEYBOARD_H

#include "stm32f4xx.h"
#include "stm32f4_adc.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
#define LED0    PCO(6)   // 
#define LED1    PCO(7)   // 
#define LED2    PBO(0)   // 
#define LED3    PBO(1)   // 
#define LED4    PAO(0)   // 
#define LED5    PAO(1)   // 
#define LED6    PAO(2)   // 
#define LED7    PAO(3)   // 
#define LED8    PBO(8)   // 
#define LED9    PBO(9)   // 

#define KEYR_J  PCI(14)  // 
#define KEYR_U  PAI(11)  // 
#define KEYR_D  PAI(10)  // 
#define KEYR_L  PAI(8)   // 
#define KEYR_R  PAI(9)   // 
#define KEYR_S1  PEI(1)  // 
#define KEYR_S2  PEI(0)  // 
#define KEYL_J  PEI(2)   // 
#define KEYL_U  PEI(3)   // 
#define KEYL_D  PEI(6)   // 
#define KEYL_L  PEI(4)   // 
#define KEYL_R  PEI(5)   // 
#define KEYL_S1  PCI(13) // 
#define KEYL_S2  PCI(15) // 

/* Joystick ADC Value */
#define Joystick_X_Max  7670
#define Joystick_X_Med  3640
#define Joystick_X_Min  60
#define Joystick_Y_Max  7670
#define Joystick_Y_Med  3670
#define Joystick_Y_Min  100
#define Joystick_Z_Max  0
#define Joystick_Z_Med  0
#define Joystick_Z_Min  0
/*====================================================================================================*/
/*====================================================================================================*/
void KeyBoard_Config( void );
void KeyBoard_Read( u8* );
void LED_Shine( u16, u16, u16 );
void LED_Shift( u16, u16, u16 );
/*====================================================================================================*/
/*====================================================================================================*/
extern u16 ADC_AveTr[ADC_Channel];
/*====================================================================================================*/
/*====================================================================================================*/
#endif
