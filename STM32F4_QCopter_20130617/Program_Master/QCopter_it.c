/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_sdio.h"
#include "QCopter.h"
#include "module_keyBoard.h"
#include "module_nrf24l01.h"
#include "module_xpt2046.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
void SysTick_Handler( void )
{
}
void EXTI3_IRQHandler( void )
{
  if(EXTI_GetITStatus(EXTI_Line3) != RESET) {
//    LED8 = ~LED8;
    nRF_Recv_IRQ();
    EXTI_ClearITPendingBit(EXTI_Line3);
  }
}
void EXTI15_10_IRQHandler( void )
{
//   u8 Times = PEN_TIMES;

  if(EXTI_GetITStatus(EXTI_Line12) != RESET) {
//     LED5 = TFT_PEN;
//     while(Times--);
//     Touch_ReadXY(TouchXY);
    EXTI_ClearITPendingBit(EXTI_Line12);
  }
}
void SDIO_IRQHandler( void )
{
  SD_ProcessIRQSrc();
}
void DMA2_Stream3_IRQHandler( void )
{
  SD_ProcessDMAIRQ();
}
/*=====================================================================================================*/
/*=====================================================================================================*/
void HardFault_Handler( void )
{
  while(1){
    LED0 = ~LED0;
    Delay_1ms(100);
  }
}
void MemManage_Handler( void )
{
  while(1){
    LED1 = ~LED1;
    Delay_1ms(100);
  }
}
void BusFault_Handler( void )
{
  while(1){
    LED2 = ~LED2;
    Delay_1ms(100);
  }
}
void UsageFault_Handler( void )
{
  while(1){
    LED3 = ~LED3;
    Delay_1ms(100);
  }
}
void SVC_Handler( void )
{
  while(1){
    LED4 = ~LED4;
    Delay_1ms(100);
  }
}
void DebugMon_Handler( void )
{
  while(1){
    LED5 = ~LED5;
    Delay_1ms(100);
  }
}
void PendSV_Handler( void )
{
  while(1){
    LED6 = ~LED6;
    Delay_1ms(100);
  }
}
void NMI_Handler( void )
{
  while(1){
    LED7 = ~LED7;
    Delay_1ms(100);
  }
}
/*=====================================================================================================*/
/*=====================================================================================================*/
