#include "0_headers.h"
MeLineFollower lineFinder(PORT_2);  // assigning lineFinder to RJ25 port 2
MeBuzzer buzzer;

bool reached_paper() {
  int sensorState = lineFinder.readSensors();  // read the line sensor's state

  if (sensorState == 3) {       //S1_OUT_S2_OUT - i.e robot does not detect any black line; not above colored paper
    return false;
  }
  return true;                  // for all other cases: black line detected; above colored paper
}

void celebrate() {
  // --- Megalovania Main Theme ---

  // Define the correct note frequencies (Hz) in the C/C# range
  const int C4 = 262;   // C4
  const int D4 = 294;   // D4
  const int F4 = 349;   // F4
  const int G4 = 392;   // G4
  const int AS4 = 466;  // A#4 (Bb4)
  const int C5 = 523;   // C5
  const int C6 = 1047;  // C6 (Kept for the ending of Measure 2)

  // Define duration units (160ms = Quarter Note)
  const int TEMPO = 460;
  const int SIXT = TEMPO / 4;   // 40ms (Sixteenth Note)
  const int EIGHT = TEMPO / 2;  // 80ms (Eighth Note)

  // Helper to calculate total tone duration (Note + Gap)
  // Assuming 'buzzer' and 'delay' are correctly defined elsewhere
  auto playNote = [&](int freq, int duration) {
    // Play tone for 90% of duration
    buzzer.tone(freq, duration * 0.9);
    delay(duration);
    buzzer.noTone();
  };

  // --- Melody Sequence ---

  // Measure 1: C-C-C-A#-G-F | D-C

  // 1 (C5)
  playNote(C5, SIXT);
  // 2 (C5)
  playNote(C5, SIXT);
  // 3 (C5 - Eighth Note)
  playNote(C5, EIGHT);
  // 4 (A#4 - Eighth Note)
  playNote(AS4, EIGHT);
  // 5 (G4 - Eighth Note)
  playNote(G4, EIGHT);
  // 6 (F4)
  playNote(F4, SIXT);
  // 7 (D4)
  playNote(D4, SIXT);
  // 8 (C4 - Eighth Note)
  playNote(C4, EIGHT);

  // Measure 2: C-C-C-A#-G-F | C6-A#4

  // 9 (C5)
  playNote(C5, SIXT);
  // 10 (C5)
  playNote(C5, SIXT);
  // 11 (C5 - Eighth Note)
  playNote(C5, EIGHT);
  // 12 (A#4 - Eighth Note)
  playNote(AS4, EIGHT);
  // 13 (G4 - Eighth Note)
  playNote(G4, EIGHT);
  // 14 (F4)
  playNote(F4, SIXT);
  // 15 (C6 - High Octave Note)
  playNote(C6, SIXT);
  // 16 (A#4 - Dotted Eighth Note = 3 Sixteenths)
  playNote(AS4, SIXT * 3);

  buzzer.noTone();
}