#ifndef __LCD_H
#define __LCD_H


#define DISP_HOR_RESOLUTION		480
#define DISP_VER_RESOLUTION		800
#define DISP_HOR_PULSE_WIDTH		1
#define DISP_HOR_BACK_PORCH		210
#define DISP_HOR_FRONT_PORCH	 45
#define DISP_VER_PULSE_WIDTH		1
#define DISP_VER_BACK_PORCH		 34	
#define DISP_VER_FRONT_PORCH	 10

#define LCD_XMAX (479)
#define LCD_YMAX (799)

#define  MAX_X  800
#define  MAX_Y  480  

#define HT (DISP_HOR_RESOLUTION+DISP_HOR_PULSE_WIDTH+DISP_HOR_BACK_PORCH+DISP_HOR_FRONT_PORCH)
#define HPS (DISP_HOR_PULSE_WIDTH+DISP_HOR_BACK_PORCH)
#define VT (DISP_VER_PULSE_WIDTH+DISP_VER_BACK_PORCH+DISP_VER_FRONT_PORCH+DISP_VER_RESOLUTION)
#define VSP (DISP_VER_PULSE_WIDTH+DISP_VER_BACK_PORCH)

/* LCD color */
#define White          0xFFFF
#define Black          0x0000
#define Grey           0xF7DE
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0

#define WHITE          0xFFFF
#define BLACK          0x0000
#define GRAY           0xF7DE
#define BLUE           0x001F
#define BLUE2          0x051F
#define RED            0xF800
#define MAGENTA        0xF81F
#define GREEN          0x07E0
#define CYAN           0x7FFF
#define YELLOW         0xFFE0
#define DGRAY          Grey
#define DCYAN          CYAN
#define PURPLE         MAGENTA
#define DGREEN         0x07E0

void LCD_Initializtion(void);
void LCD_Clear(uint16_t Color);	

void LCD_DrawLine( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color );
static void LCD_SetCursor( uint16_t x, uint16_t y );
void GUI_Text(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint16_t Color, uint16_t bkColor);

#endif
