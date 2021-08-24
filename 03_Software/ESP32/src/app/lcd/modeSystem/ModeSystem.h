
#ifndef CODE_HARDWAREV2_SRC_APP_MODESYSTEM_MODESYSTEM_H_
#define CODE_HARDWAREV2_SRC_APP_MODESYSTEM_MODESYSTEM_H_



void ModeSystem_init();

void ModeSystem_cmdReceived(unsigned int type, unsigned int action, unsigned int len, unsigned char * data);

#endif /* CODE_HARDWAREV2_SRC_APP_MODESYSTEM_MODESYSTEM_H_ */
