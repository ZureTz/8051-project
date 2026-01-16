#include "keyboard.h"
#include "delay.h"

// Matrix keypad layout mapping
// Row 0: 1  2  3  +
// Row 1: 4  5  6  -
// Row 2: 7  8  9  *
// Row 3: C  0  =  /
static unsigned char code matrixKeyMap[4][4] = {
    {KEY_1, KEY_2, KEY_3, KEY_ADD},
    {KEY_4, KEY_5, KEY_6, KEY_SUB},
    {KEY_7, KEY_8, KEY_9, KEY_MUL},
    {KEY_CLEAR, KEY_0, KEY_EQUAL, KEY_DIV}};

/**
 * @brief Initialize keyboard module
 */
void Keyboard_Init(void)
{
  // Set P1 as input (for matrix keypad)
  MATRIX_KEYPAD = 0xFF;

  // Set P3 as input (for independent keys)
  P3 = 0xFF;
}

/**
 * @brief Scan matrix keypad (4x4)
 * @return Key code if pressed, KEY_NONE if no key pressed
 */
unsigned char Keyboard_ScanMatrix(void)
{
  unsigned char row, col;
  unsigned char keyValue = KEY_NONE;

  // Scan each row
  for (row = 0; row < 4; row++)
  {
    // Set all rows high, then set current row low
    MATRIX_KEYPAD = 0xFF;
    MATRIX_KEYPAD &= ~(0x01 << row); // Pull current row low

    // Small delay for signal stabilization
    delayMicroseconds(10);

    // Check each column
    for (col = 0; col < 4; col++)
    {
      // Check if column is low (key pressed)
      if ((MATRIX_KEYPAD & (0x10 << col)) != 0)
      {
        continue;
      }

      // Debounce delay
      delayMiliseconds(10);

      // Confirm key is still pressed
      if ((MATRIX_KEYPAD & (0x10 << col)) != 0)
      {
        continue;
      }

      // Key is confirmed pressed, get key value
      keyValue = matrixKeyMap[row][col];

      // Wait for key release
      while ((MATRIX_KEYPAD & (0x10 << col)) == 0)
        ;

      // Debounce delay after release
      delayMiliseconds(10);

      return keyValue;
    }
  }

  // Restore P1 to all high
  MATRIX_KEYPAD = 0xFF;

  return KEY_NONE;
}

/**
 * @brief Scan independent keys
 * @return Key code if pressed, KEY_NONE if no key pressed
 */
unsigned char Keyboard_ScanIndependent(void)
{
  // Check dot key
  if (KEY_DOT == 0)
  {
    delayMiliseconds(10); // Debounce
    if (KEY_DOT != 0)
    {
      return KEY_NONE;
    }

    while (KEY_DOT == 0)
      ; // Wait for release
    delayMiliseconds(10);
    return KEY_DOT_CHAR;
  }

  // Check left parenthesis key
  if (KEY_LEFT_PAREN == 0)
  {
    delayMiliseconds(10);
    if (KEY_LEFT_PAREN != 0)
    {
      return KEY_NONE;
    }

    while (KEY_LEFT_PAREN == 0)
      ;
    delayMiliseconds(10);
    return KEY_LEFT_PAREN_CHAR;
  }

  // Check right parenthesis key
  if (KEY_RIGHT_PAREN == 0)
  {
    delayMiliseconds(10);
    if (KEY_RIGHT_PAREN != 0)
    {
      return KEY_NONE;
    }

    while (KEY_RIGHT_PAREN == 0)
      ;
    delayMiliseconds(10);
    return KEY_RIGHT_PAREN_CHAR;
  }

  // Check backspace key
  if (KEY_BACKSPACE == 0)
  {
    delayMiliseconds(10);
    if (KEY_BACKSPACE != 0)
    {
      return KEY_NONE;
    }

    while (KEY_BACKSPACE == 0)
      ;
    delayMiliseconds(10);
    return KEY_BACKSPACE_CHAR;
  }

  // Check scroll left key (K5)
  if (KEY_SCROLL_LEFT == 0)
  {
    delayMiliseconds(10);
    if (KEY_SCROLL_LEFT != 0)
    {
      return KEY_NONE;
    }

    while (KEY_SCROLL_LEFT == 0)
      ;
    delayMiliseconds(10);
    return KEY_SCROLL_LEFT_CHAR;
  }

  // Check scroll right key (K6)
  if (KEY_SCROLL_RIGHT == 0)
  {
    delayMiliseconds(10);
    if (KEY_SCROLL_RIGHT != 0)
    {
      return KEY_NONE;
    }

    while (KEY_SCROLL_RIGHT == 0)
      ;
    delayMiliseconds(10);
    return KEY_SCROLL_RIGHT_CHAR;
  }

  return KEY_NONE;
}

/**
 * @brief Scan all keyboards (matrix + independent)
 * @return Key code if pressed, KEY_NONE if no key pressed
 */
unsigned char Keyboard_Scan(void)
{
  unsigned char key;

  // First scan matrix keypad
  key = Keyboard_ScanMatrix();

  // If matrix key pressed, early return
  if (key != KEY_NONE)
  {
    return key;
  }

  // Then scan independent keys
  key = Keyboard_ScanIndependent();
  return key;
}
