#include "print.h"

/**
 * @brief 绘制加速度计数据(陀螺仪原坐标系)
*/
void printAcc(){
    tft180_show_string(0, 0, "accX->Y");  
    tft180_show_string(0, 16, "accY->Z"); 
    tft180_show_string(0, 32, "accZ->X");

    // tft180_show_int(43, 0, imu660ra_acc_x, 6);  
    // tft180_show_int(43, 16, imu660ra_acc_y, 6); 
    // tft180_show_int(43, 32, imu660ra_acc_z, 6);

    tft180_show_float(78, 0, imu660ra_acc_transition(imu660ra_acc_x), 2, 2);
    tft180_show_float(78, 16, imu660ra_acc_transition(imu660ra_acc_y), 2, 2);
    tft180_show_float(78, 32, imu660ra_acc_transition(imu660ra_acc_z), 2, 2);
}

void printGyro(){
    tft180_show_string(0, 48, "gyroX->Y");
    tft180_show_string(0, 64, "gyroY->Z");
    tft180_show_string(0, 80, "gyroZ->X");

    // tft180_show_int(43, 48, imu660ra_gyro_x, 6);
    // tft180_show_int(43, 64, imu660ra_gyro_y, 6);
    // tft180_show_int(43, 80, imu660ra_gyro_z, 6);

    tft180_show_float(78, 48, imu660ra_gyro_transition(imu660ra_gyro_x), 2, 2);
    tft180_show_float(78, 64, imu660ra_gyro_transition(imu660ra_gyro_y), 2, 2);
    tft180_show_float(78, 80, imu660ra_gyro_transition(imu660ra_gyro_z), 2, 2);
}

void printEularAngle(float yaw, float pitch, float roll){
    tft180_show_string(0, 96, "yaw");
    tft180_show_string(0, 112, "rol");
    tft180_show_string(0, 128, "pitch");

    tft180_show_float(43, 96, yaw, 3, 2);
    tft180_show_float(43, 112, roll, 3, 2);
    tft180_show_float(43, 128, pitch, 3, 2);

    // tft180_show_int(42, 96, euler->angle.yaw, 3);
    // tft180_show_int(42, 112, euler->angle.roll, 3);
    // tft180_show_int(42, 128, euler->angle.pitch, 3);
}

/**
 * @bug 禁用,与无线串口冲突
*/
void printAllAttitudeSolution(float yaw, float pitch, float roll){
    printAcc();
    printGyro();
    printEularAngle(yaw, pitch, roll);
}

void printAngVelPID(PIDValue *angVelPIDx, PIDValue *angVelPIDy, PIDValue *angVelPIDz){
    tft180_show_string(30, 0, "angVelPID");

    tft180_show_string(0, 16, "X:");
    tft180_show_string(0, 32, "Y:");
    tft180_show_string(0, 48, "Z:");

    tft180_show_int(20, 16, angVelPIDx->target, 4);
    tft180_show_int(20, 32, angVelPIDy->target, 4);
    tft180_show_int(20, 48, angVelPIDz->target, 4);

    tft180_show_int(30, 16, angVelPIDx->measurement, 4);
    tft180_show_int(30, 32, angVelPIDy->measurement, 4);
    tft180_show_int(30, 48, angVelPIDz->measurement, 4);

    tft180_show_int(50, 16, angVelPIDx->deltaOutput, 4);
    tft180_show_int(50, 32, angVelPIDy->deltaOutput, 4);
    tft180_show_int(50, 48, angVelPIDz->deltaOutput, 4);




}

void printMotorSpeed(int16 motorLeftSpeed, int16 motorRightSpeed, int16 motorBottomSpeed){
    tft180_show_string(0, 0, "Motor_Speed_PWM");
    tft180_show_string(0, 16, "Left");
    tft180_show_string(0, 32, "Right");
    tft180_show_string(0, 48, "Bottom");
    
    tft180_show_int(40, 16, motorLeftSpeed, 4);   
    tft180_show_int(40, 32, motorRightSpeed, 4); 
    tft180_show_int(40, 48, motorBottomSpeed, 4);

    tft180_show_int(74, 16, motorLeft.pwm, 5);
    tft180_show_int(74, 32, motorRight.pwm, 5);
    tft180_show_int(74, 48, motorBottom.pwm, 5);
}

void printAllPIDCoef(Motor *motorLeft, Motor *motorRight, Motor *motorBottom){
    tft180_show_string(32, 0, "PID Coef");
    tft180_show_string(0, 16 , "wX"); tft180_show_int(20, 16 , angVelPIDx.pCoef, 4); tft180_show_int(54, 16 , angVelPIDx.iCoef, 4); tft180_show_int(88, 16 , angVelPIDx.dCoef, 4);
    tft180_show_string(0, 32 , "wY"); tft180_show_int(20, 32 , angVelPIDy.pCoef, 4); tft180_show_int(54, 32 , angVelPIDy.iCoef, 4); tft180_show_int(88, 32 , angVelPIDy.dCoef, 4);
    tft180_show_string(0, 48 , "wZ"); tft180_show_int(20, 48 , angVelPIDz.pCoef, 4); tft180_show_int(54, 48 , angVelPIDz.iCoef, 4); tft180_show_int(88, 48 , angVelPIDz.dCoef, 4);
    tft180_show_string(0, 64 , "aX"); tft180_show_int(20, 64 , angPIDx.pCoef, 4); tft180_show_int(54, 64 , angPIDx.iCoef, 4); tft180_show_int(88, 64 , angPIDx.dCoef, 4);
    tft180_show_string(0, 80 , "aY"); tft180_show_int(20, 80 , angPIDy.pCoef, 4); tft180_show_int(54, 80 , angPIDy.iCoef, 4); tft180_show_int(88, 80 , angPIDy.dCoef, 4);
    tft180_show_string(0, 96 , "aZ"); tft180_show_int(20, 96 , angPIDz.pCoef, 4); tft180_show_int(54, 96 , angPIDz.iCoef, 4); tft180_show_int(88, 96 , angPIDz.dCoef, 4);
    tft180_show_string(0, 112, "vL"); tft180_show_int(20, 112, velPIDl.pCoef, 4); tft180_show_int(54, 112, velPIDl.iCoef, 4); tft180_show_int(88, 112, velPIDl.dCoef, 4);
    tft180_show_string(0, 128, "vR"); tft180_show_int(20, 128, velPIDr.pCoef, 4); tft180_show_int(54, 128, velPIDr.iCoef, 4); tft180_show_int(88, 128, velPIDr.dCoef, 4);
    tft180_show_string(0, 144, "vY"); tft180_show_int(20, 144, velPIDy.pCoef, 4); tft180_show_int(54, 144, velPIDy.iCoef, 4); tft180_show_int(88, 144, velPIDy.dCoef, 4);

    // tft180_show_int(74, 0, angVelPIDx.dCoef, 4);
    // tft180_show_int(42, 0, angVelPIDx.pCoef, 4);
    // tft180_show_int(80, 0, angVelPIDx.iCoef, 4);
    // tft180_show_int(100, 0, angVelPIDx.dCoef, 4);
    // tft180_show_int(16, 0, angVelPIDx.pCoef, 4);
    // tft180_show_int(16, 0, angVelPIDx.pCoef, 4);
    // tft180_show_int(16, 0, angVelPIDx.pCoef, 4);
    // tft180_show_int(16, 0, angVelPIDx.pCoef, 4);
    // tft180_show_int(16, 0, angVelPIDx.pCoef, 4);
    // tft180_show_int(16, 0, angVelPIDx.pCoef, 4);
    
}

void printAllPIDOutput(){
    tft180_show_string(0, 0, "PID DeltaOutput");
    tft180_show_string(0, 16 , "wX"); tft180_show_int(20, 16 , angVelPIDx.target, 4); tft180_show_int(54, 16 , angVelPIDx.measurement, 4); tft180_show_int(88, 16 , angVelPIDx.deltaOutput, 4);
    tft180_show_string(0, 32 , "wY"); tft180_show_int(20, 32 , angVelPIDy.target, 4); tft180_show_int(54, 32 , angVelPIDy.measurement, 4); tft180_show_int(88, 32 , angVelPIDy.deltaOutput, 4);
    tft180_show_string(0, 48 , "wZ"); tft180_show_int(20, 48 , angVelPIDz.target, 4); tft180_show_int(54, 48 , angVelPIDz.measurement, 4); tft180_show_int(88, 48 , angVelPIDz.deltaOutput, 4);
    tft180_show_string(0, 64 , "aX"); tft180_show_float(20,64, angPIDx.target, 2, 1); tft180_show_float(54,64, angPIDx.measurement, 2, 1); tft180_show_int(88, 64 , angPIDx.deltaOutput, 4);
    tft180_show_string(0, 80 , "aY"); tft180_show_int(20, 80 , angPIDy.target, 4   ); tft180_show_int(54, 80 , angPIDy.measurement, 4   ); tft180_show_int(88, 80 , angPIDy.deltaOutput, 4);
    tft180_show_string(0, 96 , "aZ"); tft180_show_float(20,96, angPIDz.target, 2, 1); tft180_show_float(54,96, angPIDz.measurement, 2, 1); tft180_show_int(88, 96 , angPIDz.deltaOutput, 4);
    tft180_show_string(0, 112, "vL"); tft180_show_int(20, 112, velPIDl.target, 4   ); tft180_show_int(54, 112, velPIDl.measurement, 4   ); tft180_show_int(88, 112, velPIDl.deltaOutput, 4);
    tft180_show_string(0, 128, "vR"); tft180_show_int(20, 128, velPIDr.target, 4   ); tft180_show_int(54, 128, velPIDr.measurement, 4   ); tft180_show_int(88, 128, velPIDr.deltaOutput, 4);
    tft180_show_string(0, 144, "vY"); tft180_show_int(20, 144, velPIDy.target, 4   ); tft180_show_int(54, 144, velPIDy.measurement, 4   ); tft180_show_int(88, 144, velPIDy.deltaOutput, 4);
}

void printCamera(){
    tft180_show_gray_image(0, 0, mt9v03x_image[0], MT9V03X_W, MT9V03X_H, 160/10, 128/10, 0);
}

void printAngVelPIDx(){
    tft180_show_string(0, 0, "Why aZ effect wX");
    
    tft180_show_string(20 , 16 , "PWM"); tft180_show_string(52 , 16 , "avX"); tft180_show_string(88 , 16 , "avZ");
    tft180_show_string(0 , 32 , "ML"); tft180_show_int(20, 32, motorLeft.pwm, 4);  tft180_show_int(52, 32, -angVelPIDx.deltaOutput, 4); tft180_show_int(88, 32, angVelPIDz.deltaOutput, 4);
    tft180_show_string(0 , 48 , "MR"); tft180_show_int(20, 48, motorRight.pwm, 4); tft180_show_int(52, 48, +angVelPIDx.deltaOutput, 4); tft180_show_int(88, 48, angVelPIDz.deltaOutput, 4);

    tft180_show_string(0, 64, "aZtar"); tft180_show_float(0,  80, angPIDz.target, 2, 2);
    tft180_show_string(42,64, "vLout"); tft180_show_int(42, 80, velPIDl.deltaOutput, 4); 
    tft180_show_string(84,64, "vRout"); tft180_show_int(84, 80, velPIDr.deltaOutput, 4);

    tft180_show_string(0 ,96, "aXtar"); tft180_show_float(0,112,(-velPIDl.deltaOutput + velPIDr.deltaOutput)/100, 2,1);
    tft180_show_string(42 ,96, "aXtar"); tft180_show_float(42,112,(-velPIDl.deltaOutput + velPIDr.deltaOutput)/100, 2,1);
    tft180_show_string(84 ,96, "aXtar"); tft180_show_float(84,112,(-velPIDl.deltaOutput + velPIDr.deltaOutput)/100, 2,1);


}