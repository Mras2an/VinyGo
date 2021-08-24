#ifndef _ESP32_PWM_H_
#define _ESP32_PWM_H_

void MotorHandling_init(void);

void MotorHandling_stop();

void MotorHandling_start(int freq, int direction);

void MotorHandling_startServo(int freq, float duty);

void MotorHandling_hookInit(double freq, int direction);

void MotorHandling_hookSetFreqAndDir(double freq, int direction);

void MotorHandling_hookStop(void);

int MotorHandling_getDirection();

int MotorHandling_getFreq();

#endif /* _ESP32_PWM_H_ */
