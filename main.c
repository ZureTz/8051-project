#include <reg52.h>

#include "utils.h"
#include "delay.h"
#include "lcd.h"
#include "keyboard.h"
#include "calculator.h"

void main(void)
{
  unsigned char key;
  char xdata displayBuffer[17];   // Display window buffer (16 characters + \0)
  char xdata resultBuffer[17];    // Result buffer (16 characters + \0)
  unsigned char scrollOffset = 0; // Current scroll offset
  unsigned char maxScrollOffset;
  unsigned char autoScroll = 1; // Auto scroll to right after input

  // Initialize LCD1602
  LCD_Init();

  // Initialize Keyboard
  Keyboard_Init();

  // Initialize Calculator
  Calculator_Init();

  // Clear display
  LCD_ShowStringAt(0, 0, "                ");
  LCD_ShowStringAt(1, 0, "                ");
  LCD_SetCursor(0, 0);

  while (true)
  {
    // Scan keyboard
    key = Keyboard_Scan();
    // No key pressed
    if (key == KEY_NONE)
    {
      continue;
    }

    // Handle scroll left (K5)
    if (key == KEY_SCROLL_LEFT_CHAR)
    {
      if (scrollOffset > 0)
      {
        scrollOffset--;
        autoScroll = 0; // Disable auto scroll
      }
      // Update display
      Calculator_GetDisplayWindow(displayBuffer, scrollOffset);
      LCD_ShowStringAt(0, 0, displayBuffer);
    }
    // Handle scroll right (K6)
    else if (key == KEY_SCROLL_RIGHT_CHAR)
    {
      maxScrollOffset = Calculator_GetMaxScrollOffset();
      if (scrollOffset < maxScrollOffset)
      {
        scrollOffset++;
        autoScroll = 0; // Disable auto scroll
      }
      // Update display
      Calculator_GetDisplayWindow(displayBuffer, scrollOffset);
      LCD_ShowStringAt(0, 0, displayBuffer);
    }
    // Handle backspace
    else if (key == KEY_BACKSPACE_CHAR)
    {
      Calculator_Backspace();
      autoScroll = 1; // Re-enable auto scroll

      // Auto scroll to right after input
      maxScrollOffset = Calculator_GetMaxScrollOffset();
      scrollOffset = maxScrollOffset;

      // Update display
      Calculator_GetDisplayWindow(displayBuffer, scrollOffset);
      LCD_ShowStringAt(0, 0, displayBuffer);

      // Clear second line (result)
      LCD_ShowStringAt(1, 0, "                ");
    }
    // Handle clear
    else if (key == KEY_CLEAR)
    {
      Calculator_Clear();
      scrollOffset = 0;
      autoScroll = 1;

      // Clear both lines on LCD
      LCD_ShowStringAt(0, 0, "                ");
      LCD_ShowStringAt(1, 0, "                ");
      LCD_SetCursor(0, 0);
    }
    // Handle equals (evaluate expression)
    else if (key == '=')
    {
      // Evaluate expression
      Calculator_Evaluate(resultBuffer);

      // Display result on second line
      LCD_ShowStringAt(1, 0, "                ");
      LCD_ShowStringAt(1, 0, resultBuffer);
    }
    // Handle normal input character
    else
    {
      // Try to add character to expression
      if (Calculator_InputChar(key))
      {
        autoScroll = 1; // Re-enable auto scroll

        // Auto scroll to rightmost position after input
        maxScrollOffset = Calculator_GetMaxScrollOffset();
        scrollOffset = maxScrollOffset;

        // Update display with scroll window
        Calculator_GetDisplayWindow(displayBuffer, scrollOffset);
        LCD_ShowStringAt(0, 0, displayBuffer);

        // Clear second line (prepare to show new result)
        LCD_ShowStringAt(1, 0, "                ");
      }
      // If input failed (buffer full or invalid char), ignore
    }

    delayMiliseconds(10); // Small delay for main loop
  }
}
