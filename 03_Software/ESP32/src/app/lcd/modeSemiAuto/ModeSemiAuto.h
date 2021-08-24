
#ifndef CODE_HARDWAREV2_SRC_APP_MODESEMI_MODESEMI_H_
#define CODE_HARDWAREV2_SRC_APP_MODESEMI_MODESEMI_H_


void ModeSemiAuto_init();

void ModeSemiAuto_cmdReceived(unsigned int type, unsigned int action, unsigned int len, unsigned char * data);

#endif /* CODE_HARDWAREV2_SRC_APP_MODESEMI_MODESEMI_H_ */
