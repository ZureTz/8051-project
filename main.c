#include <reg52.h>

#include "utils.h"
#include "delay.h"

sbit led = P2 ^ 4;

void main(void)
{
  while (true)
  {
    led = 0; // Turn on LED
    delayMiliseconds(2000);
    led = 1; // Turn off LED
    delayMiliseconds(1000);
  }
}
