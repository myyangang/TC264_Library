#include "upperComputer.h"

void wireless_uart_LingLi_send(
        uint16 a1, uint16 a2, uint16 a3, uint16 a4,
        uint16 b1, uint16 b2, uint16 b3, uint16 b4,
        uint16 c1, uint16 c2, uint16 c3, uint16 c4
        ) {

    int i, j;
    static unsigned short int send_data[3][4] = { { 0 }, { 0 }, { 0 } };
    short int checksum = 0;
    unsigned char xorsum = 0, high, low;

    send_data[0][0] = (unsigned short int)(a1);
    send_data[0][1] = (unsigned short int)(a2);
    send_data[0][2] = (unsigned short int)(a3);
    send_data[0][3] = (unsigned short int)(a4);

    send_data[1][0] = (unsigned short int)(b1);
    send_data[1][1] = (unsigned short int)(b2);
    send_data[1][2] = (unsigned short int)(b3);
    send_data[1][3] = (unsigned short int)(b4);

    send_data[2][0] = (unsigned short int)(c1);
    send_data[2][1] = (unsigned short int)(c2);
    send_data[2][2] = (unsigned short int)(c3);
    send_data[2][3] = (unsigned short int)(c4);

    wireless_uart_send_byte('S');
    wireless_uart_send_byte('T');
    for (i = 0; i < 3; i++)
        for (j = 0; j < 4; j++)
        {
            low = (unsigned char)(send_data[i][j] & 0x00ff);
            high = (unsigned char)(send_data[i][j] >> 8u);
            wireless_uart_send_byte(low); wireless_uart_send_byte(high);
            checksum += low; checksum += high;
            xorsum ^= low; xorsum ^= high;
        }
    wireless_uart_send_byte((unsigned char)(checksum & 0x00ff));
    wireless_uart_send_byte(xorsum);
}

void variableOperate(uint8 variable, uint8 operation){
    switch (variable){
        // angVelPIDx
        case 'a':
            if(operation=='+'){angVelPIDx.pCoef += 10;}
            if(operation=='-'){angVelPIDx.pCoef -= 10;}
            if(operation=='0'){angVelPIDx.pCoef = 0;}
            break;
        case 'b':
            if(operation=='+'){angVelPIDx.iCoef += 1;}
            if(operation=='-'){angVelPIDx.iCoef -= 1;}
            if(operation=='0'){angVelPIDx.iCoef = 0;}
            break;
        case 'c':
            if(operation=='+'){angVelPIDx.dCoef += 1;}
            if(operation=='-'){angVelPIDx.dCoef -= 1;}
            if(operation=='0'){angVelPIDx.dCoef = 0;}
            break;
        // angVelPIDy
        case 'd':
            if(operation=='+'){angVelPIDy.pCoef += 10;}
            if(operation=='-'){angVelPIDy.pCoef -= 10;}
            if(operation=='0'){angVelPIDy.pCoef = 0;}
            break;
        case 'e':
            if(operation=='+'){angVelPIDy.iCoef += 1;}
            if(operation=='-'){angVelPIDy.iCoef -= 1;}
            if(operation=='0'){angVelPIDy.iCoef = 0;}
            break;
        case 'f':
            if(operation=='+'){angVelPIDy.dCoef += 1;}
            if(operation=='-'){angVelPIDy.dCoef -= 1;}
            if(operation=='0'){angVelPIDy.dCoef = 0;}
            break;
        // angVelPIDz
        case 'g':
            if(operation=='+'){angVelPIDz.pCoef += 10;}
            if(operation=='-'){angVelPIDz.pCoef -= 10;}
            if(operation=='0'){angVelPIDz.pCoef = 0;}
            break;
        case 'h':
            if(operation=='+'){angVelPIDz.iCoef += 1;}
            if(operation=='-'){angVelPIDz.iCoef -= 1;}
            if(operation=='0'){angVelPIDz.iCoef = 0;}
            break;
        case 'i':
            if(operation=='+'){angVelPIDz.dCoef += 1;}
            if(operation=='-'){angVelPIDz.dCoef -= 1;}
            if(operation=='0'){angVelPIDz.dCoef = 0;}
            break;
        // angPIDx
        case 'j':
            if(operation=='+'){angPIDx.pCoef += 10;}
            if(operation=='-'){angPIDx.pCoef -= 10;}
            if(operation=='0'){angPIDx.pCoef = 0;}
            break;
        case 'k':
            if(operation=='+'){angPIDx.iCoef += 1;}
            if(operation=='-'){angPIDx.iCoef -= 1;}
            if(operation=='0'){angPIDx.iCoef = 0;}
            break;
        case 'l':
            if(operation=='+'){angPIDx.dCoef += 1;}
            if(operation=='-'){angPIDx.dCoef -= 1;}
            if(operation=='0'){angPIDx.dCoef = 0;}
            break;
        // angPIDy
        case 'm':
            if(operation=='+'){angPIDy.pCoef += 10;}
            if(operation=='-'){angPIDy.pCoef -= 10;}
            if(operation=='0'){angPIDy.pCoef = 0;}
            break;
        case 'n':
            if(operation=='+'){angPIDy.iCoef += 1;}
            if(operation=='-'){angPIDy.iCoef -= 1;}
            if(operation=='0'){angPIDy.iCoef = 0;}
            break;
        case 'o':
            if(operation=='+'){angPIDy.dCoef += 1;}
            if(operation=='-'){angPIDy.dCoef -= 1;}
            if(operation=='0'){angPIDy.dCoef = 0;}
            break;
        // angPIDz
        case 'p':
            if(operation=='+'){angPIDz.pCoef += 10;}
            if(operation=='-'){angPIDz.pCoef -= 10;}
            if(operation=='0'){angPIDz.pCoef = 0;}
            break;
        case 'q':
            if(operation=='+'){angPIDz.iCoef += 1;}
            if(operation=='-'){angPIDz.iCoef -= 1;}
            if(operation=='0'){angPIDz.iCoef = 0;}
            break;
        case 'r':
            if(operation=='+'){angPIDz.dCoef += 1;}
            if(operation=='-'){angPIDz.dCoef -= 1;}
            if(operation=='0'){angPIDz.dCoef = 0;}
            break;
        // velPIDl
        case 's':
            if(operation=='+'){velPIDl.pCoef += 10;}
            if(operation=='-'){velPIDl.pCoef -= 10;}
            if(operation=='0'){velPIDl.pCoef = 0;}
            break;
        case 't':
            if(operation=='+'){velPIDl.iCoef += 1;}
            if(operation=='-'){velPIDl.iCoef -= 1;}
            if(operation=='0'){velPIDl.iCoef = 0;}
            break;
        case 'u':
            if(operation=='+'){velPIDl.dCoef += 1;}
            if(operation=='-'){velPIDl.dCoef -= 1;}
            if(operation=='0'){velPIDl.dCoef = 0;}
            break;
        // velPIDr
        case 'v':
            if(operation=='+'){velPIDr.pCoef += 10;}
            if(operation=='-'){velPIDr.pCoef -= 10;}
            if(operation=='0'){velPIDr.pCoef = 0;}
            break;
        case 'w':
            if(operation=='+'){velPIDr.iCoef += 1;}
            if(operation=='-'){velPIDr.iCoef -= 1;}
            if(operation=='0'){velPIDr.iCoef = 0;}
            break;
        case 'x':
            if(operation=='+'){velPIDr.dCoef += 1;}
            if(operation=='-'){velPIDr.dCoef -= 1;}
            if(operation=='0'){velPIDr.dCoef = 0;}
            break;
        // velPIDy
        case 'y':
            if(operation=='+'){velPIDy.pCoef += 10;}
            if(operation=='-'){velPIDy.pCoef -= 10;}
            if(operation=='0'){velPIDy.pCoef = 0;}
            break;
        case 'z':
            if(operation=='+'){velPIDy.iCoef += 1;}
            if(operation=='-'){velPIDy.iCoef -= 1;}
            if(operation=='0'){velPIDy.iCoef = 0;}
            break;
        case 'A':
            if(operation=='+'){velPIDy.dCoef += 1;}
            if(operation=='-'){velPIDy.dCoef -= 1;}
            if(operation=='0'){velPIDy.dCoef = 0;}
            break;

        // motorLeft PWM
        case 'B':
            if(operation=='+'){setMotor(&motorLeft, PLUS, 1000);}
            if(operation=='-'){setMotor(&motorLeft, MINUS, 1000);}
            if(operation=='0'){setMotor(&motorLeft, ASSIGN, 0);}
            if(operation=='a'){setMotor(&motorLeft, OPPOSE, 0);}
            break;

        // motorRight PWM
        case 'C':
            if(operation=='+'){setMotor(&motorRight, PLUS, 1000);}
            if(operation=='-'){setMotor(&motorRight, MINUS, 1000);}
            if(operation=='0'){setMotor(&motorRight, ASSIGN, 0);}
            if(operation=='a'){setMotor(&motorRight, OPPOSE, 0);}
            break;
        
        case 'D':
            if(operation=='+'){setMotor(&motorBottom, PLUS, 1000);}
            if(operation=='-'){setMotor(&motorBottom, MINUS, 1000);}
            if(operation=='0'){setMotor(&motorBottom, ASSIGN, 0);}
            if(operation=='a'){setMotor(&motorBottom, OPPOSE, 0);}
            break;

        case 'E':
            if(operation=='+'){yawTargetOffset += 5;}
            if(operation=='-'){yawTargetOffset -= 5;}
            if(operation=='0'){yawTargetOffset =  0;}
            #ifdef _DEBUG_BELL_
                gpio_set_level(BELL_PIN, 1); system_delay_us(500); gpio_set_level(BELL_PIN, 0);
            #endif
            break;

        case 'F':
            if(operation=='+'){velPIDy.target += 20;}
            if(operation=='-'){velPIDy.target -= 20;}
            if(operation=='0'){velPIDy.target =  0;}
            #ifdef _DEBUG_BELL_
                gpio_set_level(BELL_PIN, 1); system_delay_us(500); gpio_set_level(BELL_PIN, 0);
            #endif
            break;

        default:
            break;
    }
}

void modeFlagOperate(uint8 modeFlag, uint8 value){
    switch (modeFlag){
        case 'a': // 改变串口发送内容模式
            uartSendMode = value - '0';
            break;
        case 'b': // 改变显示屏显示内容模式
            tft180_clear();
            screenMode = value - '0';
            break;
    }
}

void uartCommandHandler(uint8 data_buffer[32]){
    switch (data_buffer[0]){
        case '0': // 改变变量值
            variableOperate(data_buffer[1], data_buffer[2]);
            break;
        case '1': // 改变单标志位内容
            modeFlagOperate(data_buffer[1], data_buffer[2]);
            break;
    }
}
