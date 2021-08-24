
#ifndef CODE_HARDWAREV2_SRC_APP_HTML_LCD_H_
#define CODE_HARDWAREV2_SRC_APP_HTML_LCD_H_

typedef enum {
    LCD_MENU_MODE_IDL_MENU,
    LCD_MENU_MODE_AUTOMATIC,
    LCD_MENU_MODE_MANUAL,
    LCD_MENU_MODE_SYSTEM,
    LCD_MENU_MODE_SEMIAUTO,
    LCD_MENU_MODE_TESTS,
    LCD_MENU_MODE_UPDATE,
    LCD_MENU_MODE_CALIBRATION,
} LcdMode;

void Lcd_init();
void Lcd_sendOtaInfo(int len, const char * data);
void Lcd_sendTxtCmd(int len, const char * id, const char * data);
void Lcd_sendImgCmd(int len, const char * id, const char * data);
void Lcd_sendWaveForm(int id, int ch, int data);
LcdMode Lcd_getMode();
void Lcd_sendCheckBox(int len, const char * id, const char * data);

#endif /* CODE_HARDWAREV2_SRC_APP_HTML_LCD_H_ */
