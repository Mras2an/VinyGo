
#ifndef CODE_HARDWAREV2_SRC_APP_MODEAUTOMATIC_MODEAUTOMATIC_H_
#define CODE_HARDWAREV2_SRC_APP_MODEAUTOMATIC_MODEAUTOMATIC_H_


void ModeAutomatic_init();

void ModeAutomatic_cmdReceived(unsigned int type, unsigned int action, unsigned int len, unsigned char * data);

int ModeAutomatic_modeIsStarted();

void ModeAutomatic_setModeStarted(int stop);

#endif /* CODE_HARDWAREV2_SRC_APP_MODEAUTOMATIC_MODEAUTOMATIC_H_ */
