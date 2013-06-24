/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "module_rs232.h"
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : RS232_Config
**功能 : RS232 設定 & 配置
**輸入 : None
**輸出 : None
**使用 : RS232_Config();
**=====================================================================================================*/
/*=====================================================================================================*/
void RS232_Config( void )
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,  ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9,  GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

 	/* USART1 Tx PA9 */	/* USART1 Rx PA10 */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	USART_InitStruct.USART_BaudRate = 9600;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStruct);
	USART_Cmd(USART1, ENABLE);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : RS232_VisualScope_CRC16
**功能 : VisualScope軟體 CRC
**輸入 : *Array, Len
**輸出 : USART_CRC
**使用 : RS232_CRC16(pWord, Len);
**=====================================================================================================*/
/*=====================================================================================================*/
static u16 RS232_VisualScope_CRC16( u8 *Array, u16 Len )
{
	u16 USART_IX, USART_IY, USART_CRC;

	USART_CRC = 0xffff;
	for(USART_IX=0; USART_IX<Len; USART_IX++) {
		USART_CRC = USART_CRC^(u16)(Array[USART_IX]);
		for(USART_IY=0; USART_IY<=7; USART_IY++) {
			if((USART_CRC&1)!=0)
				USART_CRC = (USART_CRC>>1)^0xA001;
			else
				USART_CRC = USART_CRC>>1;
		}
	}
	return(USART_CRC);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : RS232_VisualScope
**功能 : VisualScope軟體傳資料
**輸入 : *USARTx, *pWord, Len
**輸出 : None
**使用 : RS232_VisualScope(USART1, TxBuf, 8);
**=====================================================================================================*/
/*=====================================================================================================*/
void RS232_VisualScope( USART_TypeDef* USARTx, u8 *pWord, u16 Len )
{
	u8 i = 0;
	u16 Temp = 0;

	Temp = RS232_VisualScope_CRC16(pWord, Len);
	pWord[8] = Temp&0x00ff;
	pWord[9] = (Temp&0xff00)>>8;

	for(i=0; i<10; i++) {
		USART_SendData(USARTx, *pWord);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
		pWord++;
	}
}
/*=====================================================================================================*/
/*=====================================================================================================*/
// 	DMA_DeInit(DMA1_Channel6);
// 	DMA_InitStructure.DMA_PeripheralBaseAddr = USART3_DR_Base;
// 	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)USART3_TxBuf;
// 	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
// 	DMA_InitStructure.DMA_BufferSize = 512;
// 	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
// 	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
// 	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
// 	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
// 	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
// 	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
// 	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
// 	DMA_Init(DMA1_Channel6, &DMA_InitStructure);

// 	DMA_ITConfig(DMA1_Channel6, DMA_IT_TC, ENABLE);
// 	DMA_ITConfig(DMA1_Channel6, DMA_IT_TE, ENABLE);

// 	/* DMA1 Channel5 (triggered by USART1 Rx event) Config */
// 	DMA_DeInit(DMA1_Channel7);
// 	DMA_InitStructure.DMA_PeripheralBaseAddr = USART3_DR_Base;
// 	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)USART3_RxBuf;
// 	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
// 	DMA_InitStructure.DMA_BufferSize = 512;
// 	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
// 	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
// 	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
// 	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
// 	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
// 	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
// 	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
// 	DMA_Init(DMA1_Channel7, &DMA_InitStructure);

// 	DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE);
// 	DMA_ITConfig(DMA1_Channel7, DMA_IT_TE, ENABLE);

// void USART3_Configuration( void )
// {
// 	GPIO_InitTypeDef GPIO_InitStructure;
// 	USART_InitTypeDef USART_InitStructure;

// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);

// 	/* Configure USART1 Rx (PA.10) as input floating */
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);

// 	USART_InitStructure.USART_BaudRate = 9600 ;
// 	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
// 	USART_InitStructure.USART_StopBits = USART_StopBits_1;
// 	USART_InitStructure.USART_Parity = USART_Parity_No ;
// 	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
// 	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
// 	USART_Init(USART3, &USART_InitStructure);
// 	USART_ITConfig(USART3, USART_IT_IDLE , ENABLE);

// 	/* Enable USART3 DMA Rx and TX request */
// 	USART_DMACmd(USART3, USART_DMAReq_Rx | USART_DMAReq_Tx, ENABLE);

// 	/* Enable DMA1 Channel6 */
// 	DMA_Cmd(DMA1_Channel6, ENABLE);
// 	DMA_Cmd(DMA1_Channel7, ENABLE);

// 	/* Enable the USART3 */
// 	USART_Cmd(USART3, ENABLE);
// 	USART_ClearFlag(USART3, USART_FLAG_TC);
// }

// /* RCC配置如下 */
// RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3 |RCC_APB1Periph_SPI2,ENABLE);

// GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
// RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

// /* NVIC配置如下 */
// NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn;
// NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
// NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
// NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
// NVIC_Init(&NVIC_InitStructure);

// /*Enable DMA Channel7 Interrupt */
// NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;
// NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
// NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
// NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
// NVIC_Init(&NVIC_InitStructure);
// NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
// NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
// NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
// NVIC_Init(&NVIC_InitStructure);
