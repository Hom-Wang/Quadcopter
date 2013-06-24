/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_adc.h"
#include "stm32f4_usart.h"
#include "stm32f4_spi.h"
#include "stm32f4_sdio.h"
#include "QCopter.h"
#include "module_keyBoard.h"
#include "module_rs232.h"
#include "module_nrf24l01.h"
#include "module_ssd1963.h"
#include "module_xpt2046.h"
#include "system_windows.h"
#include "system_fatfs.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
vu8 Time_Sec = 0, Time_Min = 0;
u16 PWM_Motor = 0;
s16 LSM303A_Data[3] = {0};
s16 L3G4200_Data[3] = {0};
s16 LSM303M_Data[3] = {0};
s16 Angular_Data[3] = {0};
vu8 WaveFormMode = Wave_Angular;

u8 Connect_Test[2] = {0};
u8 ReadSel[ReadTimes] = {0};

vs16 Tmp_PID_KP = 0;
vs16 Tmp_PID_KI = 0;
vs16 Tmp_PID_KD = 0;
vs16 Tmp_PID_Pitch = 0;

PEN_Struct PEN;

/* SD Card Test */
u8 stringcopy( u8 *buff_to, u8 *buff_from )
{
  u8 i=0;
  for(i=0;i<13;i++)
    buff_to[i] = buff_from[i];
  return 1;
}
void SD_Card_Test( void )
{
   u32 i = 0;

  FATFS fatfs;
  DIR dirs;
  FILINFO finfo;
  FIL fsrc;      // file objects
  FRESULT res;   // FatFs function common result code

  u8 buff_filename[9][13];  //最多隻能讀9個文件，保存9個文件名,文件名採用短文件名，最多13個字符

  LED1 = 0;
  f_mount(0, &fatfs);
  res =  f_opendir(&dirs, "0:/Book");
  if(res == FR_OK) {
    while(f_readdir(&dirs, &finfo) == FR_OK) {
      if(finfo.fattrib) {
        if(!finfo.fname[0])    //文件名不為空，如果為空，則表明該目錄下面的文件已經讀完了
          break;
        LED8 = 0;
        res = f_open(&fsrc, finfo.fname, FA_OPEN_EXISTING | FA_READ);
        stringcopy(buff_filename[i], (BYTE*)finfo.fname);
        i++;
  //       res = f_read(&fsrc, &buffer, 50, &br);
        f_close(&fsrc);
      }
    }
  }
  TFT_PutStr(20, 160, "SD_Card File : ", ASCII1608, WHITE, BLACK);
  TFT_PutStr(28, 180, "1. ", ASCII1608, WHITE, BLACK);
  TFT_PutStr(52, 180, buff_filename[0], ASCII1608, WHITE, BLACK);
  TFT_PutStr(28, 200, "2. ", ASCII1608, WHITE, BLACK);
  TFT_PutStr(52, 200, buff_filename[1], ASCII1608, WHITE, BLACK);
  TFT_PutStr(28, 220, "3. ", ASCII1608, WHITE, BLACK);
  TFT_PutStr(52, 220, buff_filename[2], ASCII1608, WHITE, BLACK);
//   TFT_PutStr(20, 220, "4. ", ASCII1608, WHITE, BLACK);
//   TFT_PutStr(44, 220, buff_filename[3], ASCII1608, WHITE, BLACK);
  while(1){
    LED9 = ~LED9;
    Delay_100ms(2);
    if(KEYR_L == 0) {
      LED1 = 1;
      break;
    }
  }
}
/* SD Card Test END */
/*=====================================================================================================*/
/*=====================================================================================================*/
u8 System_Init( void )
{
  u8 Sta = ERROR;

  /* Hardware */
  SystemInit();       // Clock 設定&啟動
  KeyBoard_Config();  // LED&KEY 設定
  nRF24L01_Config();  // nRF 設定
  SSD1963_Config();   // TFT 設定
  XPT2046_Config();   // Touch 設定
  RS232_Config();     // USART 設定

  Delay_10ms(10);
  
  while(Sta == ERROR)  // 檢測nRF
    Sta = nRF_Check();

  /* Software */
  SSD1963_Init();    // TFT 初始化
  Windows_Init();    // Windows 初始化
  Touch_Init(&PEN);  // Touch 初始化
  Delay_10ms(10);

  TFT_PutStr(20, 40, "SDIO SD Fatfs demo", ASCII1608, BLUE, BLACK);
  while(SD_Init()!= SD_OK)
    TFT_PutStr(20, 60, "SD Init Failed!!", ASCII1608, WHITE, BLACK);
  TFT_PutStr(20, 60, "SD Init OK!!", ASCII1608, WHITE, BLACK);
  TFT_PutStr(20, 80, "Capacity :     MB", ASCII1608, WHITE, BLACK);
   TFT_PutNum(108, 80, Type_D, 4, SDCardInfo.CardCapacity>>20, RED, BLACK);
  TFT_PutStr(20, 100, "CardBlockSize : ", ASCII1608, WHITE, BLACK);
  TFT_PutNum(148, 100, Type_D, 4, SDCardInfo.CardBlockSize, RED, BLACK);
  TFT_PutStr(20, 120, "CardType : ", ASCII1608, WHITE, BLACK);
  TFT_PutNum(108, 120, Type_D, 1, SDCardInfo.CardType, RED, BLACK);
  TFT_PutStr(20, 140, "RCA : ", ASCII1608, WHITE, BLACK);
  TFT_PutNum(68, 140, Type_D, 5, SDCardInfo.RCA, RED, BLACK);

  return SUCCESS;
}
/*=====================================================================================================*/
/*=====================================================================================================*/
int main( void )
{
//   u8 i = 0;
  u8 Sta = ERROR;
  u8 FSM_Sta = FSM_Tx;
  u8 Windows_Sel = Windows_CTRL;

  while(Sta == ERROR)  // 等待系統初始化成功.
    Sta = System_Init();

/* SD Card Test */
  SD_Card_Test();
/* SD Card Test END */
// while(1) {
//   if(KEYL_U == 0) LED0 = 0;
//   else LED0 = 1;
//   if(KEYL_D == 0) LED1 = 0;
//   else LED1 = 1;
//   if(KEYL_R == 0) LED2 = 0;
//   else LED2 = 1;
//   if(KEYL_L == 0) LED3 = 0;
//   else LED3 = 1;
//   if(KEYR_U == 0) LED4 = 0;
//   else LED4 = 1;
//   if(KEYR_D == 0) LED5 = 0;
//   else LED5 = 1;
//   if(KEYR_R == 0) LED6 = 0;
//   else LED6 = 1;
//   if(KEYR_L == 0) LED7 = 0;
//   else LED7 = 1;
// }
  while(1) {

    LED9 = ~LED9;
    switch(FSM_Sta) {

/************************** FSM Tx **************************************/
      case FSM_Tx:
        // FSM_Tx
        nRF_TX_Mode();          // 轉換成 Tx Mode
        nRF_Tx_Data(TxBuf[0]);  // Send Data
        // FSM_Tx End
        FSM_Sta = FSM_Rx;
        break;

/************************** FSM Rx **************************************/
      case FSM_Rx:
        // FSM_Rx
         nRF_RX_Mode();  // 轉換成 Rx Mode, 資料接收在中斷裡

        ReadSel[0]      = (u8)(RxBuf[0][0]);
        Connect_Test[0] = (u8)(RxBuf[0][1]);
        LSM303A_Data[0] = (s16)((RxBuf[0][3]<<8)  | RxBuf[0][2]);
        LSM303A_Data[1] = (s16)((RxBuf[0][5]<<8)  | RxBuf[0][4]);
        LSM303A_Data[2] = (s16)((RxBuf[0][7]<<8)  | RxBuf[0][6]);
        L3G4200_Data[0] = (s16)((RxBuf[0][9]<<8)  | RxBuf[0][8]);
        L3G4200_Data[1] = (s16)((RxBuf[0][11]<<8) | RxBuf[0][10]);
        L3G4200_Data[2] = (s16)((RxBuf[0][13]<<8) | RxBuf[0][12]);

        Tmp_PID_KP = (s16)((RxBuf[0][15]<<8) | RxBuf[0][14]);
        Tmp_PID_KI = (s16)((RxBuf[0][17]<<8) | RxBuf[0][16]);
        Tmp_PID_KD = (s16)((RxBuf[0][19]<<8) | RxBuf[0][18]);
        LSM303M_Data[0] = (s16)((RxBuf[0][15]<<8) | RxBuf[0][14]);
        LSM303M_Data[1] = (s16)((RxBuf[0][17]<<8) | RxBuf[0][16]);
        LSM303M_Data[2] = (s16)((RxBuf[0][19]<<8) | RxBuf[0][18]);
        Angular_Data[0] = (s16)((RxBuf[0][21]<<8) | RxBuf[0][20]);
        Angular_Data[1] = (s16)((RxBuf[0][23]<<8) | RxBuf[0][22]);
        Angular_Data[2] = (s16)((RxBuf[0][25]<<8) | RxBuf[0][24]);
        PWM_Motor       = (s16)((RxBuf[0][27]<<8) | RxBuf[0][26]);
        Tmp_PID_Pitch   = (s16)((RxBuf[0][29]<<8) | RxBuf[0][28]);
        Time_Sec        = (u8)(RxBuf[0][30]);
        Time_Min        = (u8)(RxBuf[0][31]);
        // FSM_Rx End
        FSM_Sta = FSM_USART;
        break;

/************************** FSM CTRL **************************************/
      case FSM_CTRL:
        // FSM_CTRL
         KeyBoard_Read(TxBuf[0]);  // 讀取 KeyBoard
        // FSM_CTRL End
        FSM_Sta = FSM_Tx;
        break;

/************************** FSM TFT **************************************/
      case FSM_TFT:
        // FSM_TFT
        if(KEYL_S2 == 0) {
          Windows_Sel++;
          if(Windows_Sel == 8)
            Windows_Sel = 1;
          Windows_Change(Windows_Sel);
        }
        if(KEYL_S1 == 0) {
          Windows_Sel--;
          if(Windows_Sel == 0)
            Windows_Sel = 7;
          Windows_Change(Windows_Sel);
        }
        Windows_Interface(Windows_Sel);
        // FSM_TFT End
        FSM_Sta = FSM_CTRL;
        break;

/************************** FSM USART **************************************/
      case FSM_USART:
        // FSM_USART
//         USART_Print(USART1, &i);
//         i++;
//         if(i==255)
//           i=0;
        switch(WaveFormMode) {
          case Wave_LSM303A:
            RS232_VisualScope(USART1, RxBuf[0]+2, 8);
            break;
          case Wave_L3G4200:
            RS232_VisualScope(USART1, RxBuf[0]+8, 8);
            break;
          case Wave_LSM303M:
            RS232_VisualScope(USART1, RxBuf[0]+14, 8);
            break;
          case Wave_Angular:
            RS232_VisualScope(USART1, RxBuf[0]+20, 8);
            break;
          case Wave_PID:
            RS232_VisualScope(USART1, RxBuf[0]+2, 8);
            break;
          case Wave_SinCos:
            RS232_VisualScope(USART1, RxBuf[0]+2, 8);
            break;
        }
        // FSM_USART End
        FSM_Sta = FSM_TFT;
        break;
    }
  }
}
/*=====================================================================================================*/
/*=====================================================================================================*/
