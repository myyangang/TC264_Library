#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "zf_common_headfile.h"
#include "define.h"
#include "pid.h"

typedef struct {
    uint32_t freq;
    int32_t pwm; // 同时表示大小和方向
    pwm_channel_enum pwmChannel;
    gpio_pin_enum dirPin;
}Motor;

typedef enum {
    PLUS,
    MINUS,
    ASSIGN,
    OPPOSE
}Operation;

extern Motor motorLeft, motorRight, motorBottom;
extern PIDValue velPIDy, velPIDl, velPIDr;
extern PIDValue angPIDx, angPIDy, angPIDz;
extern PIDValue angVelPIDx, angVelPIDy, angVelPIDz; 

void __initMotor(Motor *motor, uint32 freq, int32 pwm, 
        pwm_channel_enum pwmChannel, gpio_pin_enum dirPin,
        int32_t pCoef, int32_t iCoef, int32_t dCoef, int32_t target, int32_t errorIntMax);
void initMotors();
void __updateMotor(Motor *motor);
void setMotor(Motor *motor, Operation op, int32_t offset);
void updateMotors(
        float motorLeftSpeed, float motorRightSpeed, float motorBottomSpeed, 
        float cameraSpeedTarget, float cameraTurnTarget, 
        float rollX, float pitchY, float yawZ,
        float angVelX, float angVelY, float angVelZ);

#endif