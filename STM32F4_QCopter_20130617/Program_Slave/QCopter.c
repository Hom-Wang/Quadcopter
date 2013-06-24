/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_spi.h"
#include "stm32f4_i2c.h"
#include "QCopter.h"
#include "QCopter_AHRS.h"
#include "QCopter_CTRL.h"
#include "module_keyBoard.h"
#include "module_sensor.h"
#include "module_rs232.h"
#include "module_nrf24l01.h"
#include "module_motor.h"
#include "module_l3g4200.h"
#include "module_lsm303.h"
#include "algorithm_pid.h"
#include "algorithm_quaternion.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
u8 Connect_Test = 0;
u16 ADC_TMP = 0;

vs16 PWM_M1 = PWM_MOTOR_MIN;
vs16 PWM_M2 = PWM_MOTOR_MIN;
vs16 PWM_M3 = PWM_MOTOR_MIN;
vs16 PWM_M4 = PWM_MOTOR_MIN;

extern vu8 Time_Sec;
extern vu8 Time_Min;
extern vu8 SensorMode;

extern vs16 Tmp_PID_KP;
extern vs16 Tmp_PID_KI;
extern vs16 Tmp_PID_KD;
extern vs16 Tmp_PID_Pitch;
/*=====================================================================================================*/
/*=====================================================================================================*/
int main( void )
{
	u8 Sta = ERROR;
	u8 FSM_Sta = FSM_Rx;
	u16 Tmp_PWM_M1 = PWM_MOTOR_MIN;
	u16 Tmp_PWM_M2 = PWM_MOTOR_MIN;
	u16 Tmp_PWM_M3 = PWM_MOTOR_MIN;
	u16 Tmp_PWM_M4 = PWM_MOTOR_MIN;

	u16 PWM_Motor = 0;

	SystemInit();
	LED_Config();
  KEY_Config();
	RS232_Config();
	nRF24L01_Config();
 	I2C_Config();
	Motor_Config();

  Sensor_Init();
  PID_Init(&PID_Yaw);
  PID_Init(&PID_Roll);
  PID_Init(&PID_Pitch);

	/* 油門校正 */
	LED1 = 1;
	LED2 = 1;
	LED3 = 1;
	LED4 = 1;
	if(KEY == 1) {
		LED1 = 1;
		LED2 = 1;
		LED3 = 0;
		LED4 = 1;
    Motor_Control(PWM_MOTOR_MAX, PWM_MOTOR_MAX, PWM_MOTOR_MAX, PWM_MOTOR_MAX);
	}
	while(KEY == 1);
	LED3 = 1;
  Motor_Control(PWM_MOTOR_MIN, PWM_MOTOR_MIN, PWM_MOTOR_MIN, PWM_MOTOR_MIN);
  Sevro_Control(PWM_SEVRO_MED, PWM_SEVRO_MED);

	LED_PWM_Shine(0xF0, 1, 400);

	L3G4200_Init();
	LSM303A_Init();
	LSM303M_Init();

	LED_PWM_Shine(0xF0, 1, 400);

	while(Sta == ERROR)
		Sta = nRF_Check();

	LED_PWM_Shine(0xF0, 1, 400);

	LED_Shine(0x08, 10, 100);

 	if(SysTick_Config(420000)) {		// 168MHz / 420000 = 400Hz = 2.5ms
		while(1);
	}

	while(KEY == 0 || SensorMode == Mode_OffSet || SensorMode == Mode_Magnetic)
		Delay_1ms(1000);

	LED_Shine(0xFF, 1, 200);

	while(1) {
		LED1 = ~LED1;
		switch(FSM_Sta) {

/************************** FSM Tx **************************************/
			case FSM_Tx:
				// FSM_Tx
				nRF_TX_Mode();
				do {
					Sta = nRF_Tx_Data(TxBuf[0]);
				} while(Sta == MAX_RT);
				// FSM_Tx End
				FSM_Sta = FSM_Rx;
				break;

/************************** FSM Rx **************************************/
			case FSM_Rx:
				// FSM_Rx
				nRF_RX_Mode();
				Sta = nRF_Rx_Data(RxBuf[0]);
				if(Sta == RX_DR) {
					KeyBoard_Read(RxBuf[0]);
				}
				// FSM_Rx End
				FSM_Sta = FSM_CTRL;
				break;
 
/************************** FSM I2C **************************************/
			case FSM_I2C:
				// FSM_I2C
				Connect_Test++;
				if(Connect_Test == 250)
					Connect_Test = 0;

        /* 資料轉換成整數 */
        Acc.X = (s16)(Acc.TrueX*1000);  // 1 mg/LSB
        Acc.Y = (s16)(Acc.TrueY*1000);  // 1 mg/LSB
        Acc.Z = (s16)(Acc.TrueZ*1000);  // 1 mg/LSB
        Gyr.X = (s16)(Gyr.TrueX*100);   // 10 mdps/LSB
        Gyr.Y = (s16)(Gyr.TrueY*100);   // 10 mdps/LSB
        Gyr.Z = (s16)(Gyr.TrueZ*100);   // 10 mdps/LSB
        Meg.X = (s16)(Meg.TrueX*10000); // 100 ugauss/LSB
        Meg.Y = (s16)(Meg.TrueY*10000); // 100 ugauss/LSB
        Meg.Z = (s16)(Meg.TrueZ*10000); // 100 ugauss/LSB
        Ang.X = (s16)(AngE.Pitch*100);  // 10 mdeg/LSB
        Ang.Y = (s16)(AngE.Roll*100);   // 10 mdeg/LSB
        Ang.Z = (s16)(AngE.Yaw*10);     // 100 mdeg/LSB

				TxBuf[0][0]  = (u8)(0x01);
				TxBuf[0][1]  = (u8)(Connect_Test);
				TxBuf[0][2]  = (u8)(Acc.X);
				TxBuf[0][3]  = (u8)(Acc.X >> 8);
				TxBuf[0][4]  = (u8)(Acc.Y);
				TxBuf[0][5]  = (u8)(Acc.Y >> 8);
				TxBuf[0][6]  = (u8)(Acc.Z);
				TxBuf[0][7]  = (u8)(Acc.Z >> 8);
				TxBuf[0][8]  = (u8)(Gyr.X);
				TxBuf[0][9]  = (u8)(Gyr.X >> 8);
				TxBuf[0][10] = (u8)(Gyr.Y);
				TxBuf[0][11] = (u8)(Gyr.Y >> 8);
				TxBuf[0][12] = (u8)(Gyr.Z);
				TxBuf[0][13] = (u8)(Gyr.Z >> 8);

// 				TxBuf[0][0]  = (u8)(0x01);
// 				TxBuf[0][1]  = (u8)(Connect_Test);
// 				TxBuf[0][2]  = Byte8L(Acc.X);
// 				TxBuf[0][3]  = Byte8H(Acc.X);
// 				TxBuf[0][4]  = Byte8L(Acc.Y);
// 				TxBuf[0][5]  = Byte8H(Acc.Y);
// 				TxBuf[0][6]  = Byte8L(Acc.Z);
// 				TxBuf[0][7]  = Byte8H(Acc.Z);
// 				TxBuf[0][8]  = Byte8L(Gyr.X);
// 				TxBuf[0][9]  = Byte8H(Gyr.X);
// 				TxBuf[0][10] = Byte8L(Gyr.Y);
// 				TxBuf[0][11] = Byte8H(Gyr.Y);
// 				TxBuf[0][12] = Byte8L(Gyr.Z);
// 				TxBuf[0][13] = Byte8H(Gyr.Z);

// TxBuf[0][14] = (u8)(Exp_Pitch);
// TxBuf[0][15] = (u8)(Exp_Pitch >> 8);
// TxBuf[0][16] = (u8)(Exp_Roll);
// TxBuf[0][17] = (u8)(Exp_Roll >> 8);
// TxBuf[0][18] = (u8)(Exp_Yaw);
// TxBuf[0][19] = (u8)(Exp_Yaw >> 8);
// TxBuf[0][28] = (u8)(Exp_Thr);
// TxBuf[0][29] = (u8)(Exp_Thr >> 8);

        TxBuf[0][14] = (u8)(Tmp_PID_KP);
        TxBuf[0][15] = (u8)(Tmp_PID_KP >> 8);
        TxBuf[0][16] = (u8)(Tmp_PID_KI);
        TxBuf[0][17] = (u8)(Tmp_PID_KI >> 8);
        TxBuf[0][18] = (u8)(Tmp_PID_KD);
        TxBuf[0][19] = (u8)(Tmp_PID_KD >> 8);

// 				TxBuf[0][14] = (u8)(Meg.X);
// 				TxBuf[0][15] = (u8)(Meg.X >> 8);
// 				TxBuf[0][16] = (u8)(Meg.Y);
// 				TxBuf[0][17] = (u8)(Meg.Y >> 8);
// 				TxBuf[0][18] = (u8)(Meg.Z);
// 				TxBuf[0][19] = (u8)(Meg.Z >> 8);

				TxBuf[0][20] = (u8)(Ang.X);
				TxBuf[0][21] = (u8)(Ang.X >> 8);
				TxBuf[0][22] = (u8)(Ang.Y);
				TxBuf[0][23] = (u8)(Ang.Y >> 8);
				TxBuf[0][24] = (u8)(Ang.Z);
				TxBuf[0][25] = (u8)(Ang.Z >> 8);
				TxBuf[0][26] = (u8)(PWM_Motor);
				TxBuf[0][27] = (u8)(PWM_Motor >> 8);
				TxBuf[0][28] = (u8)(Tmp_PID_Pitch);
				TxBuf[0][29] = (u8)(Tmp_PID_Pitch >> 8);
				TxBuf[0][30] = (u8)(Time_Sec);
				TxBuf[0][31] = (u8)(Time_Min);
				// FSM_I2C End
				FSM_Sta = FSM_Tx;
				break;

/************************** FSM CTRL **************************************/
			case FSM_CTRL:
				// FSM_CTRL

				if( KEYR_L == 0 ) {
					Tmp_PWM_M1 = PWM_MOTOR_MIN;
					Tmp_PWM_M2 = PWM_MOTOR_MIN;
					Tmp_PWM_M3 = PWM_MOTOR_MIN;
					Tmp_PWM_M4 = PWM_MOTOR_MIN;
					PWM_M1 = Tmp_PWM_M1;
					PWM_M2 = Tmp_PWM_M2;
					PWM_M3 = Tmp_PWM_M3;
					PWM_M4 = Tmp_PWM_M4;
					PWM_Motor = Tmp_PWM_M1;
				}
				else if( KEYL_J == 0 ) {
					ADC_TMP = PWM_MOTOR_MIN + JSR_Z*0.30379746835443037974683544303797f - 70;
					if(ADC_TMP<PWM_MOTOR_MIN)
						ADC_TMP = PWM_MOTOR_MIN;
					if(ADC_TMP>PWM_MOTOR_MAX)
						ADC_TMP = PWM_MOTOR_MAX;

					Tmp_PWM_M1 = ADC_TMP;
					Tmp_PWM_M2 = ADC_TMP;
					Tmp_PWM_M3 = ADC_TMP;
					Tmp_PWM_M4 = ADC_TMP;
					PWM_M1 = Tmp_PWM_M1;
					PWM_M2 = Tmp_PWM_M2;
					PWM_M3 = Tmp_PWM_M3;
					PWM_M4 = Tmp_PWM_M4;
					PWM_Motor = Tmp_PWM_M1;
				}
				// FSM_CTRL End
				FSM_Sta = FSM_USART;
				break;

/************************** FSM USART **************************************/
			case FSM_USART:
				// FSM_USART
				RS232_VisualScope(USART1, TxBuf[0]+20, 8);
				// FSM_USART End
				FSM_Sta = FSM_I2C;
				break;
		}
	}
}
/*=====================================================================================================*/
/*=====================================================================================================*/
