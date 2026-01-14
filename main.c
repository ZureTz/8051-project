#include <reg52.h>

#include "utils.h"
#include "delay.h"
#include "lcd.h"
#include "keyboard.h"

void main(void)
{
  unsigned char key;
  unsigned char cursorPos = 0;

  // Initialize LCD1602
  LCD_Init();

  // Initialize Keyboard
  Keyboard_Init();

  // Display prompt
  LCD_ShowStringAt(0, 0, "Press any key:");
  LCD_SetCursor(1, 0);

  while (true)
  {
    // Scan keyboard
    key = Keyboard_Scan();
    // No key pressed
    if (key == KEY_NONE)
    {
      continue;
    }

    // Handle backspace
    if (key == KEY_BACKSPACE_CHAR && cursorPos > 0)
    {
      cursorPos--;
      LCD_ShowCharAt(1, cursorPos, ' '); // Clear character
      LCD_SetCursor(1, cursorPos);
    }
    // Handle clear
    else if (key == KEY_CLEAR)
    {
      // Clear second line
      LCD_ShowStringAt(1, 0, "                ");
      cursorPos = 0;
      LCD_SetCursor(1, 0);
    }
    // Display normal character
    else if (cursorPos < 16)
    {
      LCD_ShowChar(key);
      cursorPos++;
    }

    delayMiliseconds(10); // Small delay for main loop
  }
}
