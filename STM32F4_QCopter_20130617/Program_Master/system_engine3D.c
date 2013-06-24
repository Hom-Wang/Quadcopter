/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "module_ssd1963.h"
#include "algorithm_mathUnit.h"
#include "system_engine3D.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
void Engine3D_RotateX( s16 Angle, s16 *Coordinate )
{
	s16 Temp[2] = {0};
	float TempSin = 0.0f;
	float TempCos = 0.0f;

	TempSin = sinf(toRad(Angle));
	TempCos = cosf(toRad(Angle));

	Temp[0] = (s16)((float)Coordinate[1]*TempCos - (float)Coordinate[2]*TempSin+0.5f);
	Temp[1] = (s16)((float)Coordinate[2]*TempCos + (float)Coordinate[1]*TempSin+0.5f);

	Coordinate[1] = Temp[0];
	Coordinate[2] = Temp[1];
}
/*=====================================================================================================*/
/*=====================================================================================================*/
void Engine3D_RotateY( s16 Angle, s16 *Coordinate )
{
	s16 Temp[2] = {0};
	float TempSin = 0.0f;
	float TempCos = 0.0f;

	TempSin = sinf(toRad(Angle));
	TempCos = cosf(toRad(Angle));

	Temp[0] = (s16)((float)Coordinate[0]*TempCos + (float)Coordinate[2]*TempSin + 0.5f);
	Temp[1] = (s16)((float)Coordinate[2]*TempCos - (float)Coordinate[0]*TempSin + 0.5f);

	Coordinate[0] = Temp[0];
	Coordinate[2] = Temp[1];
}
/*=====================================================================================================*/
/*=====================================================================================================*/
void Engine3D_RotateZ( s16 Angle, s16 *Coordinate )
{
	s16 Temp[2] = {0};
	float TempSin = 0.0f;
	float TempCos = 0.0f;

	TempSin = sinf(toRad(Angle));
	TempCos = cosf(toRad(Angle));

	Temp[0] = (s16)((float)Coordinate[0]*TempCos - (float)Coordinate[1]*TempSin + 0.5f);
	Temp[1] = (s16)((float)Coordinate[1]*TempCos + (float)Coordinate[0]*TempSin + 0.5f);

	Coordinate[0] = Temp[0];
	Coordinate[1] = Temp[1];
}
/*=====================================================================================================*/
/*=====================================================================================================*/
