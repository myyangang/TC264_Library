#ifndef _PRINT_H_
#define _PRINT_H_

#include "zf_common_headfile.h"
#include "define.h"
#include "pid.h"
#include "motor.h"

void printAcc();
void printGyro();
void printEularAngle(float yaw, float pitch, float roll);
void printAllAttitudeSolution(float yaw, float pitch, float roll);
void printAngVelPID(PIDValue *angVelPIDx, PIDValue *angVelPIDy, PIDValue *angVelPIDz);
void printMotorSpeed(int16 motorLeftSpeed, int16 motorRightSpeed, int16 motorBottomSpeed);
void printAllPIDCoef();
void printAllPIDOutput();
#endif