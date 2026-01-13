#include <reg52.h>

#include "utils.h"
#include "delay.h"

// External interrupt pins
sbit K3 = P3 ^ 2; // INT0 - External interrupt 0
sbit K4 = P3 ^ 3; // INT1 - External interrupt 1

// Bit select control pins (138 decoder)
sbit LED_A = P2 ^ 2;
sbit LED_B = P2 ^ 3;
sbit LED_C = P2 ^ 4;

// Common cathode 7-segment display encoding (0-9)
unsigned char code digitTable[] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F  // 9
};

// Counter variables

// K3 (left 4 digits) shows fibonacci-like sequence
unsigned int prevprev0 = 0; // PrevPrev for K3 (left 4 digits)
unsigned int prev0 = 1;     // Prev for K3 (left 4 digits)
unsigned int sum0 = 0;      // Sum for K3 based on previous count
// K4 (right 4 digits) is a simple incrementing counter
unsigned int count1 = 0; // Count for K4 (right 4 digits)

// Key press flags
bit key0Pressed = false; // K3 pressed flag
bit key1Pressed = false; // K4 pressed flag

// Display buffer for 8 digits
unsigned char displayBuf[8] = {0, 0, 0, 0, 0, 0, 0, 0};

// Function to update display buffer from counter values
void updateDisplayBuffer(unsigned int num0, unsigned int num1)
{
  // Left 4 digits show count0
  displayBuf[0] = num0 / 1000;
  displayBuf[1] = (num0 / 100) % 10;
  displayBuf[2] = (num0 / 10) % 10;
  displayBuf[3] = num0 % 10;

  // Right 4 digits show count1
  displayBuf[4] = num1 / 1000;
  displayBuf[5] = (num1 / 100) % 10;
  displayBuf[6] = (num1 / 10) % 10;
  displayBuf[7] = num1 % 10;
}

// Function to select which digit to display (0-7)
void selectDigit(unsigned char digit)
{
  // Use 3-bit ABC to select digit through 138 decoder
  LED_A = digit & 0x01;
  LED_B = (digit >> 1) & 0x01;
  LED_C = (digit >> 2) & 0x01;
}

// External interrupt 0 service routine (K3)
void INT0_ISR(void) interrupt 0
{
  unsigned int i;

  // Debounce: simple delay loop (~2ms)
  for (i = 0; i < 2000; i++)
    continue;

  // Check if key is still pressed after debounce
  if (K3 == 0 && !key0Pressed)
  {
    key0Pressed = true; // Set flag, will be processed in main loop
  }
}

// External interrupt 1 service routine (K4)
void INT1_ISR(void) interrupt 2
{
  unsigned int i;

  // Debounce: simple delay loop (~2ms)
  for (i = 0; i < 2000; i++)
    continue;

  // Check if key is still pressed after debounce
  if (K4 == 0 && !key1Pressed)
  {
    key1Pressed = true; // Set flag, will be processed in main loop
  }
}

void main(void)
{
  unsigned char i;

  // Initialize display buffer
  updateDisplayBuffer(sum0, count1);

  // Configure external interrupts
  IT0 = 1; // INT0 triggered by falling edge
  IT1 = 1; // INT1 triggered by falling edge
  EX0 = 1; // Enable INT0
  EX1 = 1; // Enable INT1
  EA = 1;  // Enable global interrupt

  // Main loop: dynamic scanning display
  while (true)
  {
    // Display scanning loop
    for (i = 0; i < 8; i++)
    {
      selectDigit(i);                 // Select digit
      P0 = digitTable[displayBuf[i]]; // Output segment data
      delayMiliseconds(2);            // Display for 2ms
      P0 = 0x00;                      // Turn off to prevent ghosting
    }

    // Check K3 key release and update
    if (key0Pressed)
    {
      prevprev0 = prev0;
      prev0 = sum0;

      // Calculate next Fibonacci-like number (capped at 9999)
      sum0 = prevprev0 + prev0 > 9999 ? 9999 : prevprev0 + prev0;

      updateDisplayBuffer(sum0, count1);
      key0Pressed = false; // Clear flag
    }

    // Check K4 key release and update
    if (key1Pressed)
    {
      count1 = (count1 + 1) > 9999 ? 0 : (count1 + 1);
      updateDisplayBuffer(sum0, count1);
      key1Pressed = false; // Clear flag
    }
  }
}
