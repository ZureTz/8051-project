#include <reg52.h>

#include "utils.h"
#include "delay.h"
#include "lcd.h"

void main(void)
{
  // Initialize LCD1602
  LCD_Init();

  // Display "Hello, World!" on first line
  LCD_ShowStringAt(0, 0, "Hello, World!");

  // Display "LCD1602 Demo" on second line
  LCD_ShowStringAt(1, 0, "LCD1602 Demo");

  while (true)
  {
    // Main loop keeps display
  }
}
