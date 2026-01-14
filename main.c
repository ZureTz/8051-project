#include <reg52.h>

#include "utils.h"
#include "delay.h"

// define buzzer pin
sbit BUZZER = P1 ^ 5;

// Musical note frequencies (half period in microseconds, raised by one octave)
// Half period = 1/(2*frequency) * 1000000
#define NOTE_1_DO 954  // C5 - 523 Hz (raised one octave)
#define NOTE_2_RE 851  // D5 - 587 Hz
#define NOTE_3_MI 758  // E5 - 659 Hz
#define NOTE_4_FA 716  // F5 - 698 Hz
#define NOTE_5_SOL 638 // G5 - 784 Hz
#define NOTE_6_LA 568  // A5 - 880 Hz
#define NOTE_7_SI 506  // B5 - 988 Hz
#define NOTE_REST 0    // Rest/silence

// Duration for each note in milliseconds
#define NOTE_DURATION 500

/**
 * @brief Play a single tone by generating square wave
 * @param halfPeriod Half period of the square wave in microseconds (0 for silence)
 * @param duration Duration to play the tone in milliseconds
 */
void playTone(unsigned int halfPeriod, unsigned int duration)
{
  unsigned int cycles;

  if (halfPeriod == 0)
  {
    // Rest - no sound
    delayMiliseconds(duration);
    return;
  }

  // Calculate number of cycles needed for the duration
  // cycles = duration_ms * 1000 / (halfPeriod_us * 2)
  cycles = ((unsigned long)duration * 1000UL) / ((unsigned long)halfPeriod * 2);

  while (cycles--)
  {
    BUZZER = 1;
    delayMicroseconds(halfPeriod);
    BUZZER = 0;
    delayMicroseconds(halfPeriod);
  }
}

/**
 * @brief Play a musical note by its number (1-7)
 * @param note Note number (1=Do, 2=Re, 3=Mi, 4=Fa, 5=Sol, 6=La, 7=Si, 0=Rest)
 * @param duration Duration to play the note in milliseconds
 */
void playNote(unsigned char note, unsigned int duration)
{
  unsigned int halfPeriod;

  switch (note)
  {
  case 1:
    halfPeriod = NOTE_1_DO;
    break;
  case 2:
    halfPeriod = NOTE_2_RE;
    break;
  case 3:
    halfPeriod = NOTE_3_MI;
    break;
  case 4:
    halfPeriod = NOTE_4_FA;
    break;
  case 5:
    halfPeriod = NOTE_5_SOL;
    break;
  case 6:
    halfPeriod = NOTE_6_LA;
    break;
  case 7:
    halfPeriod = NOTE_7_SI;
    break;
  default:
    halfPeriod = NOTE_REST;
    break;
  }

  playTone(halfPeriod, duration);
}

// Last Christmas melody - main chorus (faster tempo)
// Notes: G G F C G G A F, D D G G A F D E F E D
// Converted to numbers: 5 5 4 1 5 5 6 4, 2 2 5 5 6 4 2 3 4 3 2
unsigned char code melody[] = {
    5, 5, 4, 1, 5, 5, 6, 4,          // "Last Christmas, I gave you my heart"
    2, 2, 5, 5, 6, 4, 2, 3, 4, 3, 2, // "But the very next day, you gave it away"
    6, 5, 2, 6, 7, 6, 5,             // "This year, to save me from tears"
    4, 3, 4, 3, 3, 4, 3, 1           // "I'll give it to someone special"
};

// Note durations in milliseconds (faster tempo)
unsigned int code durations[] = {
    250, 120, 180, 180, 250, 120, 250, 400,                // First line
    250, 120, 120, 120, 180, 180, 180, 120, 120, 120, 400, // Second line
    250, 180, 120, 180, 180, 180, 400,                     // Third line
    180, 120, 120, 120, 120, 120, 120, 500                 // Fourth line
};

void main(void)
{
  unsigned char i;
  unsigned char melodyLength = sizeof(melody) / sizeof(melody[0]);

  while (true)
  {
    // Play the melody
    for (i = 0; i < melodyLength; i++)
    {
      playNote(melody[i], durations[i]);
      delayMiliseconds(50); // Short pause between notes
    }

    // Longer pause before repeating
    delayMiliseconds(2000);
  }
}
