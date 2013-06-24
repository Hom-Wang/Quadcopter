/* #include "Module_KeyBoard_s.h" */

#ifndef __MODULE_KEYBOARD_S_H
#define __MODULE_KEYBOARD_S_H

#include "stm32f4xx.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
#define KeyNums			32

#define LED_ON			0
#define LED_OFF			1

#define LED_PWM_Max	2000
#define LED_PWM_Med	1000
#define LED_PWM_Min	0

#define LED1				PAO(8)	// W1
#define LED2				PDO(2)	// G1
#define LED3				PAO(4)	// B1
#define LED4				PBO(5)	// R1
#define LED5				TIM4->CCR3	// W2
#define LED6				TIM5->CCR3	// G2
#define LED7				TIM4->CCR4	// B2
#define LED8				TIM5->CCR4	// R2
#define KEY					PAI(5)

#define KEYR_J			KeyCtrl[0]
#define KEYR_U			KeyCtrl[1]
#define KEYR_D			KeyCtrl[2]
#define KEYR_L			KeyCtrl[3]
#define KEYR_R			KeyCtrl[4]
#define KEYR_S1			KeyCtrl[5]
#define KEYR_S2			KeyCtrl[6]
#define KEYL_J			KeyCtrl[7]
#define KEYL_U			KeyCtrl[8]
#define KEYL_D			KeyCtrl[9]
#define KEYL_L			KeyCtrl[10]
#define KEYL_R			KeyCtrl[11]
#define KEYL_S1			KeyCtrl[12]
#define KEYL_S2			KeyCtrl[13]
#define JSR_X				KeyCtrl[14]
#define JSR_Y				KeyCtrl[15]
#define JSR_Z				KeyCtrl[16]
#define JSL_X				KeyCtrl[17]
#define JSL_Y				KeyCtrl[18]
#define JSL_Z				KeyCtrl[19]

#define Exp_Pitch	  KeyCtrl[20]
#define Exp_Roll    KeyCtrl[21]
#define Exp_Yaw     KeyCtrl[22]
#define Exp_Thr     KeyCtrl[23]

#define JSR_X_Max	4080
#define JSR_X_Med	1986
#define JSR_X_Min	210
#define JSR_Y_Max	4080
#define JSR_Y_Med	1986
#define JSR_Y_Min	210
#define JSR_Z_Max	0
#define JSR_Z_Med	0
#define JSR_Z_Min	0

// //TEST
// #define KEY_Kp			PCI(10)
// #define KEY_Ki			PCI(11)
// #define KEY_Kd			PCI(12)
// #define KEY_U				PCI(13)
// #define KEY_D				PCI(14)
/*====================================================================================================*/
/*====================================================================================================*/
void LED_Config( void );
void KEY_Config( void );
void KeyBoard_Read( u8* );
void LED_Shine( u16, u16, u16 );
void LED_PWM( u8, u16, u16 );
void LED_PWM_Shine( u16, u16, u16 );
/*====================================================================================================*/
/*====================================================================================================*/
extern vu16 KeyCtrl[KeyNums];
/*====================================================================================================*/
/*====================================================================================================*/
#endif
