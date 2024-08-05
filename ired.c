#include <mcs51/8052.h>

// 接收红外信号

#include "ired.h"
#include "tools.h"

unsigned char ired_data[4] = {0, 0, 0, 0};

void ired_init()
{
    IT0 = 1; // INT0下降沿触发
    EX0 = 1; // 开启INT0中断
    EA = 1;  // 开启总中断
}


void ired_isr() __interrupt(0)
{
}