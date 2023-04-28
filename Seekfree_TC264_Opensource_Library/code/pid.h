#ifndef _PID_H_
#define _PID_H_

#include "define.h"

typedef struct{
    float pCorr, iCorr, dCorr;
    float error[3];
    float errorInt;
    float errorIntMax;
    float pCoef, iCoef, dCoef;
    float target;
    float measurement;
    float deltaOutput;
}PIDValue;

void __initPID(PIDValue *pid,int32_t pCoef, int32_t iCoef, int32_t dCoef, int32_t target, int32_t errorIntMax);
int32_t __updatePID(PIDValue *pid);

#endif