#include <reg52.h>

#include "utils.h"
#include "delay.h"

// 直接操作端口来同时控制所有LED
sfr led = 0xA0; // led端口地址

void main(void)
{
  while (true)
  {
    led = 0x00; // 亮起所有LED（设置为0）
    delayMiliseconds(2000);
    led = 0xFF; // 关闭所有LED（设置为1）
    delayMiliseconds(1000);
  }
}
