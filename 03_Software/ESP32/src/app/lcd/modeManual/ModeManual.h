

#ifndef CODE_HARDWAREV2_SRC_APP_MODEMANUAL_MODEMANUAL_H_
#define CODE_HARDWAREV2_SRC_APP_MODEMANUAL_MODEMANUAL_H_


void ModeManual_init();

void ModeManual_cmdReceived(unsigned int type, unsigned int action, unsigned int len, unsigned char * data);

#endif /* CODE_HARDWAREV2_SRC_APP_MODEMANUAL_MODEMANUAL_H_ */
