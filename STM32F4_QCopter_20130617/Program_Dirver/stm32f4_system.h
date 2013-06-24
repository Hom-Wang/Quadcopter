/* #include "stm32f4_system.h" */

#ifndef __STM32F4_SYSTEM_H
#define __STM32F4_SYSTEM_H

#include "stm32f4xx.h"
#include "arm_math.h"
#include "stm32f4_typedef.h"
#include "stm32f4_delay.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
#define NULL 0

#define Byte16(ByteH, ByteL) ((u16)(((ByteH)<<8) | (ByteL)))
#define Byte8H(ByteH)        ((u8)((ByteH)>>8))
#define Byte8L(ByteL)        ((u8)(ByteL))

// I/O操作定義
#define Bit_Band(Addr, BitNum)  ((Addr & 0xF0000000)+0x2000000+((Addr &0xFFFFF)<<5)+(BitNum<<2))
#define Mem_Addr(Addr)          *((vu16 *)(Addr))
#define Bit_Addr(Addr, BitNum)  Mem_Addr(Bit_Band(Addr, BitNum))

// I/O地址映射
#define GPIOA_ODR_Addr  (GPIOA_BASE+0x14)
#define GPIOB_ODR_Addr  (GPIOB_BASE+0x14)
#define GPIOC_ODR_Addr  (GPIOC_BASE+0x14)
#define GPIOD_ODR_Addr  (GPIOD_BASE+0x14)
#define GPIOE_ODR_Addr  (GPIOE_BASE+0x14)
#define GPIOF_ODR_Addr  (GPIOF_BASE+0x14)
#define GPIOG_ODR_Addr  (GPIOG_BASE+0x14)
#define GPIOA_IDR_Addr  (GPIOA_BASE+0x10)
#define GPIOB_IDR_Addr  (GPIOB_BASE+0x10)
#define GPIOC_IDR_Addr  (GPIOC_BASE+0x10)
#define GPIOD_IDR_Addr  (GPIOD_BASE+0x10)
#define GPIOE_IDR_Addr  (GPIOE_BASE+0x10)
#define GPIOF_IDR_Addr  (GPIOF_BASE+0x10)
#define GPIOG_IDR_Addr  (GPIOG_BASE+0x10)

// I/O Bit操作 Pin值必須小於16
#define PAO(Pin)  Bit_Addr(GPIOA_ODR_Addr, Pin)  // 輸出
#define PAI(Pin)  Bit_Addr(GPIOA_IDR_Addr, Pin)  // 輸入
#define PBO(Pin)  Bit_Addr(GPIOB_ODR_Addr, Pin)  // 輸出
#define PBI(Pin)  Bit_Addr(GPIOB_IDR_Addr, Pin)  // 輸入
#define PCO(Pin)  Bit_Addr(GPIOC_ODR_Addr, Pin)  // 輸出
#define PCI(Pin)  Bit_Addr(GPIOC_IDR_Addr, Pin)  // 輸入
#define PDO(Pin)  Bit_Addr(GPIOD_ODR_Addr, Pin)  // 輸出
#define PDI(Pin)  Bit_Addr(GPIOD_IDR_Addr, Pin)  // 輸入
#define PEO(Pin)  Bit_Addr(GPIOE_ODR_Addr, Pin)  // 輸出
#define PEI(Pin)  Bit_Addr(GPIOE_IDR_Addr, Pin)  // 輸入
#define PFO(Pin)  Bit_Addr(GPIOF_ODR_Addr, Pin)  // 輸出
#define PFI(Pin)  Bit_Addr(GPIOF_IDR_Addr, Pin)  // 輸入
#define PGO(Pin)  Bit_Addr(GPIOG_ODR_Addr, Pin)  // 輸出
#define PGI(Pin)  Bit_Addr(GPIOG_IDR_Addr, Pin)  // 輸入
/*=====================================================================================================*/
/*=====================================================================================================*/
typedef struct {
  u8 Bit0:1;
  u8 Bit1:1;
  u8 Bit2:1;
  u8 Bit3:1;
  u8 Bit4:1;
  u8 Bit5:1;
  u8 Bit6:1;
  u8 Bit7:1;
} BitField;
typedef struct {
  u16 L:8;
  u16 H:8;
}	HL_Struct;
/*=====================================================================================================*/
/*=====================================================================================================*/
#endif
