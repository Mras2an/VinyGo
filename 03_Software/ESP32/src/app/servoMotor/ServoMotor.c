
#include "ServoMotor.h"
#include "FileSystem.h"
#include "Board.h"
#include "System.h"
#include "MotorHandling.h"
#include "CpuDelay.h"
#include "Debug.h"
#include "ModeCalibration.h"


typedef struct ServoMotor_t {
    int direction;
} ServoMotor;

ServoMotor ServoMotor_t;
#define this (&ServoMotor_t)


void ServoMotor_init()
{
    if(ServoMotor_getDirection()) {
        this->direction = 1;
        Debug_info("Servo motor inverted\n");
    } else {
        this->direction = 0;
        Debug_info("Servo motor not inverted\n");
    }
}

void ServoMotor_toggleDirection()
{
    char * servo = Fs_read(FILE_NAME_SERVO_MOTOR, FILE_NAME_SERVO_MOTOR);

    if(servo != NULL)  {
        OsFree(servo);
        this->direction = 0;
        Debug_info("Servo motor not inverted\n");
        Fs_delete_all(FILE_NAME_SERVO_MOTOR);
    } else {
        this->direction = 1;
        Debug_info("Servo motor inverted\n");
        Fs_write(FILE_NAME_SERVO_MOTOR, "true", FILE_NAME_SERVO_MOTOR);
    }
}

int ServoMotor_getDirection()
{
    char * servo = Fs_read(FILE_NAME_SERVO_MOTOR, FILE_NAME_SERVO_MOTOR);

    if(servo != NULL)  {
        OsFree(servo);
        return 1;
    }

    return 0;
}

void ServoMotor_getUpOrDown(char * buffer)
{
    //    uint32_t direction[2] = {0x00, 0x00};
    //    Gpio_get(HEAD_DETECT, &direction[0]);
    uint32_t sensor[2] = {0x00, 0x00};
    Gpio_get(HEAD_DETECT, &sensor[0]);

    //up
    if(sensor[0])
        memcpy(buffer, "Down", 4);
    else
        memcpy(buffer, "Up", 2);
}

void ServoMotor_up()
{
    Debug_info("Servo motor up\n");

    if(this->direction == 0) {
        MotorHandling_startServo(50, ModeCalibration_getValForServoUp());
    } else {
        Debug_err("NEED TO BE CODED");
    }

    CpuDelay_ms(200);
}

void ServoMotor_down()
{
    Debug_info("Servo motor down\n");

    if(this->direction == 0) {
        for(float i = 5; i <= ModeCalibration_getValForServoDown(); i = i + 0.1) {
            MotorHandling_startServo(50, i);
            CpuDelay_ms(75);
        }
    } else {
        Debug_err("NEED TO BE CODED");
    }
}
