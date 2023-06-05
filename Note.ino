#include "Note.h"

void Note::init(void) {
  status = END;
}
void Note::init(uint8_t _waveform, uint8_t _note, uint8_t _octave) {
  counter._16 = 0;
  cursor = startAddressTable[_waveform];
  period = periodTable[_note] >> _octave;
  sustainStart = sustainStartTable[_waveform];
  sustainEnd = sustainEndTable[_waveform];
  noteEnd = endAddressTable[_waveform];
  status = SUSTAIN;
}
int8_t Note::sample(void) {
  counter._16 += period;
  cursor += counter._8[1];
  counter._8[1] = 0;
  if (cursor >= sustainEnd && status == SUSTAIN) {
    cursor = sustainStart;
    return pgm_read_byte_near(cursor);
  }
  if (cursor >= noteEnd) {
    status = END;
    return 0;
  }
  return pgm_read_byte_near(cursor);
}
uint8_t findAvailableNote(void) {
  for (uint8_t i = 0;; ++i) {
    if (notes[i].status != SUSTAIN)return i;
  }
}
