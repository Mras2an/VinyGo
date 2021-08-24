#ifndef MOTOR_H_
#define MOTOR_H_

void StepMotorHandling_init(void);

void StepMotorHandling_setPositionOnX(int position);

void StepMotorHandling_getAMeasureOnY(int measure);

void StepMotorHandling_getAMeasureOnPump(int measure, int pump);

void StepMotorHandling_setInitialPosition();

#endif /* MOTOR_H_ */
