/* #include "module_lsm303.h" */

#ifndef __MODULE_LSM303_H
#define __MODULE_LSM303_H

#include "stm32f4xx.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
#define LSM303A_I2C_ADDR				((u8)0x31)
#define LSM303M_I2C_ADDR				((u8)0x3C)

#define LSM303A_CTRL_REG1				((u8)0x20)
#define LSM303A_CTRL_REG2				((u8)0x21)
#define LSM303A_CTRL_REG3				((u8)0x22)
#define LSM303A_CTRL_REG4				((u8)0x23)
#define LSM303A_CTRL_REG5				((u8)0x24)
#define LSM303A_HP_FILTER_RESET	((u8)0x25)
#define LSM303A_REFERENCE				((u8)0x26)
#define LSM303A_STATUS_REG			((u8)0x27)
#define LSM303A_OUT_X_L					((u8)0x28)
#define LSM303A_OUT_X_H					((u8)0x29)
#define LSM303A_OUT_Y_L					((u8)0x2A)
#define LSM303A_OUT_Y_H					((u8)0x2B)
#define LSM303A_OUT_Z_L					((u8)0x2C)
#define LSM303A_OUT_Z_H					((u8)0x2D)
#define LSM303A_INT1_CFG				((u8)0x30)
#define LSM303A_INT1_SOURCE			((u8)0x31)
#define LSM303A_INT1_INT1_THS		((u8)0x32)
#define LSM303A_INT1_DURATION		((u8)0x33)
#define LSM303A_INT2_CFG				((u8)0x34)
#define LSM303A_INT2_SOURCE			((u8)0x35)
#define LSM303A_INT2_THS				((u8)0x36)
#define LSM303A_INT2_DURATION		((u8)0x37)
#define LSM303A_MULTIPLE				((u8)0xA8)	// 連續讀取地址

#define LSM303M_CRA_REG					((u8)0x00)
#define LSM303M_CRB_REG					((u8)0x01)
#define LSM303M_MR_REG					((u8)0x02)
#define LSM303M_OUT_X_H					((u8)0x03)
#define LSM303M_OUT_X_L					((u8)0x04)
#define LSM303M_OUT_Y_H					((u8)0x05)
#define LSM303M_OUT_Y_L					((u8)0x06)
#define LSM303M_OUT_Z_H					((u8)0x07)
#define LSM303M_OUT_Z_L					((u8)0x08)
#define LSM303M_SR_REG					((u8)0x09)
#define LSM303M_IRA_REG					((u8)0x0A)
#define LSM303M_IRB_REG					((u8)0x0B)
#define LSM303M_IRC_REG					((u8)0x0C)
#define LSM303M_MULTIPLE				((u8)0x83)	// 連續讀取地址

#define LSM303A_2mg     ((float)0.00006250f)  // 0.00006250 g/LSB
#define LSM303A_4mg     ((float)0.00012500f)  // 0.00012500 g/LSB
#define LSM303A_8mg     ((float)0.00025000f)  // 0.00025000 g/LSB

#define LSM303M_1p3G_XY ((float)0.00094787f)  // 0.00094787 guess/LSB
#define LSM303M_1p3G_Z  ((float)0.00105263f)  // 0.00105263 guess/LSB
#define LSM303M_1p9G_XY ((float)0.00125786f)  // 0.00125786 guess/LSB
#define LSM303M_1p9G_Z  ((float)0.00140845f)  // 0.00140845 guess/LSB
#define LSM303M_2p5G_XY ((float)0.00157480f)  // 0.00157480 guess/LSB
#define LSM303M_2p5G_Z  ((float)0.00175439f)  // 0.00175439 guess/LSB
#define LSM303M_4p0G_XY ((float)0.00232558f)  // 0.00232558 guess/LSB
#define LSM303M_4p0G_Z  ((float)0.00259740f)  // 0.00259740 guess/LSB
#define LSM303M_4p7G_XY ((float)0.00266667f)  // 0.00266667 guess/LSB
#define LSM303M_4p7G_Z  ((float)0.00298507f)  // 0.00298507 guess/LSB
#define LSM303M_5p6G_XY ((float)0.00312500f)  // 0.00312500 guess/LSB
#define LSM303M_5p6G_Z  ((float)0.00350877f)  // 0.00350877 guess/LSB
#define LSM303M_8p1G_XY ((float)0.00434783f)  // 0.00434783 guess/LSB
#define LSM303M_8p1G_Z  ((float)0.00487805f)  // 0.00487805 guess/LSB
/*=====================================================================================================*/
/*=====================================================================================================*/
void LSM303A_Init( void );
void LSM303M_Init( void );
// void LSM303A_Read_Multiple( void );
// void LSM303M_Read_Multiple( void );
/*=====================================================================================================*/
/*=====================================================================================================*/
#endif
