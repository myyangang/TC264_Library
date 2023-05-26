#ifndef _PRINT_H_
#define _PRINT_H_

#include "zf_common_headfile.h"
#include "define.h"
#include "pid.h"
#include "motor.h"

void printAcc(tft180_dir_enum direction);
void printGyro(tft180_dir_enum direction);
void printEularAngle(float yaw, float pitch, float roll, tft180_dir_enum direction);
void printAllAttitudeSolution(float yaw, float pitch, float roll, tft180_dir_enum direction);
void printAngVelPID(PIDValue *angVelPIDx, PIDValue *angVelPIDy, PIDValue *angVelPIDz, tft180_dir_enum direction);
void printMotorSpeed(int16 motorLeftSpeed, int16 motorRightSpeed, int16 motorBottomSpeed, tft180_dir_enum direction);
void printAllPIDCoef(tft180_dir_enum direction);
void printAllPIDOutput(tft180_dir_enum direction);
void printCamera(tft180_dir_enum direction);
#endif