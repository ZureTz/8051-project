/**
 * @brief Get LCD1602 character code for any ASCII character
 * @param ascii_char The ASCII character (number, letter, or symbol)
 * @return LCD1602 character code
 */
unsigned char getLCD1602CharCode(unsigned char ascii_char)
{
  // LCD1602 uses standard ASCII encoding:
  // Numbers 0-9: 0x30-0x39 (0011 0000 to 0011 1001)
  // Upper case A-Z: 0x41-0x5A
  // Lower case a-z: 0x61-0x7A
  // Common symbols: space=0x20, etc.

  // Validate input is in printable ASCII range (0x20-0x7E)
  if (ascii_char >= 0x20 && ascii_char <= 0x7E)
  {
    return ascii_char;
  }

  // Return space character for invalid input
  return 0x20;
}
