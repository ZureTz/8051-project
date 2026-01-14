#include <intrins.h> /* for _nop_() */

static void delay10us(void) //@12.000MHz
{
  unsigned char data i;

  _nop_();
  i = 2;
  while (--i)
    ;
}

static void delay100us(void) //@12.000MHz
{
  unsigned char data i;

  _nop_();
  i = 47;
  while (--i)
    ;
}

// 微秒级延时函数
void delayMicroseconds(unsigned int microseconds)
{
  unsigned int count;

  // 每10微秒调用一次delay10us
  count = microseconds / 10;
  while (count--)
  {
    delay10us();
  }
}

// Call delay100us 10 times to make 1 millisecond
void delayMiliseconds(unsigned long miliseconds)
{
  // i is a counter for 100us delay
  unsigned char i;

  while (miliseconds--)
  {
    for (i = 0; i < 10; i++)
    {
      delay100us();
    }
  }
}
