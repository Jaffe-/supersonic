#include <avr/io.h>
#include "clock.h"

void setup_clock()
{
  DDRD = 1 << 6;
  TCCR0A = (1 << COM0A0) | (1 << WGM01);
  TCCR0B = 1 << CS00;
  OCR0A = 100;
}

