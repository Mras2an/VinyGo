
#ifndef CODE_HARDWAREV2_SRC_APP_CALIBRATION_MODESEMI_H_
#define CODE_HARDWAREV2_SRC_APP_CALIBRATION_MODESEMI_H_

void ModeCalibration_init();

void ModeCalibration_cmdReceived(unsigned int type, unsigned int action, unsigned int len, unsigned char * data);

unsigned int ModeCalibration_getValForSevenP();

unsigned int ModeCalibration_getValForTenP();

unsigned int ModeCalibration_getValForTwelveP();

float ModeCalibration_getValForServoDown();

void ModeCalibration_pageTow();

unsigned int ModeCalibration_getMotorValC1ForMidi();

unsigned int ModeCalibration_getMotorValC2ForMidi();

float ModeCalibration_getValForServoUp();

#endif /* CODE_HARDWAREV2_SRC_APP_CALIBRATION_MODESEMI_H_ */
