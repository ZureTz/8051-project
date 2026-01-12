#include <reg52.h>

#include "utils.h"
#include "delay.h"

// 端口定义
sfr key = 0x80; // P0口 - 按键输入 (K1-K8 连接到 P0.0-P0.7)
sfr led = 0xA0; // P2口 - LED输出 (D1-D8 连接到 P2.0-P2.7)

void main(void)
{
  // 初始化
  key = 0xFF; // P0口设置为输入模式（上拉）
  led = 0xFF; // 初始化时关闭所有LED

  while (true)
  {
    // 读取按键状态并直接输出到LED
    // 按键按下时为低电平(0)，LED低电平点亮(0)
    led = key; // 按键按下(0)时，对应LED点亮(0)

    // 短暂延时消抖
    delayMiliseconds(10);
  }
}
