#include <reg52.h>

#include "utils.h"
#include "delay.h"

// Port definitions
sfr key = 0x80; // Port 0 - Key input (K1-K8 connected to P0.0-P0.7)
sfr led = 0xA0; // Port 2 - LED output (D1-D8 connected to P2.0-P2.7)

// Reverse function to reverse bits in a byte
unsigned char reverseBitsInNum(unsigned char value)
{
  unsigned char result = 0;
  unsigned char i;

  for (i = 0; i < 8; i++)
  {
    result <<= 1;
    result |= (value & 0x01);
    value >>= 1;
  }

  return result;
}

void main(void)
{
  // Initialize ports
  key = 0xFF; // Set Port 0 as input mode (pull-up)
  led = 0xFF; // Turn off all LEDs initially

  while (true)
  {
    // First read key state
    // Not pressed, skip
    if (key == 0xFF)
    {
      continue;
    }

    // Press detected, wait for debounce
    delayMiliseconds(20);

    // Make sure the key is still pressed after debounce delay
    if (key == 0xFF)
    {
      continue;
    }

    // Read key state and output to LED
    // Reverse bits before output
    // Flip the LED bits corresponding to pressed keys
    led ^= ~reverseBitsInNum(key);

    // Wait until key is released
    while (key != 0xFF)
      continue;
  }
}
