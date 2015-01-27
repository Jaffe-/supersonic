#include <avr/io.h>

#define F_CPU 8000000

#include <avr/delay.h>
#include "pulse.h"

int main()
{
  setup_pulse();
  while (1) {
    _delay_ms(2);
    pulse(500);
  }
}
