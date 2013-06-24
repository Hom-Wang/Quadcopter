/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_i2c.h"
#include "QCopter.h"
#include "QCopter_AHRS.h"
#include "QCopter_CTRL.h"
#include "module_keyBoard.h"
#include "module_motor.h"
#include "module_sensor.h"
#include "module_l3g4200.h"
#include "module_lsm303.h"
#include "algorithm_mathUnit.h"
#include "algorithm_pid.h"
#include "algorithm_moveAve.h"
#include "algorithm_quaternion.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
vu8 Time_Sec = 0;
vu8 Time_Min = 0;

vu8 fTime_Sec = 0;
vu8 fTime_Min = 0;

vu32 SysTick_Cnt = 0;

vu8 SensorMode = Mode_OffSet;		// Sensor 數據處理模式, 初始為偏移校正模式

s16 LSM303A_FIFO[3][256] = {0};
s16 L3G4200_FIFO[3][256] = {0};
s16 LSM303M_FIFO[3][256] = {0};

vs16 Tmp_PID_KP;
vs16 Tmp_PID_KI;
vs16 Tmp_PID_KD;
vs16 Tmp_PID_Pitch;

extern vs16 PWM_M1;
extern vs16 PWM_M2;
extern vs16 PWM_M3;
extern vs16 PWM_M4;

vs16 Thr = 0;

// extern float Kp;
// extern float Ki;
/*=====================================================================================================*/
/*=====================================================================================================*/
void SysTick_Handler( void )
{
	u8 MEMS_Buf[24] = {0};

  u16 Final_M1 = 0;
  u16 Final_M2 = 0;
  u16 Final_M3 = 0;
  u16 Final_M4 = 0;

  s16 Pitch = 0, Roll = 0, Yaw = 0;

  static s16 MegDataX[8] = {0};
  static s16 MegDataY[8] = {0};
	static u16 Correction_Time = 0;

	/* Time Count */
	SysTick_Cnt++;
	if(SysTick_Cnt == 400) {
		SysTick_Cnt = 0;
		Time_Sec++;
		if(Time_Sec == 60) {	// 0~59
			Time_Min++;
			Time_Sec = 0;
		}
	}

	/* 400Hz 取樣 */
	I2C_DMA_ReadReg(MEMS_Buf,    L3G4200_I2C_ADDR, L3G4200_MULTIPLE, 6);
	I2C_DMA_ReadReg(MEMS_Buf+6,  LSM303A_I2C_ADDR, LSM303A_MULTIPLE, 6);
	I2C_DMA_ReadReg(MEMS_Buf+12, LSM303M_I2C_ADDR, LSM303M_MULTIPLE, 6);

	/* 資料合併 */
	Gyr.X = (s16)((MEMS_Buf[1]  << 8) | MEMS_Buf[0]);
	Gyr.Y = (s16)((MEMS_Buf[3]  << 8) | MEMS_Buf[2]);
	Gyr.Z = (s16)((MEMS_Buf[5]  << 8) | MEMS_Buf[4]);
	Acc.X = (s16)((MEMS_Buf[7]  << 8) | MEMS_Buf[6]);
	Acc.Y = (s16)((MEMS_Buf[9]  << 8) | MEMS_Buf[8]);
	Acc.Z = (s16)((MEMS_Buf[11] << 8) | MEMS_Buf[10]);
	Meg.X = (s16)((MEMS_Buf[12] << 8) | MEMS_Buf[13]);
	Meg.Y = (s16)((MEMS_Buf[14] << 8) | MEMS_Buf[15]);
	Meg.Z = (s16)((MEMS_Buf[18] << 8) | MEMS_Buf[17]);

	/* 偏移校正 Offset */
	Acc.X -= Acc.OffsetX;
	Acc.Y -= Acc.OffsetY;
	Acc.Z -= Acc.OffsetZ;
	Gyr.X -= Gyr.OffsetX;
	Gyr.Y -= Gyr.OffsetY;
	Gyr.Z -= Gyr.OffsetZ;
	Meg.X -= Meg.OffsetX;
	Meg.Y -= Meg.OffsetY;
	Meg.Z -= Meg.OffsetZ;

	switch(SensorMode) {
/************************** 偏移校正模式 **********************************/
		case Mode_OffSet:
LED1 = 0;
			/* 簡單移動平均法 Simple Moving Average */
      Gyr.X = (s16)MoveAve_SMA(Gyr.X, L3G4200_FIFO[0], 250);
      Gyr.Y = (s16)MoveAve_SMA(Gyr.Y, L3G4200_FIFO[1], 250);
      Gyr.Z = (s16)MoveAve_SMA(Gyr.Z, L3G4200_FIFO[2], 250);

			Correction_Time++;	// 等待FIFO填滿空值 or 填滿靜態資料
			if(Correction_Time == 400) {
				Gyr.OffsetX = L3G4200_X_Theoretic + Gyr.X;	// 角速度為 0dps
				Gyr.OffsetY = L3G4200_Y_Theoretic + Gyr.Y;	// 角速度為 0dps
				Gyr.OffsetZ = L3G4200_Z_Theoretic + Gyr.Z;	// 角速度為 0dps

        Acc.TrueX = Acc.X*LSM303A_4mg;      // g/LSB
        Acc.TrueY = Acc.Y*LSM303A_4mg;      // g/LSB
        Acc.TrueZ = Acc.Z*LSM303A_4mg;      // g/LSB

        AngE.Pitch = atan2f(Acc.TrueY, Acc.TrueZ);
        AngE.Roll  = -asinf(Acc.TrueX);

        AngE.Pitch = toDeg(AngE.Pitch);
        AngE.Roll  = toDeg(AngE.Roll);

        Quaternion_ToNumQ(&NumQ, &AngE);

				Correction_Time = 0;
//         SensorMode = Mode_Magnetic;
// LED1 = 1;
SensorMode = Mode_Algorithm;
			}
			break;
/************************** 磁力校正模式 **********************************/
		case Mode_Magnetic:
LED1 = 0;
      Correction_Time++;
      switch((u16)(Correction_Time/600)) {
        case 0:
          LED3 = 1;
          LED2 = 0;
          MegDataX[0] = (s16)MoveAve_WMA(Meg.X, LSM303M_FIFO[0], 100);
          MegDataY[0] = (s16)MoveAve_WMA(Meg.Y, LSM303M_FIFO[1], 100);
          break;
        case 1:
          LED2 = 1;
          LED3 = 0;
          MegDataX[1] = (s16)MoveAve_WMA(Meg.X, LSM303M_FIFO[0], 100);
          MegDataY[1] = (s16)MoveAve_WMA(Meg.Y, LSM303M_FIFO[1], 100);
          break;
        case 2:
          LED3 = 1;
          LED2 = 0;
          MegDataX[2] = (s16)MoveAve_WMA(Meg.X, LSM303M_FIFO[0], 100);
          MegDataY[2] = (s16)MoveAve_WMA(Meg.Y, LSM303M_FIFO[1], 100);
          break;
        case 3:
          LED2 = 1;
          LED3 = 0;
          MegDataX[3] = (s16)MoveAve_WMA(Meg.X, LSM303M_FIFO[0], 100);
          MegDataY[3] = (s16)MoveAve_WMA(Meg.Y, LSM303M_FIFO[1], 100);
          break;
        case 4:
          LED3 = 1;
          LED2 = 0;
          MegDataX[4] = (s16)MoveAve_WMA(Meg.X, LSM303M_FIFO[0], 100);
          MegDataY[4] = (s16)MoveAve_WMA(Meg.Y, LSM303M_FIFO[1], 100);
          break;
        case 5:
          LED2 = 1;
          LED3 = 0;
          MegDataX[5] = (s16)MoveAve_WMA(Meg.X, LSM303M_FIFO[0], 100);
          MegDataY[5] = (s16)MoveAve_WMA(Meg.Y, LSM303M_FIFO[1], 100);
          break;
        case 6:
          LED3 = 1;
          LED2 = 0;
          MegDataX[6] = (s16)MoveAve_WMA(Meg.X, LSM303M_FIFO[0], 100);
          MegDataY[6] = (s16)MoveAve_WMA(Meg.Y, LSM303M_FIFO[1], 100);
          break;
        case 7:
          LED2 = 1;
          LED3 = 0;
          MegDataX[7] = (s16)MoveAve_WMA(Meg.X, LSM303M_FIFO[0], 100);
          MegDataY[7] = (s16)MoveAve_WMA(Meg.Y, LSM303M_FIFO[1], 100);
          break;
        default:
          LED1 = 1;
          LED2 = 1;
          LED3 = 1;
          Correction_Time = 0;
          Sensor_EllipseFitting(Ellipse, MegDataX, MegDataY, 8);
          Meg.OffsetX = Ellipse[1];
          Meg.OffsetY = Ellipse[2];
          SensorMode = Mode_Algorithm;  // 切換至運算模式
          break;
      }

			break;

/************************** 運算模式 **************************************/
		case Mode_Algorithm:

      /* 加權移動平均法 Weighted Moving Average */
      Acc.X = (s16)MoveAve_WMA(Acc.X, LSM303A_FIFO[0], 8);
      Acc.Y = (s16)MoveAve_WMA(Acc.Y, LSM303A_FIFO[1], 8);
      Acc.Z = (s16)MoveAve_WMA(Acc.Z, LSM303A_FIFO[2], 8);
      Gyr.X = (s16)MoveAve_WMA(Gyr.X, L3G4200_FIFO[0], 8);
      Gyr.Y = (s16)MoveAve_WMA(Gyr.Y, L3G4200_FIFO[1], 8);
      Gyr.Z = (s16)MoveAve_WMA(Gyr.Z, L3G4200_FIFO[2], 8);
      Meg.X = (s16)MoveAve_WMA(Meg.X, LSM303M_FIFO[0], 8);
      Meg.Y = (s16)MoveAve_WMA(Meg.Y, LSM303M_FIFO[1], 8);
      Meg.Z = (s16)MoveAve_WMA(Meg.Z, LSM303M_FIFO[2], 8);

			/* 資料轉換 */
			Acc.TrueX = Acc.X*LSM303A_4mg;      // g/LSB
			Acc.TrueY = Acc.Y*LSM303A_4mg;      // g/LSB
			Acc.TrueZ = Acc.Z*LSM303A_4mg;      // g/LSB
			Gyr.TrueX = Gyr.X*L3G4200_s500dps;  // dps/LSB
			Gyr.TrueY = Gyr.Y*L3G4200_s500dps;  // dps/LSB
			Gyr.TrueZ = Gyr.Z*L3G4200_s500dps;  // dps/LSB
			Meg.TrueX = Meg.X*LSM303M_1p3G_XY;  // gauss/LSB
			Meg.TrueY = Meg.Y*LSM303M_1p3G_XY;  // gauss/LSB
			Meg.TrueZ = Meg.Z*LSM303M_1p3G_Z;   // gauss/LSB

      AHRS_Update();

			/* PID */
// 			if(KEYL_U == 0)	{	PID_Roll.Kp += 0.001f;	PID_Pitch.Kp += 0.001f;  }
// 			if(KEYL_L == 0)	{	PID_Roll.Kp -= 0.001f;	PID_Pitch.Kp -= 0.001f;  }
// 			if(KEYL_R == 0)	{	PID_Roll.Ki += 0.0001f;	PID_Pitch.Ki += 0.0001f; }
// 			if(KEYL_D == 0)	{	PID_Roll.Ki -= 0.0001f;	PID_Pitch.Ki -= 0.0001f; }
// 			if(KEYR_R == 0)	{	PID_Roll.Kd += 0.0001f;	PID_Pitch.Kd += 0.0001f; }
// 			if(KEYR_D == 0)	{	PID_Roll.Kd -= 0.0001f;	PID_Pitch.Kd -= 0.0001f; }
// 			if(KEYR_L == 0)	{	PID_Roll.SumErr = 0.0f;	PID_Pitch.SumErr = 0.0f; }

// 			if(KEYL_U == 0)	{	Kp += 0.001f; }
// 			if(KEYL_L == 0)	{	Kp -= 0.001f; }
// 			if(KEYL_R == 0)	{	Ki += 0.00001f; }
// 			if(KEYL_D == 0)	{	Ki -= 0.00001f; }

      PID_Pitch.ZeroErr = (float)((s16)Exp_Pitch/2.5f);
      PID_Roll.ZeroErr  = (float)((s16)Exp_Roll/2.5f);
      PID_Yaw.ZeroErr   = 180.0f+(float)((s16)Exp_Yaw);

      PID_Yaw.Kp = 0.0f;
      PID_Yaw.Ki = 0.0f;
      PID_Yaw.Kd = -0.45f;
      Roll  = (s16)PID_AHRS_Cal(&PID_Roll,  AngE.Roll,  Gyr.TrueX);
      Pitch = (s16)PID_AHRS_Cal(&PID_Pitch, AngE.Pitch, Gyr.TrueY);
      Yaw   = (s16)PID_AHRS_Cal(&PID_Yaw,   AngE.Yaw,   Gyr.TrueZ);

      Yaw = (s16)(PID_Yaw.Kd*Gyr.TrueZ);

      Thr = (s16)Exp_Thr;
			/* Motor Ctrl */
			Final_M1 = PWM_M1 + Thr + Pitch + Roll + Yaw;
			Final_M2 = PWM_M2 + Thr - Pitch + Roll - Yaw;
			Final_M3 = PWM_M3 + Thr - Pitch - Roll + Yaw;
			Final_M4 = PWM_M4 + Thr + Pitch - Roll - Yaw;

//       Tmp_PID_KP = PID_Pitch.ZeroErr*100;
//       Tmp_PID_KI = PID_Roll.ZeroErr*100;
//       Tmp_PID_KD = PID_Yaw.ZeroErr*100;
//       Tmp_PID_Pitch = Thr;

			Tmp_PID_KP = PID_Pitch.Kp*1000;
			Tmp_PID_KI = PID_Pitch.Ki*1000;
			Tmp_PID_KD = PID_Pitch.Kd*1000;
      Tmp_PID_Pitch = Pitch;

// 			Tmp_PID_KP = Kp*1000;
// 			Tmp_PID_KI = Ki*10000;
// 			Tmp_PID_KD = 123;
      Tmp_PID_Pitch = Pitch;
#define NoSignal 1
			if(KEYR_L == 0)
        Motor_Control(PWM_MOTOR_MIN, PWM_MOTOR_MIN, PWM_MOTOR_MIN, PWM_MOTOR_MIN);
      else if(((Time_Sec-fTime_Sec)>NoSignal) || ((Time_Sec-fTime_Sec)<-NoSignal))
        Motor_Control(PWM_MOTOR_MIN, PWM_MOTOR_MIN, PWM_MOTOR_MIN, PWM_MOTOR_MIN);
			else
        Motor_Control(Final_M1, Final_M2, Final_M3, Final_M4);
			break;
	}
}
/*=====================================================================================================*/
/*=====================================================================================================*/
// void TIM5_IRQHandler( void )
// {
// 	static vu8 TIM5_Cnt = 0;

// 	if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) {
// 		TIM5_Cnt++;
// 		if(TIM5_Cnt == 100) {
// 			TIM5_Cnt = 0;
// 			Time_Sec++;
// 			if(Time_Sec == 60)	// 0~59
// 				Time_Sec = 0;
// 		}
// 		TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);
// 	}
// }
/*=====================================================================================================*/
/*=====================================================================================================*/
void DMA1_Stream0_IRQHandler( void )
{
	I2C1_Recv_DMA_IRQ();
}
/*=====================================================================================================*/
/*=====================================================================================================*/
void DMA1_Stream6_IRQHandler( void )
{
	I2C1_Send_DMA_IRQ();
}
/*=====================================================================================================*/
/*=====================================================================================================*/
void HardFault_Handler( void )
{
  while(1);
}
void MemManage_Handler( void )
{
  while(1);
}
void BusFault_Handler( void )
{
  while(1);
}
void UsageFault_Handler( void )
{
  while(1);
}
void SVC_Handler( void )
{
  while(1);
}
void DebugMon_Handler( void )
{
  while(1);
}
void PendSV_Handler( void )
{
  while(1);
}
void NMI_Handler( void )
{
  while(1);
}
/*=====================================================================================================*/
/*=====================================================================================================*/
