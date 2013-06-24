/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_adc.h"
#include "module_keyBoard.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
u16 ADC_AveTr[ADC_Channel] = {0};
s16 Ctrl_Pitch = 0;
s16 Ctrl_Roll = 0;
s16 Ctrl_Yaw = 0;
s16 Ctrl_Thr = 0;

extern vu8 Time_Sec;
extern vu8 Time_Min;
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : LED_Config
**功能 : LED 配置
**輸入 : None
**輸出 : None
**使用 : LED_Config();
**=====================================================================================================*/
/*=====================================================================================================*/
void LED_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* LED Clk Init *************************************************************/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);

  /* LED4 PA0 */  /* LED5 PA1 */  /* LED6 PA2 */  /* LED7 PA3 */
  GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_2  | GPIO_Pin_3;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* LED2 PB0 */  /* LED3 PB1 */  /* LED8 PB8 */  /* LED9 PB9 */
  GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_8  | GPIO_Pin_9;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* LED0 PC6 */  /* LED1 PC7 */
  GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_6  | GPIO_Pin_7;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStruct);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : KEY_Config
**功能 : KEY 配置
**輸入 : None
**輸出 : None
**使用 : KEY_Config();
**=====================================================================================================*/
/*=====================================================================================================*/
void KEY_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* KEY Clk Init *************************************************************/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOE, ENABLE);

  /* KEYR_R PA8 */  /* KEYR_R PA9 */  /* KEYR_D PA10 */  /* KEYR_U PA11 */
  GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* KEYL_S1 PC13 */  /* KEYR_J PC14 */  /* KEYL_S2 PC15 */
  GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* KEYR_S2 PE0 */  /* KEYR_S1 PE1 */  /* KEYL_J PE2 */  /* KEYL_U PE3 */
  /* KEYL_D  PE4 */  /* KEYL_L  PE5 */  /* KEYL_R PE6 */
  GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_2  | GPIO_Pin_3 |
                              GPIO_Pin_4  | GPIO_Pin_5  | GPIO_Pin_6;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOE, &GPIO_InitStruct);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : ADC_Config
**功能 : ADC 配置
**輸入 : None
**輸出 : None
**使用 : ADC_Config();
**=====================================================================================================*/
/*=====================================================================================================*/
void ADC_Config( void )
{
  DMA_InitTypeDef DMA_InitStruct;
  ADC_InitTypeDef ADC_InitStruct;
  ADC_CommonInitTypeDef ADC_CommonInitStruct;
  GPIO_InitTypeDef GPIO_InitStruct;

  /* ADC Clk Init *************************************************************/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

  /* ADC_L_X PC0 */  /* ADC_L_Y PC1 */  /* ADC_R_Z PC2 */
  /* ADC_L_Z PC3 */  /* ADC_R_Y PC4 */  /* ADC_R_X PC5 */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 |
                             GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* ADC DMA Init *************************************************************/
  DMA_InitStruct.DMA_Channel = DMA_Channel_0;
  DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)ADC1_DR_ADDRESS;               // Peripheral address
  DMA_InitStruct.DMA_Memory0BaseAddr = (u32)&ADC_DMA_Buf;                     // Memory address
  DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;                        // Peripheral to Memory
  DMA_InitStruct.DMA_BufferSize = ADC_Sample*ADC_Channel;                     // Memory Buffer Size
  DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;               // Peripheral address 遞增 Disable
  DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;                        // Memory address 遞增 Enable
  DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;    // Peripheral Data Size 16bit
  DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;            // Memory Data Size 16bit
  DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;                                // 循環模式 Enable
  DMA_InitStruct.DMA_Priority = DMA_Priority_High;                            // ADC DMA通道 高優先級
  DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;                         // DMA FIFO Disable
  DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream0, &DMA_InitStruct);
  DMA_Cmd(DMA2_Stream0, ENABLE);

  /* ADC Common Init **********************************************************/
  ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;                       // 獨立模式
  ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div2;                    // 預分頻2
  ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;        // ADC DMA Mode Disable
  ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;   // 轉換延遲時間
  ADC_CommonInit(&ADC_CommonInitStruct);

  /* ADC Init *****************************************************************/
  ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;                         // 解析度 12bit
  ADC_InitStruct.ADC_ScanConvMode = ENABLE;                                   // 掃描模式
  ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;                             // 連續轉換模式
  ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;    // 外部觸發 Disable
  ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;                         // ADC數據右對齊
  ADC_InitStruct.ADC_NbrOfConversion = ADC_Channel;                           // 轉換ADC通道數目
  ADC_Init(ADC1, &ADC_InitStruct);

  /* ADC Regular Config *******************************************************/
  ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_3Cycles);  // ADC_R_X
  ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 2, ADC_SampleTime_3Cycles);  // ADC_R_Y
  ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 3, ADC_SampleTime_3Cycles);  // ADC_R_Z
  ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 4, ADC_SampleTime_3Cycles);  // ADC_L_X
  ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 5, ADC_SampleTime_3Cycles);  // ADC_L_Y
  ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 6, ADC_SampleTime_3Cycles);  // ADC_L_Z

  ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
  ADC_DMACmd(ADC1, ENABLE);
  ADC_ContinuousModeCmd(ADC1, ENABLE);
  ADC_Cmd(ADC1, ENABLE);
  ADC_SoftwareStartConv(ADC1);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : KeyBoard_Config
**功能 : KeyBoard 配置
**輸入 : None
**輸出 : None
**使用 : KeyBoard_Config();
**=====================================================================================================*/
/*=====================================================================================================*/
void KeyBoard_Config( void )
{
  LED_Config();
  KEY_Config();
  ADC_Config();

  LED0 = 1;
  LED1 = 1;
  LED2 = 1;
  LED3 = 1;
  LED4 = 1;
  LED5 = 1;
  LED6 = 1;
  LED7 = 1;
  LED8 = 1;
  LED9 = 1;
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : KeyBoard_Read
**功能 : 讀取鍵盤
**輸入 : KeyBoardBuf
**輸出 : None
**使用 : KeyBoard_Read(KeyBoardBuf);
**=====================================================================================================*/
/*=====================================================================================================*/
void KeyBoard_Read( u8* KeyBoardBuf )
{

  /* ADC 取平均值 */
  ADC_Average(ADC_AveTr);

  // +-90deg
  if(ADC_AveTr[1]>2400)
    Ctrl_Pitch = (s16)(0.085f*(ADC_AveTr[1]-2400));
  else if(ADC_AveTr[1]<1600)
    Ctrl_Pitch = (s16)(0.1f*(ADC_AveTr[1]-1600));
  else
    Ctrl_Pitch = 0;

  // +-90deg
  if(ADC_AveTr[0]>2400)
    Ctrl_Roll = (s16)(0.09f*(ADC_AveTr[0]-2400));
  else if(ADC_AveTr[0]<1400)
    Ctrl_Roll = (s16)(0.064f*(ADC_AveTr[0]-1400));
  else
    Ctrl_Roll = 0;

  // +-90deg
  if(ADC_AveTr[3]>2400)
    Ctrl_Yaw = (s16)(0.05625f*(ADC_AveTr[3]-2400));
  else if(ADC_AveTr[3]<1600)
    Ctrl_Yaw = (s16)(0.0725f*(ADC_AveTr[3]-1600));
  else
    Ctrl_Yaw = 0;

  // +-400
  if(ADC_AveTr[4]>2400)
    Ctrl_Thr = (s16)(0.25f*(ADC_AveTr[4]-2400));
  else if(ADC_AveTr[4]<1600)
    Ctrl_Thr = (s16)(0.3f*(ADC_AveTr[4]-1600));
  else
    Ctrl_Thr = 0;

  KeyBoardBuf[0]  = (u8)KEYR_J;
  KeyBoardBuf[1]  = (u8)KEYR_U;
  KeyBoardBuf[2]  = (u8)KEYR_D;
  KeyBoardBuf[3]  = (u8)KEYR_L;
  KeyBoardBuf[4]  = (u8)KEYR_R;
  KeyBoardBuf[5]  = (u8)KEYR_S1;
  KeyBoardBuf[6]  = (u8)KEYR_S2;
  KeyBoardBuf[7]  = (u8)KEYL_J;
  KeyBoardBuf[8]  = (u8)KEYL_U;
  KeyBoardBuf[9]  = (u8)KEYL_D;
  KeyBoardBuf[10] = (u8)KEYL_L;
  KeyBoardBuf[11] = (u8)KEYL_R;
  KeyBoardBuf[12] = (u8)KEYL_S1;
  KeyBoardBuf[13] = (u8)KEYL_S2;
  KeyBoardBuf[14] = (u8)Ctrl_Pitch;
  KeyBoardBuf[15] = (u8)(Ctrl_Pitch >> 8);
  KeyBoardBuf[16] = (u8)Ctrl_Roll;
  KeyBoardBuf[17] = (u8)(Ctrl_Roll >> 8);
  KeyBoardBuf[18] = (u8)ADC_AveTr[2];
  KeyBoardBuf[19] = (u8)(ADC_AveTr[2] >> 8);
  KeyBoardBuf[20] = (u8)Ctrl_Yaw;
  KeyBoardBuf[21] = (u8)(Ctrl_Yaw >> 8);
  KeyBoardBuf[22] = (u8)Ctrl_Thr;
  KeyBoardBuf[23] = (u8)(Ctrl_Thr >> 8);
  KeyBoardBuf[24] = (u8)ADC_AveTr[5];
  KeyBoardBuf[25] = (u8)(ADC_AveTr[5] >> 8);
//   KeyBoardBuf[26] = (u8)Ctrl_Pitch;
//   KeyBoardBuf[27] = (u8)(Ctrl_Pitch >> 8);
//   KeyBoardBuf[28] = (u8)Ctrl_Roll;
//   KeyBoardBuf[29] = (u8)(Ctrl_Roll >> 8);
   KeyBoardBuf[30] = (u8)Time_Sec;
   KeyBoardBuf[31] = (u8)Time_Min;
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : LED_Shine
**功能 : LED 閃爍 
**輸入 : LED_Sel, LED_ShineNum, LED_DelayTime
**輸出 : None
**使用 : 
**=====================================================================================================*/
/*=====================================================================================================*/
void LED_Shine( u16 LED_Sel, u16 LED_ShineNum, u16 LED_DelayTime )
{
  while(LED_ShineNum--) {
    LED0 = ((LED_Sel&0x001) == 0x001) ? 0 : 1;
    LED1 = ((LED_Sel&0x002) == 0x002) ? 0 : 1;
    LED2 = ((LED_Sel&0x004) == 0x004) ? 0 : 1;
    LED3 = ((LED_Sel&0x008) == 0x008) ? 0 : 1;
    LED4 = ((LED_Sel&0x010) == 0x010) ? 0 : 1;
    LED5 = ((LED_Sel&0x020) == 0x020) ? 0 : 1;
    LED6 = ((LED_Sel&0x040) == 0x040) ? 0 : 1;
    LED7 = ((LED_Sel&0x080) == 0x080) ? 0 : 1;
    LED8 = ((LED_Sel&0x100) == 0x100) ? 0 : 1;
    LED9 = ((LED_Sel&0x200) == 0x200) ? 0 : 1;
    Delay_1ms(LED_DelayTime);
    LED0 = 1;
    LED1 = 1;
    LED2 = 1;
    LED3 = 1;
    LED4 = 1;
    LED5 = 1;
    LED6 = 1;
    LED7 = 1;
    LED8 = 1;
    LED9 = 1;
    Delay_1ms(LED_DelayTime);
  }
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : LED_Shift
**功能 : LED 移動
**輸入 : LED_Sel, LED_ShiftNum, LED_DelayTime
**輸出 : None
**使用 : 
**=====================================================================================================*/
/*=====================================================================================================*/
void LED_Shift( u16 LED_Sel, u16 LED_ShiftNum, u16 LED_DelayTime )
{
}
/*=====================================================================================================*/
/*=====================================================================================================*/
