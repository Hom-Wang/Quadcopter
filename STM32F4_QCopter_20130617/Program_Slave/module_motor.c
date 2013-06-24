/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "module_motor.h"
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : Motor_Config
**功能 : 設定PWM輸出訊號
**輸入 : None
**輸出 : None
**使用 : Motor_Config();
**=====================================================================================================*/
/*=====================================================================================================*/
void Motor_Config( void )
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2  | RCC_APB1Periph_TIM3,  ENABLE);

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM2);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM2);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM3);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM3);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM3);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM3);

	/* TIM3 PWM1 PA6 */	/* TIM3 PWM2 PA7 */	/* TIM2 PWM5 PA0 */	/* TIM2 PWM6 PA1 */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	/* TIM3 PWM3 PB0 */	/* TIM3 PWM4 PB1 */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	TIM_DeInit(TIM2);
	TIM_DeInit(TIM3);

/************************** PWM Output **************************************/
	/* 設定 TIM2 TIM3 Time Base */
	TIM_TimeBaseStruct.TIM_Period = (2500-1);										// 週期 = 2.5ms, 400kHz
	TIM_TimeBaseStruct.TIM_Prescaler = (84-1);									// 除頻84 = 1M ( 1us )
	TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;		// 上數
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStruct);
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStruct);

	/* 設定 TIM2 TIM3 OC */
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;							// 配置為 PWM1 模式
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;	// 致能 OC
	TIM_OCInitStruct.TIM_Pulse = PWM_MOTOR_MAX;									// 設置跳變值
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;			// 當計數值小於 PWM_MOTOR_MIN 時為高電平
	TIM_OC1Init(TIM2, &TIM_OCInitStruct);												// 初始化 TIM2 OC1
	TIM_OC2Init(TIM2, &TIM_OCInitStruct);												// 初始化 TIM2 OC2
	TIM_OC1Init(TIM3, &TIM_OCInitStruct);												// 初始化 TIM3 OC1
	TIM_OC2Init(TIM3, &TIM_OCInitStruct);												// 初始化 TIM3 OC2
	TIM_OC3Init(TIM3, &TIM_OCInitStruct);												// 初始化 TIM3 OC3
	TIM_OC4Init(TIM3, &TIM_OCInitStruct);												// 初始化 TIM3 OC4
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);						// 致能 TIM2 OC1 預裝載
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);						// 致能 TIM2 OC2 預裝載
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);						// 致能 TIM3 OC1 預裝載
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);						// 致能 TIM3 OC2 預裝載
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);						// 致能 TIM3 OC3 預裝載
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);						// 致能 TIM3 OC4 預裝載

	/* 啟動 */
	TIM_ARRPreloadConfig(TIM2, ENABLE);													// 致能 TIM2 重載寄存器ARR
	TIM_ARRPreloadConfig(TIM3, ENABLE);													// 致能 TIM3 重載寄存器ARR
	TIM_Cmd(TIM2, ENABLE);																			// 致能 TIM2
	TIM_Cmd(TIM3, ENABLE);																			// 致能 TIM3
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : Motor_Control
**功能 : 無刷馬達控制
**輸入 : Motor1, Motor2, Motor3, Motor4
**輸出 : None
**使用 : Motor_Control( Motor1, Motor2, Motor3, Motor4 );
**=====================================================================================================*/
/*=====================================================================================================*/
void Motor_Control( u16 Motor1, u16 Motor2, u16 Motor3, u16 Motor4 )
{
	if(Motor1>PWM_MOTOR_MAX)	Motor1 = PWM_MOTOR_MAX;
	if(Motor2>PWM_MOTOR_MAX)	Motor2 = PWM_MOTOR_MAX;
  if(Motor3>PWM_MOTOR_MAX)	Motor3 = PWM_MOTOR_MAX;
  if(Motor4>PWM_MOTOR_MAX)	Motor4 = PWM_MOTOR_MAX;
	if(Motor1<PWM_MOTOR_MIN)	Motor1 = PWM_MOTOR_MIN;
	if(Motor2<PWM_MOTOR_MIN)	Motor2 = PWM_MOTOR_MIN;
	if(Motor3<PWM_MOTOR_MIN)	Motor3 = PWM_MOTOR_MIN;
	if(Motor4<PWM_MOTOR_MIN)	Motor4 = PWM_MOTOR_MIN;
	
	PWM_Motor1 = Motor1;
	PWM_Motor2 = Motor2;
	PWM_Motor3 = Motor3;
	PWM_Motor4 = Motor4;
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : Sevro_Control
**功能 : 伺服馬達控制
**輸入 : SevroA, SevroB
**輸出 : None
**使用 : Sevro_Control( SevroA, SevroB );
**=====================================================================================================*/
/*=====================================================================================================*/
void Sevro_Control( u16 SevroA, u16 SevroB )
{
	if(SevroA>PWM_SEVRO_MAX)	SevroA = PWM_SEVRO_MAX;
	if(SevroB>PWM_SEVRO_MAX)	SevroB = PWM_SEVRO_MAX;
	if(SevroA<PWM_SEVRO_MIN)	SevroA = PWM_SEVRO_MIN;
	if(SevroB<PWM_SEVRO_MIN)	SevroB = PWM_SEVRO_MIN;
	
	PWM_SevroA = SevroA;
	PWM_SevroB = SevroB;
}
/*=====================================================================================================*/
/*=====================================================================================================*/
