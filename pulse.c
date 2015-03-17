/* 
   Pulse generator
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "pulse.h"

#define F_CPU 8000000L

#define STATE_PULSE 0
#define STATE_WAIT 1

static uint8_t state;

#define BUFFER_LENGTH 64

uint16_t distance_buffer_values[BUFFER_LENGTH];
uint16_t speed_buffer_values[BUFFER_LENGTH];

buffer_t distance_buffer = {.length = BUFFER_LENGTH,
			    .read_pos = 0,
			    .write_pos = 0,
			    .values = distance_buffer_values};

buffer_t speed_buffer = {.length = BUFFER_LENGTH,
			 .read_pos = 0,
			 .write_pos = 0,
			 .values = speed_buffer_values};

void stop_pulse()
{
  TCCR0A &= ~(1 << COM0A0);  
}

void start_pulse()
{
  TCNT0 = 0;
  TCCR0A |= 1 << COM0A0;
}

void reset_timer()
{
  TCNT1 = 0;
}


void read_timer()
{
  static uint16_t last_value;
  buffer_write(&distance_buffer, TCNT1);
  if (TCNT1 >= last_value)
    buffer_write(&speed_buffer, TCNT1 - last_value);
  else
    buffer_write(&speed_buffer, last_value - TCNT1);
  last_value = TCNT1;
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

  // Set prescaling to 8
  TCCR1B = 1 << CS11;  


  /* Timer 2 setup */
    
  // Put timer 2 in CTC mode
  TCCR2A = 1 << WGM21;

  // Set prescaling to 1024
  TCCR2B = 0b111 << CS20;

  // Set initial compare match value
  OCR2A = 7;

  // Enable interrupt on compare match on timer 2
  TIMSK2 = 1 << OCIE2A;


  /* Enable pin change interrupt on PC4 */
  PCMSK1 = 1 << PCINT12;
  PCICR = 1 << PCIE1;

  state = STATE_PULSE;
}

ISR(TIMER2_COMPA_vect) 
{
  if (state == STATE_PULSE) {
    stop_pulse(); 
    OCR2A = 255;
    state = STATE_WAIT;
  }
  else {
    start_pulse();
    reset_timer();
    OCR2A = 7;
    state = STATE_PULSE;
  } 
}

ISR(PCINT1_vect) 
{
  // If pin is high, put the timer value in the buffer
  if (PINC & (1 << PC4)) {
    read_timer();
  }
}
