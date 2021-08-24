#ifndef CODE_HARDWAREV2_SRC_APP_SERVOMOTOR_H_
#define CODE_HARDWAREV2_SRC_APP_SERVOMOTOR_H_

#define FILE_NAME_SERVO_MOTOR   "sMot"

void ServoMotor_init();

void ServoMotor_up();

void ServoMotor_down();

int ServoMotor_getDirection();

void ServoMotor_toggleDirection();

void ServoMotor_getUpOrDown(char * buffer);

#endif /* CODE_HARDWAREV2_SRC_APP_SERVOMOTOR_H_ */
