/* 
   Displayfunksjoner

   Displayet er koblet til følgende pinner:

   Display-pinne    Signal      Atmega
   4                RS          PD7
   6                E           PB0
   7 - 11           DB0 - DB4   PD0 - PD4
   12, 13           DB5, DB6    PB6, PB7
   14               DB7         PD5
*/

#include "display.h"
#include <avr/io.h>

// Bitposisjonene til RS og E i portregisterene
#define RS 7
#define E 0
#define DB0_4 0
#define DB5_DB6 6
#define DB7 5

/* kode ... */
