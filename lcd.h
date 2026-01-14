#ifndef LCD_H
#define LCD_H

#include <reg52.h>

// LCD1602 Pin Definitions
#define LCD_DATA P0   // 8-bit data bus DB0-DB7
sbit LCD_RW = P2 ^ 5; // R/W: Read/Write Select (0=Write, 1=Read)
sbit LCD_RS = P2 ^ 6; // RS: Register Select (0=Instruction, 1=Data)
sbit LCD_EN = P2 ^ 7; // E: Enable Signal

// LCD1602 Command Definitions
#define LCD_CLEAR 0x01          // Clear screen
#define LCD_CURSOR_RETURN 0x02  // Return cursor to home
#define LCD_ENTRY_MODE 0x06     // Cursor moves right, display does not shift
#define LCD_DISPLAY_ON 0x0C     // Display on, cursor off, no blink
#define LCD_DISPLAY_OFF 0x08    // Display off
#define LCD_CURSOR_ON 0x0E      // Display on, cursor on, no blink
#define LCD_FUNCTION_SET 0x38   // 8-bit data, 2 lines, 5x7 dots
#define LCD_SET_DDRAM_ADDR 0x80 // Set DDRAM address

/**
 * @brief Initialize LCD1602
 */
void LCD_Init(void);

/**
 * @brief Write command to LCD
 * @param cmd Command byte
 */
void LCD_WriteCmd(unsigned char cmd);

/**
 * @brief Write data to LCD
 * @param dat Data byte
 */
void LCD_WriteData(unsigned char dat);

/**
 * @brief Set cursor position
 * @param row Row number (0 or 1)
 * @param col Column number (0-15)
 */
void LCD_SetCursor(unsigned char row, unsigned char col);

/**
 * @brief Clear screen
 */
void LCD_Clear(void);

/**
 * @brief Display a character on LCD
 * @param c Character to display
 */
void LCD_ShowChar(unsigned char c);

/**
 * @brief Display a string on LCD
 * @param str String to display
 */
void LCD_ShowString(unsigned char *str);

/**
 * @brief Display a character at specified position
 * @param row Row number (0 or 1)
 * @param col Column number (0-15)
 * @param c Character to display
 */
void LCD_ShowCharAt(unsigned char row, unsigned char col, unsigned char c);

/**
 * @brief Display a string at specified position
 * @param row Row number (0 or 1)
 * @param col Column number (0-15)
 * @param str String to display
 */
void LCD_ShowStringAt(unsigned char row, unsigned char col, unsigned char *str);

#endif // LCD_H