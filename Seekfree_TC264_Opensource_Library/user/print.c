#include "print.h"

/**
 * @brief 绘制加速度计数据(陀螺仪原坐标系)
*/
void printAcc(tft180_dir_enum direction){
    if(direction == TFT180_PORTAIT){
        tft180_show_string(0, 0, "accX->Y");  
        tft180_show_string(0, 16, "accY->Z"); 
        tft180_show_string(0, 32, "accZ->X");
        tft180_show_float(78, 0, imu660ra_acc_transition(imu660ra_acc_x), 2, 2);
        tft180_show_float(78, 16, imu660ra_acc_transition(imu660ra_acc_y), 2, 2);
        tft180_show_float(78, 32, imu660ra_acc_transition(imu660ra_acc_z), 2, 2);
    }
    if(direction == TFT180_CROSSWISE){
        tft180_show_string(0, 0 , "aX>Y");  
        tft180_show_string(0, 16, "aY>Z"); 
        tft180_show_string(0, 32, "aZ>X");
        tft180_show_float(40, 0 , imu660ra_acc_transition(imu660ra_acc_x), 2, 1);
        tft180_show_float(40, 16, imu660ra_acc_transition(imu660ra_acc_y), 2, 1);
        tft180_show_float(40, 32, imu660ra_acc_transition(imu660ra_acc_z), 2, 1);
    }


}

void printGyro(tft180_dir_enum direction){
    if(direction == TFT180_PORTAIT){
        tft180_show_string(0, 48, "gyroX->Y");
        tft180_show_string(0, 64, "gyroY->Z");
        tft180_show_string(0, 80, "gyroZ->X");
        tft180_show_float(78, 48, imu660ra_gyro_transition(imu660ra_gyro_x), 2, 1);
        tft180_show_float(78, 64, imu660ra_gyro_transition(imu660ra_gyro_y), 2, 1);
        tft180_show_float(78, 80, imu660ra_gyro_transition(imu660ra_gyro_z), 2, 1);
    }
    if(direction == TFT180_CROSSWISE){
        tft180_show_string(76, 0 , "gX>Y");
        tft180_show_string(76, 16, "gY>Z");
        tft180_show_string(76, 32, "gZ>X");
        tft180_show_float(118, 0 , imu660ra_gyro_transition(imu660ra_gyro_x), 2, 1);
        tft180_show_float(118, 16, imu660ra_gyro_transition(imu660ra_gyro_y), 2, 1);
        tft180_show_float(118, 32, imu660ra_gyro_transition(imu660ra_gyro_z), 2, 1);
    }
}

void printEularAngle(float yaw, float pitch, float roll, tft180_dir_enum direction){
    if(direction == TFT180_PORTAIT){
        tft180_show_string(0, 96, "yaw");
        tft180_show_string(0, 112, "rol");
        tft180_show_string(0, 128, "pitch");
        tft180_show_float(43, 96, yaw, 3, 2);
        tft180_show_float(43, 112, roll, 3, 2);
        tft180_show_float(43, 128, pitch, 3, 2);
    }
    if(direction == TFT180_CROSSWISE){
        tft180_show_string(0, 48, "yaw");
        tft180_show_string(0, 64, "rol");
        tft180_show_string(0, 80, "pitch");
        tft180_show_float(40, 48, yaw, 3, 2);
        tft180_show_float(40, 64, roll, 3, 2);
        tft180_show_float(40, 80, pitch, 3, 2);
    }
}

// screenMode = 0
void printAllAttitudeSolution(float yaw, float pitch, float roll, tft180_dir_enum direction){
    printAcc(direction);
    printGyro(direction);
    printEularAngle(yaw, pitch, roll, direction);
}

// screenMode = 1
void printMotorSpeed(int16 motorLeftSpeed, int16 motorRightSpeed, int16 motorBottomSpeed, tft180_dir_enum direction){
    if(direction == TFT180_PORTAIT){
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
    if(direction == TFT180_CROSSWISE){
        tft180_show_string(0, 16, "Left");
        tft180_show_string(0, 32, "Right");
        tft180_show_string(0, 48, "Bottom");
        
        tft180_show_string(50, 0, "Speed");
        tft180_show_int(50, 16, motorLeftSpeed, 4);   
        tft180_show_int(50, 32, motorRightSpeed, 4); 
        tft180_show_int(50, 48, motorBottomSpeed, 4);

        tft180_show_string(100, 0, "PWM");
        tft180_show_int(100, 16, motorLeft.pwm, 5);
        tft180_show_int(100, 32, motorRight.pwm, 5);
        tft180_show_int(100, 48, motorBottom.pwm, 5);
    }
}

// screenMode = 2
void printAngVelPID(PIDValue *angVelPIDx, PIDValue *angVelPIDy, PIDValue *angVelPIDz, tft180_dir_enum direction){
    if(direction == TFT180_PORTAIT){
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
    if(direction == TFT180_CROSSWISE){
        tft180_show_string(40, 0 , "angVelPID");
        tft180_show_string(0, 32, "X");
        tft180_show_string(0, 48, "Y");
        tft180_show_string(0, 64, "Z");

        tft180_show_string(20, 16, "targ");
        tft180_show_int(20, 32, angVelPIDx->target, 4);
        tft180_show_int(20, 48, angVelPIDy->target, 4);
        tft180_show_int(20, 64, angVelPIDz->target, 4);

        tft180_show_string(70, 16, "meas");
        tft180_show_int(70, 32, angVelPIDx->measurement, 4);
        tft180_show_int(70, 48, angVelPIDy->measurement, 4);
        tft180_show_int(70, 64, angVelPIDz->measurement, 4);

        tft180_show_string(110, 16, "output");
        tft180_show_int(110, 32, angVelPIDx->deltaOutput, 4);
        tft180_show_int(110, 48, angVelPIDy->deltaOutput, 4);
        tft180_show_int(110, 64, angVelPIDz->deltaOutput, 4);
    }
}

// screenMode = 3
void printAllPIDCoef(tft180_dir_enum direction){
    if(direction == TFT180_PORTAIT){
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
    }
    if(direction == TFT180_CROSSWISE){
        tft180_show_string(32, 0, "PID Coef");
        tft180_show_string(0, 16 , "wX"); tft180_show_int(20, 16 , angVelPIDx.pCoef, 4); tft180_show_int(54, 16 , angVelPIDx.iCoef, 4); tft180_show_int(88, 16 , angVelPIDx.dCoef, 4);
        tft180_show_string(0, 32 , "wY"); tft180_show_int(20, 32 , angVelPIDy.pCoef, 4); tft180_show_int(54, 32 , angVelPIDy.iCoef, 4); tft180_show_int(88, 32 , angVelPIDy.dCoef, 4);
        tft180_show_string(0, 48 , "wZ"); tft180_show_int(20, 48 , angVelPIDz.pCoef, 4); tft180_show_int(54, 48 , angVelPIDz.iCoef, 4); tft180_show_int(88, 48 , angVelPIDz.dCoef, 4);
        tft180_show_string(0, 64 , "aX"); tft180_show_int(20, 64 , angPIDx.pCoef, 4); tft180_show_int(54, 64 , angPIDx.iCoef, 4); tft180_show_int(88, 64 , angPIDx.dCoef, 4);
        tft180_show_string(0, 80 , "aY"); tft180_show_int(20, 80 , angPIDy.pCoef, 4); tft180_show_int(54, 80 , angPIDy.iCoef, 4); tft180_show_int(88, 80 , angPIDy.dCoef, 4);
        tft180_show_string(0, 96 , "aZ"); tft180_show_int(20, 96 , angPIDz.pCoef, 4); tft180_show_int(54, 96 , angPIDz.iCoef, 4); tft180_show_int(88, 96 , angPIDz.dCoef, 4);
        // 实在排不开版了
        // tft180_show_string(0, 112, "vL"); tft180_show_int(20, 112, velPIDl.pCoef, 4); tft180_show_int(54, 112, velPIDl.iCoef, 4); tft180_show_int(88, 112, velPIDl.dCoef, 4);
        // tft180_show_string(0, 128, "vR"); tft180_show_int(20, 128, velPIDr.pCoef, 4); tft180_show_int(54, 128, velPIDr.iCoef, 4); tft180_show_int(88, 128, velPIDr.dCoef, 4);
        // tft180_show_string(0, 144, "vY"); tft180_show_int(20, 144, velPIDy.pCoef, 4); tft180_show_int(54, 144, velPIDy.iCoef, 4); tft180_show_int(88, 144, velPIDy.dCoef, 4);
    }
}

// screenMode = 4
void printAllPIDOutput(tft180_dir_enum direction){
    if(direction == TFT180_PORTAIT){
        tft180_show_string(0, 0, "PID DeltaOutput");
        tft180_show_string(0, 16 , "wX"); tft180_show_int(20, 16 , angVelPIDx.target, 4); tft180_show_int(54, 16 , angVelPIDx.measurement, 4); tft180_show_int(88, 16 , angVelPIDx.deltaOutput, 4);
        tft180_show_string(0, 32 , "wY"); tft180_show_int(20, 32 , angVelPIDy.target, 4); tft180_show_int(54, 32 , angVelPIDy.measurement, 4); tft180_show_int(88, 32 , angVelPIDy.deltaOutput, 4);
        tft180_show_string(0, 48 , "wZ"); tft180_show_int(20, 48 , angVelPIDz.target, 4); tft180_show_int(54, 48 , angVelPIDz.measurement, 4); tft180_show_int(88, 48 , angVelPIDz.deltaOutput, 4);
        tft180_show_string(0, 64 , "aX"); tft180_show_int(20, 64 , angPIDx.target, 4); tft180_show_int(54, 64 , angPIDx.measurement, 4); tft180_show_int(88, 64 , angPIDx.deltaOutput, 4);
        tft180_show_string(0, 80 , "aY"); tft180_show_int(20, 80 , angPIDy.target, 4); tft180_show_int(54, 80 , angPIDy.measurement, 4); tft180_show_int(88, 80 , angPIDy.deltaOutput, 4);
        tft180_show_string(0, 96 , "aZ"); tft180_show_int(20, 96 , angPIDz.target, 4); tft180_show_int(54, 96 , angPIDz.measurement, 4); tft180_show_int(88, 96 , angPIDz.deltaOutput, 4);
        tft180_show_string(0, 112, "vL"); tft180_show_int(20, 112, velPIDl.target, 4); tft180_show_int(54, 112, velPIDl.measurement, 4); tft180_show_int(88, 112, velPIDl.deltaOutput, 4);
        tft180_show_string(0, 128, "vR"); tft180_show_int(20, 128, velPIDr.target, 4); tft180_show_int(54, 128, velPIDr.measurement, 4); tft180_show_int(88, 128, velPIDr.deltaOutput, 4);
        tft180_show_string(0, 144, "vY"); tft180_show_int(20, 144, velPIDy.target, 4); tft180_show_int(54, 144, velPIDy.measurement, 4); tft180_show_int(88, 144, velPIDy.deltaOutput, 4);
    }
    if(direction == TFT180_CROSSWISE){
        // 开摆，排不开版
        tft180_show_string(0, 0, "PID DeltaOutput");
        tft180_show_string(0, 16 , "wX"); tft180_show_int(20, 16 , angVelPIDx.target, 4); tft180_show_int(54, 16 , angVelPIDx.measurement, 4); tft180_show_int(88, 16 , angVelPIDx.deltaOutput, 4);
        tft180_show_string(0, 32 , "wY"); tft180_show_int(20, 32 , angVelPIDy.target, 4); tft180_show_int(54, 32 , angVelPIDy.measurement, 4); tft180_show_int(88, 32 , angVelPIDy.deltaOutput, 4);
        tft180_show_string(0, 48 , "wZ"); tft180_show_int(20, 48 , angVelPIDz.target, 4); tft180_show_int(54, 48 , angVelPIDz.measurement, 4); tft180_show_int(88, 48 , angVelPIDz.deltaOutput, 4);
        tft180_show_string(0, 64 , "aX"); tft180_show_int(20, 64 , angPIDx.target, 4); tft180_show_int(54, 64 , angPIDx.measurement, 4); tft180_show_int(88, 64 , angPIDx.deltaOutput, 4);
        tft180_show_string(0, 80 , "aY"); tft180_show_int(20, 80 , angPIDy.target, 4); tft180_show_int(54, 80 , angPIDy.measurement, 4); tft180_show_int(88, 80 , angPIDy.deltaOutput, 4);
        tft180_show_string(0, 96 , "aZ"); tft180_show_int(20, 96 , angPIDz.target, 4); tft180_show_int(54, 96 , angPIDz.measurement, 4); tft180_show_int(88, 96 , angPIDz.deltaOutput, 4);
        // tft180_show_string(0, 112, "vL"); tft180_show_int(20, 112, velPIDl.target, 4); tft180_show_int(54, 112, velPIDl.measurement, 4); tft180_show_int(88, 112, velPIDl.deltaOutput, 4);
        // tft180_show_string(0, 128, "vR"); tft180_show_int(20, 128, velPIDr.target, 4); tft180_show_int(54, 128, velPIDr.measurement, 4); tft180_show_int(88, 128, velPIDr.deltaOutput, 4);
        // tft180_show_string(0, 144, "vY"); tft180_show_int(20, 144, velPIDy.target, 4); tft180_show_int(54, 144, velPIDy.measurement, 4); tft180_show_int(88, 144, velPIDy.deltaOutput, 4);
    }
}

void printCamera(tft180_dir_enum direction){
    if(direction == TFT180_PORTAIT){
        tft180_show_gray_image(0, 0, mt9v03x_image[0], MT9V03X_W, MT9V03X_H, 160/2+1, 128/2+1, 0);
    }
    if(direction == TFT180_CROSSWISE){
        tft180_show_gray_image(0, 0, mt9v03x_image[0], MT9V03X_W, MT9V03X_H, 141, 90, 0);
    }
}