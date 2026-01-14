#ifndef FONT_TABLE_H
#define FONT_TABLE_H

/**
 * @brief Get LCD1602 character code for any ASCII character
 * @param ascii_char The ASCII character (number, letter, or symbol)
 * @return LCD1602 character code (same as ASCII for standard characters)
 *         For digits 0-9: 0x30-0x39 (b7-b4=0011, b3-b0=digit binary)
 *         For letters A-Z: 0x41-0x5A, a-z: 0x61-0x7A
 *         Returns the input character if valid, 0x20 (space) if invalid
 */
unsigned char getLCD1602CharCode(unsigned char ascii_char);

#endif // FONT_TABLE_H