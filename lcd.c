#include "lcd.h"
#include "delay.h"
#include "font_table.h"

/**
 * @brief LCD enable signal
 */
static void LCD_Enable(void)
{
  LCD_EN = 1;
  delayMicroseconds(1); // Enable signal high level for at least 450ns
  LCD_EN = 0;
  delayMicroseconds(1); // Enable signal low level hold
}

/**
 * @brief Write command to LCD
 * @param cmd Command byte
 */
void LCD_WriteCmd(unsigned char cmd)
{
  LCD_RS = 0;            // Select instruction register
  LCD_RW = 0;            // Write mode
  LCD_DATA = cmd;        // Put command on data bus
  LCD_Enable();          // Generate enable signal
  delayMicroseconds(50); // Wait for command execution
}

/**
 * @brief Write data to LCD
 * @param dat Data byte
 */
void LCD_WriteData(unsigned char dat)
{
  LCD_RS = 1;            // Select data register
  LCD_RW = 0;            // Write mode
  LCD_DATA = dat;        // Put data on data bus
  LCD_Enable();          // Generate enable signal
  delayMicroseconds(50); // Wait for data write completion
}

/**
 * @brief Initialize LCD1602
 */
void LCD_Init(void)
{
  delayMiliseconds(15); // Wait for LCD power-on stabilization

  LCD_WriteCmd(LCD_FUNCTION_SET); // Function set: 8-bit data, 2 lines, 5x7 dots
  delayMicroseconds(50);

  LCD_WriteCmd(LCD_DISPLAY_ON); // Display on, cursor off, no blink
  delayMicroseconds(50);

  LCD_WriteCmd(LCD_CLEAR); // Clear screen
  delayMiliseconds(2);     // Clear command needs longer time

  LCD_WriteCmd(LCD_ENTRY_MODE); // Cursor moves right, display does not shift
  delayMicroseconds(50);
}

/**
 * @brief Set cursor position
 * @param row Row number (0 or 1)
 * @param col Column number (0-15)
 */
void LCD_SetCursor(unsigned char row, unsigned char col)
{
  unsigned char addr;

  if (row == 0)
  {
    addr = 0x00 + col; // First line address range: 0x00-0x0F
  }
  else
  {
    addr = 0x40 + col; // Second line address range: 0x40-0x4F
  }

  LCD_WriteCmd(LCD_SET_DDRAM_ADDR | addr); // Set DDRAM address
}

/**
 * @brief Clear screen
 */
void LCD_Clear(void)
{
  LCD_WriteCmd(LCD_CLEAR);
  delayMiliseconds(2); // Clear command needs longer time
}

/**
 * @brief Display a character on LCD
 * @param c Character to display
 */
void LCD_ShowChar(unsigned char c)
{
  unsigned char charCode = getLCD1602CharCode(c);
  LCD_WriteData(charCode);
}

/**
 * @brief Display a string on LCD
 * @param str String to display
 */
void LCD_ShowString(unsigned char *str)
{
  while (*str != '\0')
  {
    LCD_ShowChar(*str);
    str++;
  }
}

/**
 * @brief Display a character at specified position
 * @param row Row number (0 or 1)
 * @param col Column number (0-15)
 * @param c Character to display
 */
void LCD_ShowCharAt(unsigned char row, unsigned char col, unsigned char c)
{
  LCD_SetCursor(row, col);
  LCD_ShowChar(c);
}

/**
 * @brief Display a string at specified position
 * @param row Row number (0 or 1)
 * @param col Column number (0-15)
 * @param str String to display
 */
void LCD_ShowStringAt(unsigned char row, unsigned char col, unsigned char *str)
{
  LCD_SetCursor(row, col);
  LCD_ShowString(str);
}
