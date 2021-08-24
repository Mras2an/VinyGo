
#ifndef CODE_HARDWAREV2_SRC_APP_HEATER_HEATER_H_
#define CODE_HARDWAREV2_SRC_APP_HEATER_HEATER_H_


void Heater_init();
void Heater_setPotvalue(int value);
//void Heater_saveValue(int value);
int Heater_getValue(void);
//float Heater_readValue(void);
void Heater_increaseValue(void);
void Heater_increaseValueAndNoStart(void);
void Heater_decreaseValueAndNoStart(void);
void Heater_getValueForLcd(char * buffer);
void Heater_decreaseValue(void);
void Heater_setValue(int value);
void Heater_startHeating(void);
void Heater_stopHeating(void);

#endif /* CODE_HARDWAREV2_SRC_APP_HEATER_HEATER_H_ */
