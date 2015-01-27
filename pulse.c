/* 
   Pulsgenerator
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "pulse.h"

#define F_CPU 8000000L

void stop_pulse()
{
  TCCR0A &= ~(1 << COM0A0);  
}

void start_pulse()
{
  TCCR0A |= 1 << COM0A0;
}

void pulse(uint16_t time)
/* time is value in micro seconds */
{
  OCR1A = (uint16_t)8 * time;
  start_pulse();
  TCCR1B |= 1 << CS10;
}

void setup_pulse()
{
  sei();

  DDRD = 1 << 6;
  TCCR0A = (1 << COM0A0) | (1 << WGM01);
  TCCR0B = 1 << CS00;
  OCR0A = 100;

  TCCR1B = (1 << WGM12); 
  TIMSK1 = 1 << OCIE1A;  
}

ISR(TIMER1_COMPA_vect) 
{
  stop_pulse(); 
  TCCR1B &= ~(1 << CS10);
  TCNT1 = 0;
}
