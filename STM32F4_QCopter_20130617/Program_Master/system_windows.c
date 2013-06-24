/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_adc.h"
#include "stm32f4_usart.h"
#include "module_keyBoard.h"
#include "module_ssd1963.h"
#include "module_xpt2046.h"
#include "algorithm_mathUnit.h"
#include "system_windows.h"
#include "system_engine3D.h"
#include "system_waveForm.h"
#include "system_fatfs.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
extern s16 LSM303A_Data[3];
extern s16 L3G4200_Data[3];
extern s16 LSM303M_Data[3];
extern s16 Angular_Data[3];
float MEMS_Angular[3] = {0};
s16 Sita = 0;
u8 PrintTr[16][32] = {0};
u8 Data_PL = 0, Data_PR = 0;
u16 Tmp_Motor_PWM = 0;
extern s16 PID_Ctrl_1;
extern s16 PID_Ctrl_2;
extern s16 PID_Ctrl_3;
extern s16 PID_Ctrl_4;
extern vu8 WaveFormMode;
extern vu8 Time_Sec, Time_Min;
extern u16 PWM_Motor;

extern u8 Connect_Test[2];
extern u16 ADC_AveTr[ADC_Channel];
extern vs16 Tmp_PID_KP;
extern vs16 Tmp_PID_KI;
extern vs16 Tmp_PID_KD;
extern vs16 Tmp_PID_Pitch;
WaveForm_Struct WaveForm;

extern PEN_Struct PEN;

s16 Cube0[8][3] = {0};
s16 Cube1[8][3] = {0};
s16 Cube2[8][3] = {0};
s16 Copter3D[12][3] = {0};

extern s16 Ctrl_Pitch;
extern s16 Ctrl_Roll;
extern s16 Ctrl_Yaw;
extern s16 Ctrl_Thr;

s16 MEG_Max[2] = {0};
s16 MEG_Min[2] = {0};
s16 MEG_Offset[2] = {0};
u32 MEG_SampleCount = 0;
/*=====================================================================================================*/
/*=====================================================================================================*/
void Windows_Init( void )
{
	WaveFormInit(&WaveForm);
	Windows_DrawMenu();
}
/*=====================================================================================================*/
/*=====================================================================================================*/
void Windows_Interface( u8 Windows_Sel )
{
	u16 i = 0;
	u16 Tmp_ADCL_Z = 0;
	u16 CorrectCnt = 0;
	u32 TouchTemp[2] = {0};

	switch(Windows_Sel) {

/************************** FSM CTRL **************************************/
		case Windows_CTRL:		// 主控介面
			/* 馬達 Motor */
			MenuCtrl_Motor();
			/* 連線狀況 Connection */
			MenuCtrl_Connection();
			/* 模式 Mode */
			MenuCtrl_Mode();
			/* 運行時間 WorkTime */
			MenuCtrl_WorkTime();
			/* 電池電量 BatteryPower */
			MenuCtrl_Power();
			break;

/************************** FSM WAVE **************************************/
		case Windows_WAVE:		// 示波器
			if(KEYR_S2==0) {
				Delay_100ms(1);
				WaveFormMode++;
				if(WaveFormMode == 7)
					WaveFormMode = 1;
			}
			if(KEYR_S1==0) {
				Delay_100ms(1);
				WaveFormMode--;
				if(WaveFormMode == 0)
					WaveFormMode = 6;
			}
			if(KEYL_J == 0) {
				Tmp_ADCL_Z = (ADC_AveTr[5]-70)/8+0.5;
				if(Tmp_ADCL_Z>500)
					Tmp_ADCL_Z = 500;
				if(Tmp_ADCL_Z<1)
					Tmp_ADCL_Z = 1;
				WaveForm.Scale[0] = Tmp_ADCL_Z;
				WaveForm.Scale[1] = Tmp_ADCL_Z;
				WaveForm.Scale[2] = Tmp_ADCL_Z;
			}

			switch(WaveFormMode) {
				case Wave_LSM303A:
					WaveForm.Data[0] = LSM303A_Data[0];
					WaveForm.Data[1] = LSM303A_Data[1];
					WaveForm.Data[2] = LSM303A_Data[2];
					WaveForm.Data[3] = 0;
					NumToChar(Type_I, 5, PrintTr[0], LSM303A_Data[0]);
					NumToChar(Type_I, 5, PrintTr[1], LSM303A_Data[1]);
					NumToChar(Type_I, 5, PrintTr[2], LSM303A_Data[2]);
					NumToChar(Type_I, 5, PrintTr[3], 0);
					TFT_PutStr(390, 16, "< LSM303A >", ASCII1608, WHITE, BLACK);
					break;
				case Wave_L3G4200:
					WaveForm.Data[0] = L3G4200_Data[0];
					WaveForm.Data[1] = L3G4200_Data[1];
					WaveForm.Data[2] = L3G4200_Data[2];
					WaveForm.Data[3] = 0;
					NumToChar(Type_I, 5, PrintTr[0], L3G4200_Data[0]);
					NumToChar(Type_I, 5, PrintTr[1], L3G4200_Data[1]);
					NumToChar(Type_I, 5, PrintTr[2], L3G4200_Data[2]);
					NumToChar(Type_I, 5, PrintTr[3], 0);
					TFT_PutStr(390, 16, "< L3G4200 >", ASCII1608, WHITE, BLACK);
					break;
				case Wave_LSM303M:
					WaveForm.Data[0] = LSM303M_Data[0];
					WaveForm.Data[1] = LSM303M_Data[1];
					WaveForm.Data[2] = LSM303M_Data[2];
					WaveForm.Data[3] = 0;
					NumToChar(Type_I, 5, PrintTr[0], LSM303M_Data[0]);
					NumToChar(Type_I, 5, PrintTr[1], LSM303M_Data[1]);
					NumToChar(Type_I, 5, PrintTr[2], LSM303M_Data[2]);
					NumToChar(Type_I, 5, PrintTr[3], 0);
					TFT_PutStr(390, 16, "< LSM303M >", ASCII1608, WHITE, BLACK);
					break;
				case Wave_Angular:
					WaveForm.Data[0] = Angular_Data[0];
					WaveForm.Data[1] = Angular_Data[1];
					WaveForm.Data[2] = Angular_Data[2];
					WaveForm.Data[3] = 0;
					NumToChar(Type_I, 5, PrintTr[0], Angular_Data[0]);
					NumToChar(Type_I, 5, PrintTr[1], Angular_Data[1]);
					NumToChar(Type_I, 5, PrintTr[2], Angular_Data[2]);
// NumToChar(Type_I, 5, PrintTr[10], Ctrl_Pitch);
// NumToChar(Type_I, 5, PrintTr[11], Ctrl_Roll);
// NumToChar(Type_I, 5, PrintTr[12], Ctrl_Yaw);
// NumToChar(Type_I, 5, PrintTr[13], Ctrl_Thr);
NumToChar(Type_I, 5, PrintTr[10], Tmp_PID_KP);
NumToChar(Type_I, 5, PrintTr[11], Tmp_PID_KI);
NumToChar(Type_I, 5, PrintTr[12], Tmp_PID_KD);
NumToChar(Type_I, 5, PrintTr[13], Tmp_PID_Pitch);
// NumToChar(Type_I, 5, PrintTr[14], Ctrl_Pitch);
// NumToChar(Type_I, 5, PrintTr[15], Ctrl_Roll);
					TFT_PutStr(390, 16, "< Angular >", ASCII1608, WHITE, BLACK);
					break;
				case Wave_PID:
					WaveForm.Data[0] = Tmp_PID_KP;
					WaveForm.Data[1] = Tmp_PID_KI;
					WaveForm.Data[2] = Tmp_PID_KD;
					WaveForm.Data[3] = Tmp_PID_Pitch;
					NumToChar(Type_I, 5, PrintTr[0], Tmp_PID_KP);
					NumToChar(Type_I, 5, PrintTr[1], Tmp_PID_KI);
					NumToChar(Type_I, 5, PrintTr[2], Tmp_PID_KD);
					NumToChar(Type_I, 5, PrintTr[3], Tmp_PID_Pitch);
					TFT_PutStr(390, 16, "<   PID   >", ASCII1608, WHITE, BLACK);
					break;
				case Wave_SinCos:
					Sita++;
					if(Sita == 360)	Sita = 0;
					WaveForm.Data[0] = sinf(toRad(Sita))*10000;
					WaveForm.Data[1] = cosf(toRad(Sita))*10000;
					WaveForm.Data[2] = 0;
					WaveForm.Data[3] = 0;
					NumToChar(Type_I, 5, PrintTr[0], sinf(toRad(Sita))*10000);
					NumToChar(Type_I, 5, PrintTr[1], cosf(toRad(Sita))*10000);
					NumToChar(Type_I, 5, PrintTr[2], 0);
					NumToChar(Type_I, 5, PrintTr[3], 0);
					TFT_PutStr(390, 16, "< SIN/COS >", ASCII1608, WHITE, BLACK);
					break;
			}
			WaveFormPrint(&WaveForm);

			NumToChar(Type_D, 5, PrintTr[4], WaveForm.Scale[0]);

			TFT_PutStr(390, 32, "CH1:", ASCII1608, RED, BLACK);
			TFT_PutStr(390, 48, "CH2:", ASCII1608, GREEN, BLACK);
			TFT_PutStr(390, 64, "CH3:", ASCII1608, BLUE, BLACK);
			TFT_PutStr(390, 80, "CH4:", ASCII1608, YELLOW, BLACK);
			TFT_PutStr(390, 96, "Sca: ", ASCII1608, WHITE, BLACK);
			TFT_PutStr(390+32, 32, PrintTr[0], ASCII1608, RED, BLACK);
			TFT_PutStr(390+32, 48, PrintTr[1], ASCII1608, GREEN, BLACK);
			TFT_PutStr(390+32, 64, PrintTr[2], ASCII1608, BLUE, BLACK);
			TFT_PutStr(390+32, 80, PrintTr[3], ASCII1608, YELLOW, BLACK);
			TFT_PutStr(390+40, 96, PrintTr[4], ASCII1608, WHITE, BLACK);

TFT_PutStr(390, 128, "< PID >", ASCII1608, WHITE, BLACK);
TFT_PutStr(390, 144, "Kp :", ASCII1608, RED, BLACK);
TFT_PutStr(390, 160, "Ki :", ASCII1608, GREEN, BLACK);
TFT_PutStr(390, 176, "Kd :", ASCII1608, BLUE, BLACK);
TFT_PutStr(390, 192, "Out:", ASCII1608, YELLOW, BLACK);
// TFT_PutStr(390, 208, "P:", ASCII1608, WHITE, BLACK);
// TFT_PutStr(390, 224, "R:", ASCII1608, WHITE, BLACK);
TFT_PutStr(390+32, 144, PrintTr[10], ASCII1608, RED, BLACK);
TFT_PutStr(390+32, 160, PrintTr[11], ASCII1608, GREEN, BLACK);
TFT_PutStr(390+32, 176, PrintTr[12], ASCII1608, BLUE, BLACK);
TFT_PutStr(390+32, 192, PrintTr[13], ASCII1608, YELLOW, BLACK);
// TFT_PutStr(390+32, 208, PrintTr[14], ASCII1608, YELLOW, BLACK);
// TFT_PutStr(390+32, 224, PrintTr[15], ASCII1608, YELLOW, BLACK);
			break;

/************************** FSM AHRS **************************************/
		case Windows_AHRS:		// 姿態顯示
			#define MEMS_X0	50
			#define MEMS_Y0	90
			#define MEMS_X1	150
			#define MEMS_Y1	90
      #define MEMS_X2	340
			#define MEMS_Y2	150
      #define MEMS_L	40
      #define MEMS_ZL	80

      NumToChar(Type_I, 3, PrintTr[7], MEMS_Angular[0]);
      NumToChar(Type_I, 3, PrintTr[8], MEMS_Angular[1]);
      NumToChar(Type_I, 3, PrintTr[9], MEMS_Angular[2]);
			TFT_PutStr(MEMS_X0-8*6/2, 10, "Axis-X", ASCII1608, WHITE, BLACK);
			TFT_PutStr(MEMS_X1-8*6/2, 10, "Axis-Y", ASCII1608, WHITE, BLACK);
      TFT_PutStr(MEMS_X2-8*6/2, 10, "Axis-Z", ASCII1608, WHITE, BLACK);
      TFT_PutStr(MEMS_X0-8*6/2+8, 30, PrintTr[7], ASCII1608, WHITE, BLACK);
      TFT_PutStr(MEMS_X1-8*6/2+8, 30, PrintTr[8], ASCII1608, WHITE, BLACK);
      TFT_PutStr(MEMS_X2-8*6/2+8, 30, PrintTr[9], ASCII1608, WHITE, BLACK);
			TFT_DrawLine(MEMS_X0, MEMS_Y0, MEMS_X0+MEMS_L*cosf(toRad(MEMS_Angular[0])), MEMS_Y0+MEMS_L*sinf(toRad(MEMS_Angular[0])), BLACK);
			TFT_DrawLine(MEMS_X0-MEMS_L*cosf(toRad(MEMS_Angular[0])), MEMS_Y0-MEMS_L*sinf(toRad(MEMS_Angular[0])), MEMS_X0, MEMS_Y0, BLACK);
			TFT_DrawLine(MEMS_X1, MEMS_Y1, MEMS_X1+MEMS_L*cosf(toRad(MEMS_Angular[1])), MEMS_Y1+MEMS_L*sinf(toRad(MEMS_Angular[1])), BLACK);
			TFT_DrawLine(MEMS_X1-MEMS_L*cosf(toRad(MEMS_Angular[1])), MEMS_Y1-MEMS_L*sinf(toRad(MEMS_Angular[1])), MEMS_X1, MEMS_Y1, BLACK);
      TFT_DrawLine(MEMS_X2, MEMS_Y2, MEMS_X2+MEMS_ZL*cosf(toRad(MEMS_Angular[2])), MEMS_Y2+MEMS_ZL*sinf(toRad(MEMS_Angular[2])), BLACK);
			MEMS_Angular[0] = (float)Angular_Data[0]/100;
			MEMS_Angular[1] = (float)Angular_Data[1]/100;
			MEMS_Angular[2] = (float)Angular_Data[2]/10;
			TFT_DrawLine(MEMS_X0, MEMS_Y0, MEMS_X0+MEMS_L*cosf(toRad(MEMS_Angular[0])), MEMS_Y0+MEMS_L*sinf(toRad(MEMS_Angular[0])), WHITE);
			TFT_DrawLine(MEMS_X0-MEMS_L*cosf(toRad(MEMS_Angular[0])), MEMS_Y0-MEMS_L*sinf(toRad(MEMS_Angular[0])), MEMS_X0, MEMS_Y0, WHITE);
			TFT_DrawLine(MEMS_X1, MEMS_Y1, MEMS_X1+MEMS_L*cosf(toRad(MEMS_Angular[1])), MEMS_Y1+MEMS_L*sinf(toRad(MEMS_Angular[1])), WHITE);
			TFT_DrawLine(MEMS_X1-MEMS_L*cosf(toRad(MEMS_Angular[1])), MEMS_Y1-MEMS_L*sinf(toRad(MEMS_Angular[1])), MEMS_X1, MEMS_Y1, WHITE);
      TFT_DrawLine(MEMS_X2, MEMS_Y2, MEMS_X2+MEMS_ZL*cosf(toRad(MEMS_Angular[2])), MEMS_Y2+MEMS_ZL*sinf(toRad(MEMS_Angular[2])), WHITE);

// 			#define MEMS_X0	120
// 			#define MEMS_Y0	120
// 			#define MEMS_X1	360
// 			#define MEMS_Y1	120
// 			TFT_DrawLine(MEMS_X0+Copter3D[0][0], MEMS_Y0+Copter3D[0][1], MEMS_X0+Copter3D[2][0], MEMS_Y0+Copter3D[2][1], BLACK);
// 			TFT_DrawLine(MEMS_X0+Copter3D[1][0], MEMS_Y0+Copter3D[1][1], MEMS_X0+Copter3D[3][0], MEMS_Y0+Copter3D[3][1], BLACK);
// 			Copter3D[0][0] = 45;
// 			Copter3D[0][1] = 0;
// 			Copter3D[0][2] = 0;
// 			Copter3D[1][0] = 0;
// 			Copter3D[1][1] = 45;
// 			Copter3D[1][2] = 0;
// 			Copter3D[2][0] = -45;
// 			Copter3D[2][1] = 0;
// 			Copter3D[2][2] = 0;
// 			Copter3D[3][0] = 0;
// 			Copter3D[3][1] = -45;
// 			Copter3D[3][2] = 0;
// 			for(i=0; i<4; i++) {
// 				Engine3D_RotateZ(45, Copter3D[i]);
// 				Engine3D_RotateX(MEMS_Angular[0]+60, Copter3D[i]);
// 				Engine3D_RotateY(MEMS_Angular[1], Copter3D[i]);
// 			}
// 			TFT_DrawLine(MEMS_X0+Copter3D[0][0], MEMS_Y0+Copter3D[0][1], MEMS_X0+Copter3D[2][0], MEMS_Y0+Copter3D[2][1], RED);
// 			TFT_DrawLine(MEMS_X0+Copter3D[1][0], MEMS_Y0+Copter3D[1][1], MEMS_X0+Copter3D[3][0], MEMS_Y0+Copter3D[3][1], BLUE);
			break;

/************************** FSM IMAGE *************************************/
		case Windows_IMAGE:		// 相機影像
			TFT_PutStr(240, 120, "<< Windows_IMAGE >>", ASCII1608, WHITE, BLACK);
			break;

/************************** FSM GPS ***************************************/
		case Windows_GPS:			// GPS定位
//			TFT_PutStr(240, 120, "<< Windows_GPS >>", ASCII1608, WHITE, BLACK);
      TFT_DrawLine(10, Window_W/2, Window_L-10, Window_W/2, WHITE);
      TFT_DrawLine(Window_L/2, 10, Window_L/2, Window_W-10, WHITE);
      TFT_DrawLine(10, Window_W/2, Window_L-10, Window_W/2, WHITE);
      TFT_DrawLine(Window_L/2, 10, Window_L/2, Window_W-10, WHITE);
      TFT_DrawLine(10, Window_W/2, Window_L-10, Window_W/2, WHITE);
      TFT_DrawLine(Window_L/2, 10, Window_L/2, Window_W-10, WHITE);
      TFT_DrawLine(10, Window_W/2, Window_L-10, Window_W/2, WHITE);
      TFT_DrawLine(Window_L/2, 10, Window_L/2, Window_W-10, WHITE);
      TFT_DrawLine(10, Window_W/2, Window_L-10, Window_W/2, WHITE);
      TFT_DrawLine(Window_L/2, 10, Window_L/2, Window_W-10, WHITE);
      TFT_DrawLine(10, Window_W/2, Window_L-10, Window_W/2, WHITE);
      TFT_DrawLine(Window_L/2, 10, Window_L/2, Window_W-10, WHITE);
      TFT_DrawLine(10, Window_W/2, Window_L-10, Window_W/2, WHITE);
      TFT_DrawLine(Window_L/2, 10, Window_L/2, Window_W-10, WHITE);
      TFT_DrawPoint((float)LSM303M_Data[0]/50.0f+Window_L/2, (float)LSM303M_Data[1]/50.0f+Window_W/2, GREEN);
//       if(MEG_SampleCount < 1024) {
//         if(LSM303M_Data[0] > MEG_Max[0])
//           MEG_Max[0] = LSM303M_Data[0];
//         if(LSM303M_Data[0] < MEG_Min[0])
//           MEG_Min[0] = LSM303M_Data[0];
//         if(LSM303M_Data[1] > MEG_Max[1])
//           MEG_Max[1] = LSM303M_Data[1];
//         if(LSM303M_Data[1] < MEG_Min[1])
//           MEG_Min[1] = LSM303M_Data[1];

//         MEG_SampleCount++;
//       }
//       else if(MEG_SampleCount == 1024) {
//         MEG_Offset[0] = (MEG_Max[0] + MEG_Min[0])/2.0f;
//         MEG_Offset[1] = (MEG_Max[1] + MEG_Min[1])/2.0f;
//         MEG_SampleCount++;
//       }
//       else {
//         TFT_DrawPoint((float)(LSM303M_Data[0]-MEG_Offset[0])/40.0f+Window_L/2, (float)(LSM303M_Data[1]-MEG_Offset[1])/40.0f+Window_W/2, RED);
//       }
			break;

/************************** FSM TEST1 *************************************/
		case Windows_TEST1:		// 保留
			/* 畫線測試 */
			#define Line_X0	60
			#define Line_Y0	60
			/* 畫直線 */
			TFT_DrawLine(Line_X0, Line_Y0, Line_X0+25, Line_Y0+50, RED);
			TFT_DrawLine(Line_X0-25, Line_Y0-50, Line_X0, Line_Y0, RED);
			TFT_DrawLine(Line_X0, Line_Y0, Line_X0+50, Line_Y0+25, BLUE);
			TFT_DrawLine(Line_X0-50, Line_Y0-25, Line_X0, Line_Y0, BLUE);
			TFT_DrawLine(Line_X0, Line_Y0, Line_X0+25, Line_Y0-50, CYAN);
			TFT_DrawLine(Line_X0-25, Line_Y0+50, Line_X0, Line_Y0, CYAN);
			TFT_DrawLine(Line_X0, Line_Y0, Line_X0+50, Line_Y0-25, MAGENTA);
			TFT_DrawLine(Line_X0-50, Line_Y0+25, Line_X0, Line_Y0, MAGENTA);
			/* 畫矩形 */
			TFT_DrawRectangle(Line_X0-50, Line_Y0-50, 100, 100,		 YELLOW);
			/* 畫圓 */
			TFT_DrawCircle(Line_X0, Line_Y0, 50, GREEN);
			/* 圓周運動 */
			TFT_DrawLine(Line_X0, Line_Y0, Line_X0+50*cosf(toRad(Sita)), Line_Y0+50*sinf(toRad(Sita)), BLACK);
			TFT_DrawLine(Line_X0, Line_Y0, Line_X0, Line_Y0+50*sinf(toRad(Sita)), BLACK);
			TFT_DrawLine(Line_X0, Line_Y0, Line_X0+50*cosf(toRad(Sita)), Line_Y0, BLACK);
			Sita+=5;
			if(Sita == 360)	Sita = 0;
			TFT_DrawLine(Line_X0, Line_Y0, Line_X0+50*cosf(toRad(Sita)), Line_Y0+50*sinf(toRad(Sita)), WHITE);
			TFT_DrawLine(Line_X0, Line_Y0, Line_X0, Line_Y0+50*sinf(toRad(Sita)), WHITE);
			TFT_DrawLine(Line_X0, Line_Y0, Line_X0+50*cosf(toRad(Sita)), Line_Y0, WHITE);

			/* 字體測試 */
			#define CHAR_X0	160
			#define CHAR_Y0	74
			TFT_PutStr(CHAR_X0, CHAR_Y0, "0123456789 ABCDEFG abcdefg", ASCII1206, WHITE, RED);
			TFT_PutStr(CHAR_X0, CHAR_Y0+12, "0123456789 ABCDEFG abcdefg", ASCII1608, WHITE, GREEN);
			TFT_PutStr(CHAR_X0, CHAR_Y0+28, "0123456789 ABCDEFG abcdefg", ASCII1608_, WHITE, BLUE);

			/* ADC 測試 */
			#define ADC_X0	160
			#define ADC_Y0	10
			NumToChar(Type_D, 5, PrintTr[4], ADC_AveTr[0]);
			NumToChar(Type_D, 5, PrintTr[5], ADC_AveTr[1]);
			NumToChar(Type_D, 5, PrintTr[6], ADC_AveTr[2]);
			NumToChar(Type_D, 5, PrintTr[7], ADC_AveTr[3]);
			NumToChar(Type_D, 5, PrintTr[8], ADC_AveTr[4]);
			NumToChar(Type_D, 5, PrintTr[9], ADC_AveTr[5]);
			TFT_PutStr(ADC_X0+80*0, ADC_Y0+16*0, "ADC_L_X : ", ASCII1608, WHITE, BLACK);
			TFT_PutStr(ADC_X0+80*0, ADC_Y0+16*1, "ADC_L_Y : ", ASCII1608, WHITE, BLACK);
			TFT_PutStr(ADC_X0+80*0, ADC_Y0+16*2, "ADC_L_Z : ", ASCII1608, WHITE, BLACK);
			TFT_PutStr(ADC_X0+80*1, ADC_Y0+16*0, PrintTr[7], ASCII1608, WHITE, BLACK);
			TFT_PutStr(ADC_X0+80*1, ADC_Y0+16*1, PrintTr[8], ASCII1608, WHITE, BLACK);
			TFT_PutStr(ADC_X0+80*1, ADC_Y0+16*2, PrintTr[9], ASCII1608, WHITE, BLACK);
			TFT_PutStr(ADC_X0+80*2, ADC_Y0+16*0, "ADC_R_X : ", ASCII1608, WHITE, BLACK);
			TFT_PutStr(ADC_X0+80*2, ADC_Y0+16*1, "ADC_R_Y : ", ASCII1608, WHITE, BLACK);
			TFT_PutStr(ADC_X0+80*2, ADC_Y0+16*2, "ADC_R_Z : ", ASCII1608, WHITE, BLACK);
			TFT_PutStr(ADC_X0+80*3, ADC_Y0+16*0, PrintTr[4], ASCII1608, WHITE, BLACK);
			TFT_PutStr(ADC_X0+80*3, ADC_Y0+16*1, PrintTr[5], ASCII1608, WHITE, BLACK);
			TFT_PutStr(ADC_X0+80*3, ADC_Y0+16*2, PrintTr[6], ASCII1608, WHITE, BLACK);
			
			/* 3D 測試 */
			#define Sp3D_X0	60
			#define Sp3D_Y0	180
			#define Sp3D_X1	160
			#define Sp3D_Y1	180
			#define Sp3D_X2	260
			#define Sp3D_Y2	180
			#define Sp3D_X3	360
			#define Sp3D_Y3	180
			#define Sp3D_R	30
			while(KEYL_L == 0);

			TFT_DrawLine(Sp3D_X0+Cube0[0][0], Sp3D_Y0+Cube0[0][1], Sp3D_X0+Cube0[1][0], Sp3D_Y0+Cube0[1][1], BLACK);
			TFT_DrawLine(Sp3D_X0+Cube0[0][0], Sp3D_Y0+Cube0[0][1], Sp3D_X0+Cube0[2][0], Sp3D_Y0+Cube0[2][1], BLACK);
			TFT_DrawLine(Sp3D_X0+Cube0[0][0], Sp3D_Y0+Cube0[0][1], Sp3D_X0+Cube0[3][0], Sp3D_Y0+Cube0[3][1], BLACK);
			TFT_DrawLine(Sp3D_X0+Cube0[7][0], Sp3D_Y0+Cube0[7][1], Sp3D_X0+Cube0[4][0], Sp3D_Y0+Cube0[4][1], BLACK);
			TFT_DrawLine(Sp3D_X0+Cube0[7][0], Sp3D_Y0+Cube0[7][1], Sp3D_X0+Cube0[5][0], Sp3D_Y0+Cube0[5][1], BLACK);
			TFT_DrawLine(Sp3D_X0+Cube0[7][0], Sp3D_Y0+Cube0[7][1], Sp3D_X0+Cube0[6][0], Sp3D_Y0+Cube0[6][1], BLACK);
			TFT_DrawLine(Sp3D_X0+Cube0[1][0], Sp3D_Y0+Cube0[1][1], Sp3D_X0+Cube0[5][0], Sp3D_Y0+Cube0[5][1], BLACK);
			TFT_DrawLine(Sp3D_X0+Cube0[1][0], Sp3D_Y0+Cube0[1][1], Sp3D_X0+Cube0[6][0], Sp3D_Y0+Cube0[6][1], BLACK);
			TFT_DrawLine(Sp3D_X0+Cube0[2][0], Sp3D_Y0+Cube0[2][1], Sp3D_X0+Cube0[4][0], Sp3D_Y0+Cube0[4][1], BLACK);
			TFT_DrawLine(Sp3D_X0+Cube0[2][0], Sp3D_Y0+Cube0[2][1], Sp3D_X0+Cube0[6][0], Sp3D_Y0+Cube0[6][1], BLACK);
			TFT_DrawLine(Sp3D_X0+Cube0[3][0], Sp3D_Y0+Cube0[3][1], Sp3D_X0+Cube0[5][0], Sp3D_Y0+Cube0[5][1], BLACK);
			TFT_DrawLine(Sp3D_X0+Cube0[3][0], Sp3D_Y0+Cube0[3][1], Sp3D_X0+Cube0[4][0], Sp3D_Y0+Cube0[4][1], BLACK);
			Cube0[0][0] = -Sp3D_R;
			Cube0[0][1] = -Sp3D_R;
			Cube0[0][2] = -Sp3D_R;
			Cube0[1][0] = Sp3D_R;
			Cube0[1][1] = -Sp3D_R;
			Cube0[1][2] = -Sp3D_R;
			Cube0[2][0] = -Sp3D_R;
			Cube0[2][1] = Sp3D_R;
			Cube0[2][2] = -Sp3D_R;
			Cube0[3][0] = -Sp3D_R;
			Cube0[3][1] = -Sp3D_R;
			Cube0[3][2] = Sp3D_R;
			Cube0[4][0] = -Sp3D_R;
			Cube0[4][1] = Sp3D_R;
			Cube0[4][2] = Sp3D_R;
			Cube0[5][0] = Sp3D_R;
			Cube0[5][1] = -Sp3D_R;
			Cube0[5][2] = Sp3D_R;
			Cube0[6][0] = Sp3D_R;
			Cube0[6][1] = Sp3D_R;
			Cube0[6][2] = -Sp3D_R;
			Cube0[7][0] = Sp3D_R;
			Cube0[7][1] = Sp3D_R;
			Cube0[7][2] = Sp3D_R;
			for(i=0; i<8; i++) {
				Engine3D_RotateY(10, Cube0[i]);
				Engine3D_RotateX(Sita, Cube0[i]);
			}
			TFT_DrawLine(Sp3D_X0+Cube0[0][0], Sp3D_Y0+Cube0[0][1], Sp3D_X0+Cube0[1][0], Sp3D_Y0+Cube0[1][1], WHITE);
			TFT_DrawLine(Sp3D_X0+Cube0[0][0], Sp3D_Y0+Cube0[0][1], Sp3D_X0+Cube0[2][0], Sp3D_Y0+Cube0[2][1], WHITE);
			TFT_DrawLine(Sp3D_X0+Cube0[0][0], Sp3D_Y0+Cube0[0][1], Sp3D_X0+Cube0[3][0], Sp3D_Y0+Cube0[3][1], WHITE);
			TFT_DrawLine(Sp3D_X0+Cube0[7][0], Sp3D_Y0+Cube0[7][1], Sp3D_X0+Cube0[4][0], Sp3D_Y0+Cube0[4][1], YELLOW);
			TFT_DrawLine(Sp3D_X0+Cube0[7][0], Sp3D_Y0+Cube0[7][1], Sp3D_X0+Cube0[5][0], Sp3D_Y0+Cube0[5][1], YELLOW);
			TFT_DrawLine(Sp3D_X0+Cube0[7][0], Sp3D_Y0+Cube0[7][1], Sp3D_X0+Cube0[6][0], Sp3D_Y0+Cube0[6][1], YELLOW);
			TFT_DrawLine(Sp3D_X0+Cube0[1][0], Sp3D_Y0+Cube0[1][1], Sp3D_X0+Cube0[5][0], Sp3D_Y0+Cube0[5][1], RED);
			TFT_DrawLine(Sp3D_X0+Cube0[1][0], Sp3D_Y0+Cube0[1][1], Sp3D_X0+Cube0[6][0], Sp3D_Y0+Cube0[6][1], RED);
			TFT_DrawLine(Sp3D_X0+Cube0[2][0], Sp3D_Y0+Cube0[2][1], Sp3D_X0+Cube0[4][0], Sp3D_Y0+Cube0[4][1], BLUE);
			TFT_DrawLine(Sp3D_X0+Cube0[2][0], Sp3D_Y0+Cube0[2][1], Sp3D_X0+Cube0[6][0], Sp3D_Y0+Cube0[6][1], BLUE);
			TFT_DrawLine(Sp3D_X0+Cube0[3][0], Sp3D_Y0+Cube0[3][1], Sp3D_X0+Cube0[5][0], Sp3D_Y0+Cube0[5][1], GREEN);
			TFT_DrawLine(Sp3D_X0+Cube0[3][0], Sp3D_Y0+Cube0[3][1], Sp3D_X0+Cube0[4][0], Sp3D_Y0+Cube0[4][1], GREEN);


			TFT_DrawLine(Sp3D_X1+Cube1[0][0], Sp3D_Y1+Cube1[0][1], Sp3D_X1+Cube1[1][0], Sp3D_Y1+Cube1[1][1], BLACK);
			TFT_DrawLine(Sp3D_X1+Cube1[0][0], Sp3D_Y1+Cube1[0][1], Sp3D_X1+Cube1[2][0], Sp3D_Y1+Cube1[2][1], BLACK);
			TFT_DrawLine(Sp3D_X1+Cube1[0][0], Sp3D_Y1+Cube1[0][1], Sp3D_X1+Cube1[3][0], Sp3D_Y1+Cube1[3][1], BLACK);
			TFT_DrawLine(Sp3D_X1+Cube1[7][0], Sp3D_Y1+Cube1[7][1], Sp3D_X1+Cube1[4][0], Sp3D_Y1+Cube1[4][1], BLACK);
			TFT_DrawLine(Sp3D_X1+Cube1[7][0], Sp3D_Y1+Cube1[7][1], Sp3D_X1+Cube1[5][0], Sp3D_Y1+Cube1[5][1], BLACK);
			TFT_DrawLine(Sp3D_X1+Cube1[7][0], Sp3D_Y1+Cube1[7][1], Sp3D_X1+Cube1[6][0], Sp3D_Y1+Cube1[6][1], BLACK);
			TFT_DrawLine(Sp3D_X1+Cube1[1][0], Sp3D_Y1+Cube1[1][1], Sp3D_X1+Cube1[5][0], Sp3D_Y1+Cube1[5][1], BLACK);
			TFT_DrawLine(Sp3D_X1+Cube1[1][0], Sp3D_Y1+Cube1[1][1], Sp3D_X1+Cube1[6][0], Sp3D_Y1+Cube1[6][1], BLACK);
			TFT_DrawLine(Sp3D_X1+Cube1[2][0], Sp3D_Y1+Cube1[2][1], Sp3D_X1+Cube1[4][0], Sp3D_Y1+Cube1[4][1], BLACK);
			TFT_DrawLine(Sp3D_X1+Cube1[2][0], Sp3D_Y1+Cube1[2][1], Sp3D_X1+Cube1[6][0], Sp3D_Y1+Cube1[6][1], BLACK);
			TFT_DrawLine(Sp3D_X1+Cube1[3][0], Sp3D_Y1+Cube1[3][1], Sp3D_X1+Cube1[5][0], Sp3D_Y1+Cube1[5][1], BLACK);
			TFT_DrawLine(Sp3D_X1+Cube1[3][0], Sp3D_Y1+Cube1[3][1], Sp3D_X1+Cube1[4][0], Sp3D_Y1+Cube1[4][1], BLACK);
			Cube1[0][0] = -Sp3D_R;
			Cube1[0][1] = -Sp3D_R;
			Cube1[0][2] = -Sp3D_R;
			Cube1[1][0] = Sp3D_R;
			Cube1[1][1] = -Sp3D_R;
			Cube1[1][2] = -Sp3D_R;
			Cube1[2][0] = -Sp3D_R;
			Cube1[2][1] = Sp3D_R;
			Cube1[2][2] = -Sp3D_R;
			Cube1[3][0] = -Sp3D_R;
			Cube1[3][1] = -Sp3D_R;
			Cube1[3][2] = Sp3D_R;
			Cube1[4][0] = -Sp3D_R;
			Cube1[4][1] = Sp3D_R;
			Cube1[4][2] = Sp3D_R;
			Cube1[5][0] = Sp3D_R;
			Cube1[5][1] = -Sp3D_R;
			Cube1[5][2] = Sp3D_R;
			Cube1[6][0] = Sp3D_R;
			Cube1[6][1] = Sp3D_R;
			Cube1[6][2] = -Sp3D_R;
			Cube1[7][0] = Sp3D_R;
			Cube1[7][1] = Sp3D_R;
			Cube1[7][2] = Sp3D_R;
			for(i=0; i<8; i++) {
				Engine3D_RotateX(10, Cube1[i]);
				Engine3D_RotateY(Sita, Cube1[i]);
			}
			TFT_DrawLine(Sp3D_X1+Cube1[0][0], Sp3D_Y1+Cube1[0][1], Sp3D_X1+Cube1[1][0], Sp3D_Y1+Cube1[1][1], WHITE);
			TFT_DrawLine(Sp3D_X1+Cube1[0][0], Sp3D_Y1+Cube1[0][1], Sp3D_X1+Cube1[2][0], Sp3D_Y1+Cube1[2][1], WHITE);
			TFT_DrawLine(Sp3D_X1+Cube1[0][0], Sp3D_Y1+Cube1[0][1], Sp3D_X1+Cube1[3][0], Sp3D_Y1+Cube1[3][1], WHITE);
			TFT_DrawLine(Sp3D_X1+Cube1[7][0], Sp3D_Y1+Cube1[7][1], Sp3D_X1+Cube1[4][0], Sp3D_Y1+Cube1[4][1], YELLOW);
			TFT_DrawLine(Sp3D_X1+Cube1[7][0], Sp3D_Y1+Cube1[7][1], Sp3D_X1+Cube1[5][0], Sp3D_Y1+Cube1[5][1], YELLOW);
			TFT_DrawLine(Sp3D_X1+Cube1[7][0], Sp3D_Y1+Cube1[7][1], Sp3D_X1+Cube1[6][0], Sp3D_Y1+Cube1[6][1], YELLOW);
			TFT_DrawLine(Sp3D_X1+Cube1[1][0], Sp3D_Y1+Cube1[1][1], Sp3D_X1+Cube1[5][0], Sp3D_Y1+Cube1[5][1], RED);
			TFT_DrawLine(Sp3D_X1+Cube1[1][0], Sp3D_Y1+Cube1[1][1], Sp3D_X1+Cube1[6][0], Sp3D_Y1+Cube1[6][1], RED);
			TFT_DrawLine(Sp3D_X1+Cube1[2][0], Sp3D_Y1+Cube1[2][1], Sp3D_X1+Cube1[4][0], Sp3D_Y1+Cube1[4][1], BLUE);
			TFT_DrawLine(Sp3D_X1+Cube1[2][0], Sp3D_Y1+Cube1[2][1], Sp3D_X1+Cube1[6][0], Sp3D_Y1+Cube1[6][1], BLUE);
			TFT_DrawLine(Sp3D_X1+Cube1[3][0], Sp3D_Y1+Cube1[3][1], Sp3D_X1+Cube1[5][0], Sp3D_Y1+Cube1[5][1], GREEN);
			TFT_DrawLine(Sp3D_X1+Cube1[3][0], Sp3D_Y1+Cube1[3][1], Sp3D_X1+Cube1[4][0], Sp3D_Y1+Cube1[4][1], GREEN);

			TFT_DrawLine(Sp3D_X2+Cube2[0][0], Sp3D_Y2+Cube2[0][1], Sp3D_X2+Cube2[1][0], Sp3D_Y2+Cube2[1][1], BLACK);
			TFT_DrawLine(Sp3D_X2+Cube2[0][0], Sp3D_Y2+Cube2[0][1], Sp3D_X2+Cube2[2][0], Sp3D_Y2+Cube2[2][1], BLACK);
			TFT_DrawLine(Sp3D_X2+Cube2[0][0], Sp3D_Y2+Cube2[0][1], Sp3D_X2+Cube2[3][0], Sp3D_Y2+Cube2[3][1], BLACK);
			TFT_DrawLine(Sp3D_X2+Cube2[7][0], Sp3D_Y2+Cube2[7][1], Sp3D_X2+Cube2[4][0], Sp3D_Y2+Cube2[4][1], BLACK);
			TFT_DrawLine(Sp3D_X2+Cube2[7][0], Sp3D_Y2+Cube2[7][1], Sp3D_X2+Cube2[5][0], Sp3D_Y2+Cube2[5][1], BLACK);
			TFT_DrawLine(Sp3D_X2+Cube2[7][0], Sp3D_Y2+Cube2[7][1], Sp3D_X2+Cube2[6][0], Sp3D_Y2+Cube2[6][1], BLACK);
			TFT_DrawLine(Sp3D_X2+Cube2[1][0], Sp3D_Y2+Cube2[1][1], Sp3D_X2+Cube2[5][0], Sp3D_Y2+Cube2[5][1], BLACK);
			TFT_DrawLine(Sp3D_X2+Cube2[1][0], Sp3D_Y2+Cube2[1][1], Sp3D_X2+Cube2[6][0], Sp3D_Y2+Cube2[6][1], BLACK);
			TFT_DrawLine(Sp3D_X2+Cube2[2][0], Sp3D_Y2+Cube2[2][1], Sp3D_X2+Cube2[4][0], Sp3D_Y2+Cube2[4][1], BLACK);
			TFT_DrawLine(Sp3D_X2+Cube2[2][0], Sp3D_Y2+Cube2[2][1], Sp3D_X2+Cube2[6][0], Sp3D_Y2+Cube2[6][1], BLACK);
			TFT_DrawLine(Sp3D_X2+Cube2[3][0], Sp3D_Y2+Cube2[3][1], Sp3D_X2+Cube2[5][0], Sp3D_Y2+Cube2[5][1], BLACK);
			TFT_DrawLine(Sp3D_X2+Cube2[3][0], Sp3D_Y2+Cube2[3][1], Sp3D_X2+Cube2[4][0], Sp3D_Y2+Cube2[4][1], BLACK);
			Cube2[0][0] = -Sp3D_R;
			Cube2[0][1] = -Sp3D_R;
			Cube2[0][2] = -Sp3D_R;
			Cube2[1][0] = Sp3D_R;
			Cube2[1][1] = -Sp3D_R;
			Cube2[1][2] = -Sp3D_R;
			Cube2[2][0] = -Sp3D_R;
			Cube2[2][1] = Sp3D_R;
			Cube2[2][2] = -Sp3D_R;
			Cube2[3][0] = -Sp3D_R;
			Cube2[3][1] = -Sp3D_R;
			Cube2[3][2] = Sp3D_R;
			Cube2[4][0] = -Sp3D_R;
			Cube2[4][1] = Sp3D_R;
			Cube2[4][2] = Sp3D_R;
			Cube2[5][0] = Sp3D_R;
			Cube2[5][1] = -Sp3D_R;
			Cube2[5][2] = Sp3D_R;
			Cube2[6][0] = Sp3D_R;
			Cube2[6][1] = Sp3D_R;
			Cube2[6][2] = -Sp3D_R;
			Cube2[7][0] = Sp3D_R;
			Cube2[7][1] = Sp3D_R;
			Cube2[7][2] = Sp3D_R;
			for(i=0; i<8; i++) {
 				Engine3D_RotateX(Sita, Cube2[i]);
				Engine3D_RotateY(Sita, Cube2[i]);
				Engine3D_RotateZ(Sita, Cube2[i]);
			}
			TFT_DrawLine(Sp3D_X2+Cube2[0][0], Sp3D_Y2+Cube2[0][1], Sp3D_X2+Cube2[1][0], Sp3D_Y2+Cube2[1][1], WHITE);
			TFT_DrawLine(Sp3D_X2+Cube2[0][0], Sp3D_Y2+Cube2[0][1], Sp3D_X2+Cube2[2][0], Sp3D_Y2+Cube2[2][1], WHITE);
			TFT_DrawLine(Sp3D_X2+Cube2[0][0], Sp3D_Y2+Cube2[0][1], Sp3D_X2+Cube2[3][0], Sp3D_Y2+Cube2[3][1], WHITE);
			TFT_DrawLine(Sp3D_X2+Cube2[7][0], Sp3D_Y2+Cube2[7][1], Sp3D_X2+Cube2[4][0], Sp3D_Y2+Cube2[4][1], YELLOW);
			TFT_DrawLine(Sp3D_X2+Cube2[7][0], Sp3D_Y2+Cube2[7][1], Sp3D_X2+Cube2[5][0], Sp3D_Y2+Cube2[5][1], YELLOW);
			TFT_DrawLine(Sp3D_X2+Cube2[7][0], Sp3D_Y2+Cube2[7][1], Sp3D_X2+Cube2[6][0], Sp3D_Y2+Cube2[6][1], YELLOW);
			TFT_DrawLine(Sp3D_X2+Cube2[1][0], Sp3D_Y2+Cube2[1][1], Sp3D_X2+Cube2[5][0], Sp3D_Y2+Cube2[5][1], RED);
			TFT_DrawLine(Sp3D_X2+Cube2[1][0], Sp3D_Y2+Cube2[1][1], Sp3D_X2+Cube2[6][0], Sp3D_Y2+Cube2[6][1], RED);
			TFT_DrawLine(Sp3D_X2+Cube2[2][0], Sp3D_Y2+Cube2[2][1], Sp3D_X2+Cube2[4][0], Sp3D_Y2+Cube2[4][1], BLUE);
			TFT_DrawLine(Sp3D_X2+Cube2[2][0], Sp3D_Y2+Cube2[2][1], Sp3D_X2+Cube2[6][0], Sp3D_Y2+Cube2[6][1], BLUE);
			TFT_DrawLine(Sp3D_X2+Cube2[3][0], Sp3D_Y2+Cube2[3][1], Sp3D_X2+Cube2[5][0], Sp3D_Y2+Cube2[5][1], GREEN);
			TFT_DrawLine(Sp3D_X2+Cube2[3][0], Sp3D_Y2+Cube2[3][1], Sp3D_X2+Cube2[4][0], Sp3D_Y2+Cube2[4][1], GREEN);

			TFT_DrawLine(Sp3D_X3+Copter3D[0][0], Sp3D_Y3+Copter3D[0][1], Sp3D_X3+Copter3D[2][0], Sp3D_Y3+Copter3D[2][1], BLACK);
			TFT_DrawLine(Sp3D_X3+Copter3D[1][0], Sp3D_Y3+Copter3D[1][1], Sp3D_X3+Copter3D[3][0], Sp3D_Y3+Copter3D[3][1], BLACK);
			Copter3D[0][0] = Sp3D_R*3/2;
			Copter3D[0][1] = 0;
			Copter3D[0][2] = 0;
			Copter3D[1][0] = 0;
			Copter3D[1][1] = Sp3D_R*3/2;
			Copter3D[1][2] = 0;
			Copter3D[2][0] = -Sp3D_R*3/2;
			Copter3D[2][1] = 0;
			Copter3D[2][2] = 0;
			Copter3D[3][0] = 0;
			Copter3D[3][1] = -Sp3D_R*3/2;
			Copter3D[3][2] = 0;
			for(i=0; i<4; i++) {
				Engine3D_RotateZ(45, Copter3D[i]);
				Engine3D_RotateX(60, Copter3D[i]);
				Engine3D_RotateY(Sita, Copter3D[i]);
			}
			TFT_DrawLine(Sp3D_X3+Copter3D[0][0], Sp3D_Y3+Copter3D[0][1], Sp3D_X3+Copter3D[2][0], Sp3D_Y3+Copter3D[2][1], RED);
			TFT_DrawLine(Sp3D_X3+Copter3D[1][0], Sp3D_Y3+Copter3D[1][1], Sp3D_X3+Copter3D[3][0], Sp3D_Y3+Copter3D[3][1], BLUE);
			break;

/************************** FSM TEST2 *************************************/
		case Windows_TEST2:		// 保留
			/* 校正 CORRECT */
			#define TP_X1 20
			#define TP_X2 460
			#define TP_Y1 20
			#define TP_Y2 220
			#define DIVIDE_X 16300
			#define DIVIDE_Y 16400
			#define TOUCH_TIMES 10
			LED5 = TFT_PEN;

			CorrectCnt = 0;
			TouchTemp[0] = 0;
			TouchTemp[1] = 0;

			while(TFT_PEN==0 && CorrectCnt<TOUCH_TIMES) {
				Touch_ReadXY(&PEN);
				Touch_Transform(&PEN);
				TouchTemp[0] += PEN.ADC_X;
				TouchTemp[1] += PEN.ADC_Y;
				CorrectCnt++;
			}
			if(CorrectCnt == TOUCH_TIMES) {
				PEN.ADC_X = (u16)(TouchTemp[0]/TOUCH_TIMES+0.5f);
				PEN.ADC_Y = (u16)(TouchTemp[1]/TOUCH_TIMES+0.5f);
			}

			switch(PEN.Sta) {
				case 0:
					Touch_CorrectPoint(TP_X1, TP_Y1, WHITE);
					if(TFT_PEN==0 && CorrectCnt==TOUCH_TIMES && PEN.ADC_X>DIVIDE_X && PEN.ADC_Y>DIVIDE_Y) {
							PEN.ADC_XMax = PEN.ADC_X;
							PEN.ADC_YMax = PEN.ADC_Y;
							PEN.Sta = 1;
							Touch_CorrectPoint(TP_X1, TP_Y1, BLACK);
					}
					break;
				case 1:
					Touch_CorrectPoint(TP_X2, TP_Y1, WHITE);
					Touch_ReadXY(&PEN);
					if(TFT_PEN==0 && CorrectCnt==TOUCH_TIMES && PEN.ADC_X<DIVIDE_X && PEN.ADC_Y>DIVIDE_Y) {
						PEN.ADC_XMin = PEN.ADC_X;
						PEN.ADC_YMax = (u16)((PEN.ADC_YMax+PEN.ADC_Y)/2+0.5f);
						PEN.Sta = 2;
						Touch_CorrectPoint(TP_X2, TP_Y1, BLACK);
					}
					break;
				case 2:
					Touch_CorrectPoint(TP_X1, TP_Y2, WHITE);
					Touch_ReadXY(&PEN);
					if(TFT_PEN==0 && CorrectCnt==TOUCH_TIMES && PEN.ADC_X>DIVIDE_X && PEN.ADC_Y<DIVIDE_Y) {
						PEN.ADC_XMax = (u16)((PEN.ADC_XMax+PEN.ADC_X)/2+0.5f);
						PEN.ADC_YMin = PEN.ADC_Y;
						PEN.Sta = 3;
						Touch_CorrectPoint(TP_X1, TP_Y2, BLACK);
					}
					break;
				case 3:
					Touch_CorrectPoint(TP_X2, TP_Y2, WHITE);
					Touch_ReadXY(&PEN);
					if(TFT_PEN==0 && CorrectCnt==TOUCH_TIMES && PEN.ADC_X<DIVIDE_X && PEN.ADC_Y<DIVIDE_Y) {
						PEN.ADC_XMin = (u16)((PEN.ADC_XMin+PEN.ADC_X)/2+0.5f);
						PEN.ADC_YMin = (u16)((PEN.ADC_YMin+PEN.ADC_Y)/2+0.5f);
						PEN.Sta = 4;
						Touch_CorrectPoint(TP_X2, TP_Y2, BLACK);
					}
					break;
				case 4:
					PEN.ADC_XMin = (u16)((TP_X2*PEN.ADC_XMin-TP_X1*PEN.ADC_XMax)/(TP_X2-TP_X1)+0.5);
					PEN.ADC_XMax = (u16)(PEN.ADC_XMin + ((PEN.ADC_XMax-PEN.ADC_XMin)*TFT_L/TP_X2) + 0.5f);
					PEN.ADC_YMin = (u16)((TP_Y2*PEN.ADC_YMin-TP_Y1*PEN.ADC_YMax)/(TP_X2-TP_X1)+0.5);
					PEN.ADC_YMax = (u16)(PEN.ADC_YMin + ((PEN.ADC_YMax-PEN.ADC_YMin)*TFT_W/TP_Y2) + 0.5f);
					PEN.Sta = 5;
					break;
				case 5:
					/* 觸控測試 */
					#define Touch_X0 20
					#define Touch_Y0 20
					NumToChar(Type_D, 5, PrintTr[10], PEN.ADC_X);
					NumToChar(Type_D, 5, PrintTr[11], PEN.ADC_Y);
					TFT_PutStr(Touch_X0,    Touch_Y0+16*0, "Touch X0 : ", ASCII1608, WHITE, BLACK);
					TFT_PutStr(Touch_X0,    Touch_Y0+16*1, "Touch Y0 : ", ASCII1608, WHITE, BLACK);
					TFT_PutStr(Touch_X0+88, Touch_Y0+16*0, PrintTr[10], ASCII1608, WHITE, BLACK);
					TFT_PutStr(Touch_X0+88, Touch_Y0+16*1, PrintTr[11], ASCII1608, WHITE, BLACK);
					NumToChar(Type_D, 5, PrintTr[12], PEN.X);
					NumToChar(Type_D, 5, PrintTr[13], PEN.Y);
					TFT_PutStr(Touch_X0,    Touch_Y0+16*2, "Touch X  : ", ASCII1608, WHITE, BLACK);
					TFT_PutStr(Touch_X0,    Touch_Y0+16*3, "Touch Y  : ", ASCII1608, WHITE, BLACK);
					TFT_PutStr(Touch_X0+88, Touch_Y0+16*2, PrintTr[12], ASCII1608, WHITE, BLACK);
					TFT_PutStr(Touch_X0+88, Touch_Y0+16*3, PrintTr[13], ASCII1608, WHITE, BLACK);
NumToChar(Type_D, 5, PrintTr[10], PEN.ADC_XMax);
NumToChar(Type_D, 5, PrintTr[11], PEN.ADC_XMin);
NumToChar(Type_D, 5, PrintTr[12], PEN.ADC_YMax);
NumToChar(Type_D, 5, PrintTr[13], PEN.ADC_YMin);
TFT_PutStr(Touch_X0,    Touch_Y0+16*5, "Max X  : ", ASCII1608, WHITE, BLACK);
TFT_PutStr(Touch_X0,    Touch_Y0+16*6, "Min X  : ", ASCII1608, WHITE, BLACK);
TFT_PutStr(Touch_X0,    Touch_Y0+16*7, "Max Y  : ", ASCII1608, WHITE, BLACK);
TFT_PutStr(Touch_X0,    Touch_Y0+16*8, "Min Y  : ", ASCII1608, WHITE, BLACK);
TFT_PutStr(Touch_X0+88, Touch_Y0+16*5, PrintTr[10], ASCII1608, WHITE, BLACK);
TFT_PutStr(Touch_X0+88, Touch_Y0+16*6, PrintTr[11], ASCII1608, WHITE, BLACK);
TFT_PutStr(Touch_X0+88, Touch_Y0+16*7, PrintTr[12], ASCII1608, WHITE, BLACK);
TFT_PutStr(Touch_X0+88, Touch_Y0+16*8, PrintTr[13], ASCII1608, WHITE, BLACK);
					if(TFT_PEN==0) {
						if(KEYL_R == 0)
							TFT_DrawPoint(PEN.X, PEN.Y, GREEN);
						else if(KEYL_L == 0)
							TFT_DrawPoint(PEN.X, PEN.Y, BLUE);
						else if(KEYL_D == 0)
							TFT_DrawPoint(PEN.X, PEN.Y, RED);
						else if(KEYL_U == 0)
							TFT_DrawPoint(PEN.X, PEN.Y, BLACK);
						else
							TFT_DrawPoint(PEN.X, PEN.Y, WHITE);
					}
					break;
			}
			break;
	}
}
/*=====================================================================================================*/
/*=====================================================================================================*/
void Windows_DrawButton( u16 CoordiX, u16 CoordiY, u32 Color )
{
	TFT_DrawRectangle(CoordiX, CoordiY, 64, 20, Color);
	TFT_DrawRectangle(CoordiX+1, CoordiY+1, 62, 18, Color);
}
/*=====================================================================================================*/
/*=====================================================================================================*/
void Windows_DrawFull( u32 Color )
{
	TFT_DrawRectangleFull(0, 0, Window_L, Window_W, Color);
}
/*=====================================================================================================*/
/*=====================================================================================================*/
#define SelMenu_1	4
#define SelMenu_2	72
#define SelMenu_3	140
#define SelMenu_4	208
#define SelMenu_5	276
#define SelMenu_6	344
#define SelMenu_7	412
void Windows_DrawMenu( void )
{
	TFT_Clear(BLACK);

	TFT_DrawLineX(0, 248, 479, WHITE);
	TFT_DrawLineX(0, 247, 479, WHITE);

	Windows_DrawButton(SelMenu_1, 251, WHITE);
	TFT_PutStr(20,  254, "CTRL",  ASCII1608, WHITE, BLACK);
	Windows_DrawButton(SelMenu_2, 251, WHITE);
	TFT_PutStr(88,  254, "WAVE",  ASCII1608, WHITE, BLACK);
	Windows_DrawButton(SelMenu_3, 251, WHITE);
	TFT_PutStr(156, 254, "AHRS",  ASCII1608, WHITE, BLACK);
	Windows_DrawButton(SelMenu_4, 251, WHITE);
	TFT_PutStr(220, 254, "IMAGE", ASCII1608, WHITE, BLACK);
	Windows_DrawButton(SelMenu_5, 251, WHITE);
	TFT_PutStr(296, 254, "GPS",   ASCII1608, WHITE, BLACK);
	Windows_DrawButton(SelMenu_6, 251, WHITE);
	TFT_PutStr(356, 254, "TEST1", ASCII1608, WHITE, BLACK);
	Windows_DrawButton(SelMenu_7, 251, WHITE);
	TFT_PutStr(424, 254, "TEST2", ASCII1608, WHITE, BLACK);

	Windows_SelMenu_Init();
	MenuCtrl_Motor_Init();
	MenuCtrl_Motor();
	MenuCtrl_Power_Init();
	MenuCtrl_Power();
	MenuCtrl_Connection_Init();
	MenuCtrl_Connection();
	MenuCtrl_Mode_Init();
	MenuCtrl_Mode();
	MenuCtrl_WorkTime_Init();
	MenuCtrl_WorkTime();
}
/*=====================================================================================================*/
/*=====================================================================================================*/
static void Windows_SelMenu_Init( void )
{
	u8 i;

	for(i=2; i<63; i++) {
		TFT_DrawPoint(SelMenu_1+i, 247, BLACK);
		TFT_DrawPoint(SelMenu_1+i, 248, BLACK);
		TFT_DrawPoint(SelMenu_1+i, 251, BLACK);
		TFT_DrawPoint(SelMenu_1+i, 252, BLACK);
	}
	TFT_DrawPoint(SelMenu_1,    249, WHITE);
	TFT_DrawPoint(SelMenu_1+1,  249, WHITE);
	TFT_DrawPoint(SelMenu_1+63, 249, WHITE);
	TFT_DrawPoint(SelMenu_1+64, 249, WHITE);
	TFT_DrawPoint(SelMenu_1,    250, WHITE);
	TFT_DrawPoint(SelMenu_1+1,  250, WHITE);
	TFT_DrawPoint(SelMenu_1+63, 250, WHITE);
	TFT_DrawPoint(SelMenu_1+64, 250, WHITE);
}
static void Windows_SelMenu( u16 CoordiX, u32 SColor, u32 BColor )
{
	u8 i;
	static u16 SelMenu[2] = {0};
	static u16 Tmp_CoordiX = 4;

	SelMenu[0] = (CoordiX-4)/68;

	for(i=2; i<63; i++) {
		TFT_DrawPoint(Tmp_CoordiX+i, 247, SColor);
		TFT_DrawPoint(Tmp_CoordiX+i, 248, SColor);
		TFT_DrawPoint(Tmp_CoordiX+i, 251, SColor);
		TFT_DrawPoint(Tmp_CoordiX+i, 252, SColor);
		TFT_DrawPoint(CoordiX+i, 247, BColor);
		TFT_DrawPoint(CoordiX+i, 248, BColor);
		TFT_DrawPoint(CoordiX+i, 251, BColor);
		TFT_DrawPoint(CoordiX+i, 252, BColor);
	}
	TFT_DrawPoint(Tmp_CoordiX,    249, BColor);
	TFT_DrawPoint(Tmp_CoordiX+1,  249, BColor);
	TFT_DrawPoint(Tmp_CoordiX+63, 249, BColor);
	TFT_DrawPoint(Tmp_CoordiX+64, 249, BColor);
	TFT_DrawPoint(Tmp_CoordiX,    250, BColor);
	TFT_DrawPoint(Tmp_CoordiX+1,  250, BColor);
	TFT_DrawPoint(Tmp_CoordiX+63, 250, BColor);
	TFT_DrawPoint(Tmp_CoordiX+64, 250, BColor);
	TFT_DrawPoint(CoordiX,    249, SColor);
	TFT_DrawPoint(CoordiX+1,  249, SColor);
	TFT_DrawPoint(CoordiX+63, 249, SColor);
	TFT_DrawPoint(CoordiX+64, 249, SColor);
	TFT_DrawPoint(CoordiX,    250, SColor);
	TFT_DrawPoint(CoordiX+1,  250, SColor);
	TFT_DrawPoint(CoordiX+63, 250, SColor);
	TFT_DrawPoint(CoordiX+64, 250, SColor);

	Tmp_CoordiX = CoordiX;
	SelMenu[1] = SelMenu[0];
}
/*=====================================================================================================*/
/*=====================================================================================================*/
/* 馬達 Motor */
// MAX_LW = {40,221}
#define Motor_X0	440
#define Motor_Y0	25
static void MenuCtrl_Motor_Init( void )
{
	TFT_PutStr(Motor_X0+16, Motor_Y0+205, ".", ASCII1608, WHITE, BLACK);
	TFT_PutStr(Motor_X0+32, Motor_Y0+205, "%", ASCII1608, WHITE, BLACK);
}
static void MenuCtrl_Motor( void )
{
	u8 i = 0;
	u16 Tmp_ADC = 0;

	Tmp_ADC = (u16)((double)(ADC_AveTr[2]-70)/39.5f+0.5f);
	Tmp_Motor_PWM = (u16)((double)(PWM_Motor-800)/1.2f+0.5f);
	Data_PL = (u16)(Tmp_Motor_PWM/10);
	Data_PR = (u16)(Tmp_Motor_PWM%10);

	if(Data_PL >= 100) {
		NumToChar(Type_D, 2, PrintTr[4], 99);
		NumToChar(Type_D, 1, PrintTr[5], 9);
	}
	else {
		NumToChar(Type_D, 2, PrintTr[4], Data_PL);
		NumToChar(Type_D, 1, PrintTr[5], Data_PR);
	}
	TFT_PutStr(Motor_X0, Motor_Y0+205, PrintTr[4], ASCII1608, WHITE, BLACK);
	TFT_PutStr(Motor_X0+24, Motor_Y0+205, PrintTr[5], ASCII1608, WHITE, BLACK);
	for(i=0; i<41; i++) {
		if((i%4)==0) {
			if(i<=(Data_PL*2/5) && i>27)
				TFT_DrawRectangleFull(Motor_X0, Motor_Y0+5*(40-i), 32, 3, 0x470000);
			else if(i<=(Data_PL*2/5))
				TFT_DrawRectangleFull(Motor_X0, Motor_Y0+5*(40-i), 32, 3, GREEN);
			else
				TFT_DrawRectangleFull(Motor_X0, Motor_Y0+5*(40-i), 32, 3, 0x050505);
		}
		else {
			if(i<=(Data_PL*2/5) && i>27)
				TFT_DrawRectangleFull(Motor_X0+12, Motor_Y0+5*(40-i), 20, 3, 0x470000);
			else if(i<=(Data_PL*2/5))
				TFT_DrawRectangleFull(Motor_X0+12, Motor_Y0+5*(40-i), 20, 3, GREEN);
			else
				TFT_DrawRectangleFull(Motor_X0+12, Motor_Y0+5*(40-i), 20, 3, 0x050505);
		}
	}
	for(i=0; i<41; i++) {
		if(i<=(Tmp_ADC*2/5))
			TFT_DrawRectangleFull(475, 25+5*(40-i), 3, 3, RED);
		else
			TFT_DrawRectangleFull(475, 25+5*(40-i), 3, 3, 0x400000);
	}
}
/*=====================================================================================================*/
/*=====================================================================================================*/
/* 連線狀況 Connection */
// MAX_LW = {21,15}
#define Connect_X0	10
#define Connect_Y0	5
static void MenuCtrl_Connection_Init( void )
{
	TFT_DrawRectangleFull(Connect_X0, Connect_Y0+10, 5, 5, WHITE);
	TFT_DrawRectangleFull(Connect_X0+8, Connect_Y0+5, 5, 10, WHITE);
	TFT_DrawRectangleFull(Connect_X0+16, Connect_Y0, 5, 15, WHITE);
}
static void MenuCtrl_Connection( void )
{
	if(Connect_Test[0] == Connect_Test[1]) {
		TFT_DrawRectangleFull(Connect_X0, Connect_Y0+10, 5, 5, WHITE);
		TFT_DrawRectangleFull(Connect_X0+8, Connect_Y0+5, 5, 10, WHITE);
		TFT_DrawRectangleFull(Connect_X0+16, Connect_Y0, 5, 15, WHITE);
	}
	else {
		TFT_DrawRectangleFull(Connect_X0, Connect_Y0+10, 5, 5, GREEN);
		TFT_DrawRectangleFull(Connect_X0+8, Connect_Y0+5, 5, 10, GREEN);
		TFT_DrawRectangleFull(Connect_X0+16, Connect_Y0, 5, 15, GREEN);
	}
	Connect_Test[1] = Connect_Test[0];
}
/*=====================================================================================================*/
/*=====================================================================================================*/
/* 模式 Mode */
// MAX_LW = {104,20}
#define Mode_X0	46
#define Mode_Y0	0
static void MenuCtrl_Mode_Init( void )
{
	TFT_DrawRectangle(Mode_X0, Mode_Y0, 20, 20, WHITE);
	TFT_DrawRectangle(Mode_X0+1, Mode_Y0+1, 18, 18, WHITE);
	TFT_DrawRectangle(Mode_X0+28, Mode_Y0, 20, 20, WHITE);
	TFT_DrawRectangle(Mode_X0+29, Mode_Y0+1, 18, 18, WHITE);
	TFT_DrawRectangle(Mode_X0+56, Mode_Y0, 20, 20, WHITE);
	TFT_DrawRectangle(Mode_X0+57, Mode_Y0+1, 18, 18, WHITE);
	TFT_DrawRectangle(Mode_X0+84, Mode_Y0, 20, 20, WHITE);
	TFT_DrawRectangle(Mode_X0+85, Mode_Y0+1, 18, 18, WHITE);
	TFT_DrawRectangle(Mode_X0+112, Mode_Y0, 20, 20, WHITE);
	TFT_DrawRectangle(Mode_X0+113, Mode_Y0+1, 18, 18, WHITE);
	TFT_PutStr(Mode_X0+7, Mode_Y0+3, "F", ASCII1608, WHITE, BLACK);
	TFT_PutStr(Mode_X0+35, Mode_Y0+3, "A", ASCII1608, WHITE, BLACK);
	TFT_PutStr(Mode_X0+63, Mode_Y0+3, "G", ASCII1608, WHITE, BLACK);
	TFT_PutStr(Mode_X0+91, Mode_Y0+3, "C", ASCII1608, WHITE, BLACK);
	TFT_PutStr(Mode_X0+119, Mode_Y0+3, "L", ASCII1608, WHITE, BLACK);
}
static void MenuCtrl_Mode( void )
{
}
/*=====================================================================================================*/
/*=====================================================================================================*/
/* 運行時間 WorkTime */
// MAX_LW = {40,16}
#define Time_X0	195
#define Time_Y0	4
static void MenuCtrl_WorkTime_Init( void )
{
	TFT_PutStr(Time_X0+16, Time_Y0, ":", ASCII1608, WHITE, BLACK);
}
static void MenuCtrl_WorkTime( void )
{
	u8 PrintTr[2][4];

	NumToChar(Type_D, 2, PrintTr[0], Time_Sec);
	NumToChar(Type_D, 2, PrintTr[1], Time_Min);
	TFT_PutStr(Time_X0, Time_Y0, PrintTr[1], ASCII1608, WHITE, BLACK);
	TFT_PutStr(Time_X0+24, Time_Y0, PrintTr[0], ASCII1608, WHITE, BLACK);
}
/*=====================================================================================================*/
/*=====================================================================================================*/
/* 電池電量 BatteryPower */
#define Power_X0	440
#define Power_Y0	0
static void MenuCtrl_Power_Init( void )
{
	TFT_DrawRectangleFull(Power_X0+3, Power_Y0,   31, 20, WHITE);
	TFT_DrawRectangleFull(Power_X0,   Power_Y0+6,  3,  8, WHITE);
	TFT_DrawRectangleFull(Power_X0+7, Power_Y0+3, 23, 14, BLACK);
}
static void MenuCtrl_Power( void )
{
	TFT_DrawRectangleFull(Power_X0+9,  Power_Y0+5, 5, 10, GREEN);
	TFT_DrawRectangleFull(Power_X0+16, Power_Y0+5, 5, 10, GREEN);
	TFT_DrawRectangleFull(Power_X0+23, Power_Y0+5, 5, 10, GREEN);
}
/*=====================================================================================================*/
/*=====================================================================================================*/
void Windows_Change( u8 Windows_Sel )
{
	switch(Windows_Sel) {
		case Windows_CTRL:
			Windows_DrawFull(BLACK);
			Windows_SelMenu(SelMenu_1, WHITE, BLACK);
			MenuCtrl_Motor_Init();
			MenuCtrl_Connection_Init();
			MenuCtrl_Mode_Init();
			MenuCtrl_WorkTime_Init();
			MenuCtrl_Power_Init();
			break;
		case Windows_WAVE:
			Windows_DrawFull(BLACK);
			Windows_SelMenu(SelMenu_2, WHITE, BLACK);
			break;
		case Windows_AHRS:
			Windows_DrawFull(BLACK);
			Windows_SelMenu(SelMenu_3, WHITE, BLACK);
			break;
		case Windows_IMAGE:
			Windows_DrawFull(RED);
			Windows_SelMenu(SelMenu_4, WHITE, BLACK);
			break;
		case Windows_GPS:
			Windows_DrawFull(BLACK);
			Windows_SelMenu(SelMenu_5, WHITE, BLACK);
			break;
		case Windows_TEST1:
			Windows_DrawFull(BLACK);
			Windows_SelMenu(SelMenu_6, WHITE, BLACK);
			break;
		case Windows_TEST2:
			Windows_DrawFull(BLACK);
			Windows_SelMenu(SelMenu_7, WHITE, BLACK);
			break;
	}
}

/*=====================================================================================================*/
/*=====================================================================================================*/
