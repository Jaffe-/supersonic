/* 
   Pulse generator
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

void pulse_setup()
{
  // Enable interrupts
  sei();

  // Set PD6 as output
  DDRD |= 1 << 6;


  /* Timer 0 setup */
  
  // Set WGM02:0 to 010 to put the timer in Clear Timer on Compare Match (CTC) mode
  // Set COM0A1:0 to 1 to make it toggle PD6 each time the counter reaches the top
  // value set in OCR0A
  TCCR0A = (1 << COM0A0) | (1 << WGM01);

  // CS02:0 = 1 means no prescaling
  TCCR0B = 1 << CS00;

  // Set compare value for the timer. The timer resets when it reaches this value
  // Correction: f = f_CPU / (2 * (OCR0A + 1))
  // changed value from 100 to 99, which will give an exact 40 kHz square wave:
  // 8e6 / (2 * (99 + 1)) = 40e3
  OCR0A = 99;


  /* Timer 1 setup */
  
  TCCR1B = (1 << WGM12); 
  TIMSK1 = 1 << OCIE1A;  
}

ISR(TIMER1_COMPA_vect) 
{
  stop_pulse(); 
  TCCR1B &= ~(1 << CS10);
  TCNT1 = 0;
}
