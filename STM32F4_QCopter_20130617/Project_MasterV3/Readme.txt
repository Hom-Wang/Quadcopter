/*=====================================================================================================*/
/*=====================================================================================================**
* File Name   : Readme.txt
* Author      : Hom19910422
* Version     : V1.2
* Date        : 2012/09/27
* Description : QCopter Ctrl Board Software/Hardware Information
**=====================================================================================================*/
/*=====================================================================================================**
* Hardware
*	MCU       : STM32F407VGT
*	LCD       : 4.3' 272*480 TFT ( SSD1963 )
*	RF        : nRF24L01P+PA+LNA
*	LED       : 10*LED
*	KEY       : 2*Joystick, 8*BigButton, 4*SmallButton
*	
*	Add       : SDCard
**=====================================================================================================*/
/*=====================================================================================================**
* Pin Assign
*	PA0		LED						*	PB0		LED						*	PC0		ADC
*	PA1		LED						*	PB1		LED						*	PC1		ADC
*	PA2		LED						*	PB2		BOOT_BOOT1				*	PC2		ADC
*	PA3		LED						*	PB3		JTAG_JTDO				*	PC3		ADC
*	PA4		NRF_CSN					*	PB4		JTAG_NJTRST				*	PC4		ADC
*	PA5		NRF_SCK					*	PB5		NULL					*	PC5		ADC
*	PA6		NRF_MISO				*	PB6		USART1_TX				*	PC6		LED
*	PA7		NRF_MOSI				*	PB7		USART1_RX				*	PC7		LED
*	PA8		KEY						*	PB8		LED						*	PC8		SDIO_D0
*	PA9		KEY						*	PB9		LED						*	PC9		SDIO_D1
*	PA10	KEY						*	PB10	USART3_TX				*	PC10	SDIO_D2
*	PA11	KEY						*	PB11	USART3_RX				*	PC11	SDIO_D3
*	PA12	TFT_PEN					*	PB12	TFT_TPCS				*	PC12	SDIO_CK
*	PA13	JTAG_JTMS				*	PB13	TFT_SCK					*	PC13	KEY
*	PA14	JTAG_JTCK				*	PB14	TFT_MISO				*	PC14	KEY
*	PA15	JTAG_JTDI				*	PB15	TFT_MOSI				*	PC15	KEY

*	PD0		TFT_D2					*	PE0		KEY
*	PD1		TFT_D3					*	PE1		KEY
*	PD2		SDIO_CMD				*	PE2		KEY
*	PD3		NRF_IRQ					*	PE3		KEY
*	PD4		TFT_RD					*	PE4		KEY
*	PD5		TFT_WR					*	PE5		KEY
*	PD6		NRF_CE					*	PE6		KEY
*	PD7		TFT_CS					*	PE7		TFT_D4
*	PD8		TFT_D13					*	PE8		TFT_D
