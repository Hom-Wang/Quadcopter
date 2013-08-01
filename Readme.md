Quadcopter-V1.0
========
* File Name   : Readme.md
* Author      : Hom19910422
* Version     : v1.0
* Date        : 2013/08/01
* Description : University Project

Wiki
https://github.com/Hom19910422/Quadcopter-V1.0/wiki

飛控板
========
* 控制器　STM32F405RG 64Pin 168MHz DSP FPU
* 感測器　L3G4200 + LSM303
* 無線傳輸　nRF24L01P + PA + LNA
* 6個PWM通道　連接電調、LED、PPM
* 1個USART接口　可擴充GPS模組、藍芽模組、電腦傳輸RS232

遙控器
========
* 控制器　STM32F407VG 100Pin 168MHz DSP FPU
* 顯示器　4.3吋螢幕，480*242，SSD1963
* 無線傳輸　nRF24L01P + PA + LNA
* 儲存紀錄　SD卡，使用SDIO操作
* 1個I2C接口　外擴其他裝置
* 1個UART接口　可擴充GPS模組、藍芽模組、電腦傳輸RS232


目前狀況
========
* 可以達到平衡，但尚無法懸停
