
#ifndef CODE_HARDWAREV2_SRC_APP_MODEUPDATE_MODEUPDATE_H_
#define CODE_HARDWAREV2_SRC_APP_MODEUPDATE_MODEUPDATE_H_


void ModeUpdate_init();

void ModeUpdate_cmdReceived(unsigned int type, unsigned int action, unsigned int len, unsigned char * data);

#endif /* CODE_HARDWAREV2_SRC_APP_MODEUPDATE_MODEUPDATE_H_ */
