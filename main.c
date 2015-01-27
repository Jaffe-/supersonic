#include <avr/io.h>

#define F_CPU 8000000L

#include <util/delay.h>
#include "pulse.h"

int main()
{
  setup_pulse();
  while (1) {
    _delay_ms(8);
    pulse(2000);
  }
}
