#include <reg52.h>

#include "utils.h"
#include "delay.h"

// define buzzer pin
sbit BUZZER = P1 ^ 5;

// Musical note frequencies (half period in microseconds for middle C octave)
// Half period = 1/(2*frequency) * 1000000
#define NOTE_1_DO 1908  // C4 - 262 Hz
#define NOTE_2_RE 1701  // D4 - 294 Hz
#define NOTE_3_MI 1515  // E4 - 330 Hz
#define NOTE_4_FA 1433  // F4 - 349 Hz
#define NOTE_5_SOL 1276 // G4 - 392 Hz
#define NOTE_6_LA 1136  // A4 - 440 Hz
#define NOTE_7_SI 1012  // B4 - 494 Hz
#define NOTE_REST 0     // Rest/silence

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

// Happy Birthday melody
// Based on sheet music: 5 5 6 5 1 7, 5 5 6 5 2 1, 5 5 5 3 1 7 6, 4 4 3 1 2 1
unsigned char code melody[] = {
    5, 5, 6, 5, 1, 7,    // Happy birthday to you
    5, 5, 6, 5, 2, 1,    // Happy birthday to you
    5, 5, 5, 3, 1, 7, 6, // Happy birthday dear...
    4, 4, 3, 1, 2, 1     // Happy birthday to you
};

// Note durations in milliseconds (rests merged into previous notes)
unsigned int code durations[] = {
    300, 300, 600, 600, 600, 1100,      // First line (7 extended by 200)
    300, 300, 600, 600, 600, 1100,      // Second line (1 extended by 200)
    300, 300, 600, 600, 600, 600, 1100, // Third line (6 extended by 200)
    600, 600, 600, 600, 600, 1200       // Fourth line
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
