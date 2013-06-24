/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "module_sensor.h"
#include "module_keyBoard.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
Sensor Acc = {0};
Sensor Gyr = {0};
Sensor Meg = {0};
Sensor Ang = {0};
float Ellipse[5] = {0};
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : 
**功能 : 
**輸入 : 
**輸出 : 
**使用 : 
**=====================================================================================================*/
/*=====================================================================================================*/
void Sensor_Init( void )
{
	Acc.X = 0;
	Acc.Y = 0;
	Acc.Z = 0;
  Acc.OffsetX = +540;
  Acc.OffsetY = -32;
  Acc.OffsetZ = +427;
 	Acc.TrueX = 0.0f;
	Acc.TrueY = 0.0f;
	Acc.TrueZ = 0.0f;

	Gyr.X = 0;
	Gyr.Y = 0;
	Gyr.Z = 0;
  Gyr.OffsetX = 0;
  Gyr.OffsetY = 0;
  Gyr.OffsetZ = 0;
 	Gyr.TrueX = 0.0f;
	Gyr.TrueY = 0.0f;
	Gyr.TrueZ = 0.0f;

	Meg.X = 0;
	Meg.Y = 0;
	Meg.Z = 0;
  Meg.OffsetX = 0;
  Meg.OffsetY = 0;
  Meg.OffsetX = 0;
  Meg.OffsetY = 0;
  Meg.OffsetZ = 0;
 	Meg.TrueX = 0.0f;
	Meg.TrueY = 0.0f;
	Meg.TrueZ = 0.0f;

	Ang.X = 0;
	Ang.Y = 0;
	Ang.Z = 0;
  Ang.OffsetX = 0;
  Ang.OffsetY = 0;
  Ang.OffsetZ = 0;
 	Ang.TrueX = 0.0f;
	Ang.TrueY = 0.0f;
	Ang.TrueZ = 0.0f;
}
/*=====================================================================================================*/
/*=====================================================================================================*/
void Sensor_EllipseFitting( float* Ans, s16* MegDataX, s16* MegDataY, u8 Num )
{
  s8 i, j, k;
  float temp, temp1, temp2;
  float tempArrX[8] = {0};
  float tempArrY[8] = {0};

  float MEG_X1Y0 = 0.0f;
  float MEG_X2Y0 = 0.0f;
  float MEG_X3Y0 = 0.0f;
  float MEG_X0Y1 = 0.0f;
  float MEG_X0Y2 = 0.0f;
  float MEG_X0Y3 = 0.0f;
  float MEG_X0Y4 = 0.0f;
  float MEG_X1Y1 = 0.0f;
  float MEG_X2Y1 = 0.0f;
  float MEG_X1Y2 = 0.0f;
  float MEG_X1Y3 = 0.0f;
  float MEG_X2Y2 = 0.0f;
  float MEG_X3Y1 = 0.0f;

  float MegArr[5][6] = {0};

  for(i=0; i<Num; i++) {
    tempArrX[i] = (float)MegDataX[i]/1000.0f;
    tempArrY[i] = (float)MegDataY[i]/1000.0f;
  }

  for(i=0; i<Num; i++) {
    MEG_X1Y0 += tempArrX[i];
    MEG_X2Y0 += tempArrX[i]*tempArrX[i];
    MEG_X3Y0 += tempArrX[i]*tempArrX[i]*tempArrX[i];
    MEG_X0Y1 += tempArrY[i];
    MEG_X0Y2 += tempArrY[i]*tempArrY[i];
    MEG_X0Y3 += tempArrY[i]*tempArrY[i]*tempArrY[i];
    MEG_X0Y4 += tempArrY[i]*tempArrY[i]*tempArrY[i]*tempArrY[i];
    MEG_X1Y1 += tempArrX[i]*tempArrY[i];
    MEG_X2Y1 += tempArrX[i]*tempArrX[i]*tempArrY[i];
    MEG_X1Y2 += tempArrX[i]*tempArrY[i]*tempArrY[i];
    MEG_X1Y3 += tempArrX[i]*tempArrY[i]*tempArrY[i]*tempArrY[i];
    MEG_X2Y2 += tempArrX[i]*tempArrX[i]*tempArrY[i]*tempArrY[i];
    MEG_X3Y1 += tempArrX[i]*tempArrX[i]*tempArrX[i]*tempArrY[i];
  }

  MegArr[0][0] = MEG_X2Y2;
  MegArr[0][1] = MEG_X1Y3;
  MegArr[0][2] = MEG_X2Y1;
  MegArr[0][3] = MEG_X1Y2;
  MegArr[0][4] = MEG_X1Y1;
  MegArr[0][5] = -MEG_X3Y1;

  MegArr[1][0] = MEG_X1Y3;
  MegArr[1][1] = MEG_X0Y4;
  MegArr[1][2] = MEG_X1Y2;
  MegArr[1][3] = MEG_X0Y3;
  MegArr[1][4] = MEG_X0Y2;
  MegArr[1][5] = -MEG_X2Y2;

  MegArr[2][0] = MEG_X2Y1;
  MegArr[2][1] = MEG_X1Y2;
  MegArr[2][2] = MEG_X2Y0;
  MegArr[2][3] = MEG_X1Y1;
  MegArr[2][4] = MEG_X1Y0;
  MegArr[2][5] = -MEG_X3Y0;

  MegArr[3][0] = MEG_X1Y2;
  MegArr[3][1] = MEG_X0Y3;
  MegArr[3][2] = MEG_X1Y1;
  MegArr[3][3] = MEG_X0Y2;
  MegArr[3][4] = MEG_X0Y1;
  MegArr[3][5] = -MEG_X2Y1;

  MegArr[4][0] = MEG_X1Y1;
  MegArr[4][1] = MEG_X0Y2;
  MegArr[4][2] = MEG_X1Y0;
  MegArr[4][3] = MEG_X0Y1;
  MegArr[4][4] = Num;
  MegArr[4][5] = -MEG_X2Y0;

  for(i=0; i<5; i++)
    for(j=i+1; j<6; j++)
      for(k=5; k>i-1; k--)
        MegArr[j][k] = MegArr[j][k] - MegArr[j][i]/MegArr[i][i]*MegArr[i][k];
  for(i=0; i<5; i++) {
    temp = MegArr[i][i];
    for(j=i; j<6; j++)
      MegArr[i][j] = MegArr[i][j]/temp;
  }
  for(j=4; j>0; j--)
    for(i=0; i<j; i++)
      MegArr[i][5] = MegArr[i][5] - MegArr[i][j]*MegArr[j][5];

  temp = (1.0f-MegArr[1][5])/MegArr[0][5];
  temp1 = temp + sqrtf(temp*temp+1.0f);
  Ans[0] = atanf(temp1);	// Theta

  temp = MegArr[0][5]*MegArr[0][5]-4*MegArr[1][5];
  Ans[1] = (2.0f*MegArr[1][5]*MegArr[2][5]-MegArr[0][5]*MegArr[3][5])/temp;	// X0
  Ans[2] = (2.0f*MegArr[3][5]-MegArr[0][5]*MegArr[2][5])/temp;	            // Y0

  temp = cosf(Ans[0]);
  temp2  = (Ans[1]*Ans[1]+MegArr[0][5]*Ans[1]*Ans[2]+MegArr[1][5]*Ans[2]*Ans[2]-MegArr[4][5])*(temp1*temp1*temp1*temp1-1.0f);
  Ans[3] = temp/sqrtf((MegArr[1][5]*temp1*temp1-1)/temp2);	// a
  Ans[4] = temp/sqrtf((temp1*temp1-MegArr[1][5])/temp2);    // b

  Ans[1] = Ans[1]*1000.0f;
  Ans[2] = Ans[2]*1000.0f;
  Ans[3] = Ans[3]*1000.0f;
  Ans[4] = Ans[4]*1000.0f;
}
/*=====================================================================================================*/
/*=====================================================================================================*/
