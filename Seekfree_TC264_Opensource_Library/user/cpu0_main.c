/*********************************************************************************************************************
* TC264 Opensourec Library 即（TC264 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
*
* 本文件是 TC264 开源库的一部分
*
* TC264 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
*
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
*
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
*
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
*
* 文件名称          cpu0_main
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          ADS v1.8.0
* 适用平台          TC264D
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期              作者                备注
* 2022-09-15       pudding            first version
********************************************************************************************************************/
#include "zf_common_headfile.h"
#include "define.h"
#include "motor.h"
#include "print.h"
#include "upperComputer.h"
#pragma section all "cpu0_dsram"
// 将本语句与#pragma section all restore语句之间的全局变量都放在CPU0的RAM中


// 工程导入到软件之后，应该选中工程然后点击refresh刷新一下之后再编译
// 工程默认设置为关闭优化，可以自己右击工程选择properties->C/C++ Build->Setting
// 然后在右侧的窗口中找到C/C++ Compiler->Optimization->Optimization level处设置优化等级
// 一般默认新建立的工程都会默认开2级优化，因此大家也可以设置为2级优化

// 对于TC系列默认是不支持中断嵌套的，希望支持中断嵌套需要在中断内使用 interrupt_global_enable(0); 来开启中断嵌套
// 简单点说实际上进入中断后TC系列的硬件自动调用了 interrupt_global_disable(); 来拒绝响应任何的中断，因此需要我们自己手动调用 interrupt_global_enable(0); 来开启中断的响应。

// 本例程是开源库移植用空工程
// 本例程是开源库移植用空工程
// 本例程是开源库移植用空工程

// **************************** 代码区域 ****************************

// 有线串口有关变量
uint8 uart_get_data[64];
uint8 fifo_get_data[64];
uint8 get_data = 0;
uint32 fifo_data_count = 0;
fifo_struct uart_data_file;

// 无线串口相关变量
uint8 data_buffer[32];
uint8 data_len;
uint8 count = 0;

// 欧拉角相关变量
FusionAhrs ahrs;

uint8 switchMode = 255;
uint8 screenMode = 6;
uint8 uartSendMode = 255;
uint8 isMotorRunning = true;
// 姿态解算相关变量
FusionEuler euler;
int32 yawCount = 0;
float yawPrevious = 0;
float yawTargetOffset = 0;

int core0_main(void)
{
    clock_init();                   // 获取时钟频率<务必保留>
    debug_init();                   // 初始化默认调试串口
    // 此处编写用户代码 例如外设初始化代码等

    gpio_init(SW_1_PIN, GPI, GPIO_LOW, GPI_PULL_UP);
    gpio_init(SW_2_PIN, GPI, GPIO_LOW, GPI_PULL_UP);
    gpio_init(SW_3_PIN, GPI, GPIO_LOW, GPI_PULL_UP);
    gpio_init(SW_4_PIN, GPI, GPIO_LOW, GPI_PULL_UP);

    gpio_init(BELL_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL);

    gpio_init(BTN_1_PIN, GPI, GPIO_HIGH, GPI_PULL_UP);
    gpio_init(BTN_2_PIN, GPI, GPIO_HIGH, GPI_PULL_UP);

    wireless_uart_init();
    wireless_uart_send_byte('\r');
    wireless_uart_send_byte('\n');
    wireless_uart_send_string("Wireless uart init successful.\r\n");


    // 摄像头初始化
    mt9v03x_init();

    tft180_init();

    // 陀螺仪初始化
    imu660ra_init();
    FusionAhrsInitialise(&ahrs);

    initMotors();
    
    pit_ms_init(CCU60_CH1, 10);
    
    // gpio_set_level(WHEEL_3_DIR_PIN, 0);
    // pwm_set_duty(WHEEL_3_PWM_PIN, 1000);

    // 此处编写用户代码 例如外设初始化代码等
    cpu_wait_event_ready();         // 等待所有核心初始化完毕
    while (TRUE)
    {
        // 此处编写需要循环执行的代码

        switchMode = 0;
        switchMode |= gpio_get_level(SW_1_PIN) << 0;
        switchMode |= gpio_get_level(SW_2_PIN) << 1;
        switchMode |= gpio_get_level(SW_3_PIN) << 2;
        switchMode |= gpio_get_level(SW_4_PIN) << 3;
        
        if(!(switchMode & 1)){ screenMode = 255; }
        if(!(switchMode & 2)){ uartSendMode = 255; }
        if(!(switchMode & 4)){ isMotorRunning = false; }

        data_len = (uint8)wireless_uart_read_buff(data_buffer, 185);             // 查看是否有消息 默认缓冲区是 WIRELESS_UART_BUFFER_SIZE 总共 64 字节
        if(data_len > 0){
            wireless_uart_send_string("Receive: ");
            wireless_uart_send_buff(data_buffer, data_len);
        }
        if(data_len >= 3){
            uartCommandHandler(data_buffer);
        }
        memset(data_buffer, 0, 32);

        switch (screenMode){ 
            case 0:
                printAllAttitudeSolution(angPIDz.measurement, angPIDy.measurement, angPIDx.measurement);        
                break;
            case 1:
                printMotorSpeed(velPIDl.measurement, velPIDr.measurement, velPIDy.measurement);
                break;
            case 2:
                printAngVelPID(&angVelPIDx, &angVelPIDy, &angVelPIDz);
                break;
            case 3: // 显示所有PID参数
                printAllPIDCoef();
                break;
            case 4:
                printAllPIDOutput();
                break;
            case 5:
                printCamera();
                break;
            case 6:
                printAngVelPIDx();
                break;
            default:
                system_delay_ms(5); // 千万别删!无线串口read_buffer()相邻两次调用需要一定的延时,否则会收发失去同步/藏包.
        }
        
        switch (uartSendMode){
            case 0:
                wireless_uart_LingLi_send(
                        imu660ra_gyro_x, imu660ra_gyro_y, imu660ra_gyro_z, 0,
                        imu660ra_acc_x, imu660ra_acc_y, imu660ra_acc_z, 0,
                        euler.angle.yaw, euler.angle.roll, euler.angle.pitch, 0
                );
                break;
            case 1:
                wireless_uart_LingLi_send(
                        motorLeft.pwm, motorRight.pwm, motorBottom.pwm, 0,
                        angVelPIDx.measurement, angVelPIDy.measurement, angVelPIDz.measurement, 0,
                        angPIDx.measurement, angPIDy.measurement, angPIDz.measurement, 0 
                );
                break;
            case 2: // 调试角速度环
                wireless_uart_LingLi_send(
                        imu660ra_gyro_x, imu660ra_gyro_y, imu660ra_gyro_z, 0,
                        angPIDx.deltaOutput, angPIDy.deltaOutput, angPIDz.deltaOutput, 0,
                        angVelPIDx.deltaOutput, angVelPIDy.deltaOutput, angVelPIDz.deltaOutput, 0
                );
                break;
            case 3:
                wireless_uart_LingLi_send(
                        angVelPIDx.target, angVelPIDx.measurement, angVelPIDx.deltaOutput, 0,
                        angVelPIDy.target, angVelPIDy.measurement, angVelPIDy.deltaOutput, 0,
                        angVelPIDz.target, angVelPIDz.measurement, angVelPIDz.deltaOutput, 0
                );
                break;
            case 4:
                wireless_uart_LingLi_send(
                        motorLeft.pwm, motorRight.pwm, motorBottom.pwm, 0,
                        0,0,0,0,
                        0,0,0,0
                );
                break;
            case 5:
                wireless_uart_LingLi_send(
                        angPIDx.target * 100, angPIDx.measurement*100   , 0, 0,
                        motorLeft.pwm, motorRight.pwm, 0, 0,
                        velPIDl.measurement, velPIDr.measurement, 0, 0
                );
                break;
            case 6:
                wireless_uart_LingLi_send(
                        angPIDz.pCorr, angPIDz.iCorr, angPIDz.dCorr, angPIDz.deltaOutput,
                        motorLeft.pwm, motorRight.pwm, 0, 0,
                        angPIDz.target, angPIDz.measurement, angPIDz.deltaOutput, 0
                );
        }

        // if(gpio_get_level(BTN_1_PIN) == 0){
        //     system_delay_ms(2);
        //     if(gpio_get_level(BTN_1_PIN) == 0){
        //         motorMode = 0;
        //     }
        // }

        // if(gpio_get_level(BTN_2_PIN) == 0){
        //     system_delay_ms(2);
        //     if(gpio_get_level(BTN_2_PIN) == 0){
        //         motorMode = 1;
        //     }
        // }

        // 此处编写需要循环执行的代码
    }
}

#pragma section all restore
// **************************** 代码区域 ****************************

