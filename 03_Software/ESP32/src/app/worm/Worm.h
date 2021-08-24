#ifndef CODE_HARDWAREV2_SRC_APP_WORM_WORM_H_
#define CODE_HARDWAREV2_SRC_APP_WORM_WORM_H_

#define FILE_NAME_WROM          "worm"

void Worm_init();

void Worm_convertFreqToTimeInString(int freq, char * buffer);

void Worm_toggleValue();

#endif /* CODE_HARDWAREV2_SRC_APP_WORM_WORM_H_ */
