#ifndef _UPPERCOMPUTER_H_
#define _UPPERCOMPUTER_H_

#include "zf_common_headfile.h"
#include "motor.h"
extern uint8 uartSendMode;
extern uint8 screenMode;
void wireless_uart_LingLi_send(
        uint16 a1, uint16 a2, uint16 a3, uint16 a4,
        uint16 b1, uint16 b2, uint16 b3, uint16 b4,
        uint16 c1, uint16 c2, uint16 c3, uint16 c4
);
void variableOperate(uint8 variable, uint8 operation);
void modeFlagOperate(uint8 modeFlag, uint8 value);
void uartCommandHandler(uint8 data_buffer[32]);


#endif
