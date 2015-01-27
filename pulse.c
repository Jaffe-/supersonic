/* 
   Pulsgenerator
 */

#include <avr/io.h>
#include "pulse.h"

#define F_CPU 8000000

void stop_pulse()
{
  OCR0A &= ~(1 << COM0A0);
}

void start_pulse()
{
  OCR0A |= 1 << COM0A0;
}

void pulse(uint16_t time)
/* time is value in micro seconds */
{
  OCR1A = F_CPU / 1000000 * time;
  TIMSK1 = 1 << OCIE1A;
  start_pulse();
}

void setup_pulse()
{
  sei();

  DDRD = 1 << 6;
  TCCR0A = 1 << WGM01;
  TCCR0B = 1 << CS00;
  OCR0A = 100;

  //  TCCR1A = 1 << WGM01;
  TCCR1B = (1 << WGM12) | (1 << CS10);
  OCR0A = 100;  
}

ISR(TIMER1_COMPA) 
{
  stop_pulse();
  TIMSK1 = 0;
}
