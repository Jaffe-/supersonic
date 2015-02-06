/*
  Character animation
 */

#include "animation.h"
#include "display.h"
#include <avr/io.h>

void animation_animate(animation_t* a)
/*
  Animates an animation

  Fethces the next set of pixel data for each character used in the animation
  and updates CGRAM
*/
{  
  if (++a->counter == a->count) {
    a->counter = 0;
    
    uint8_t character = a->first_character;
    
    for (uint8_t row = 0; row < a->height; row++) {

      // If width is 1, a-> frames is expected to be an array of pointers to
      // arrays of uint8_t elements having length a->height * 8
      if (a->width == 1) {
	uint8_t (*frames)[a->height * 8] = a->frames;
	display_write_character(character++, frames[a->current_frame] + row * 8); 
      }

      // Otherwise, a->frames is expected to be an array of pointers to arrays
      // of uint16_t elements having length a->height * 8. 
      else {	
	uint16_t (*frames)[a->height * 8] = a->frames;

	for (int8_t col = a->width - 1; col >= 0; col--) {
	  uint8_t pixel_rows[8];

	  // This loop shifts the data in the 16-bit pixel data value to get the
	  // 5 bit character data for the given column
	  for (uint8_t i = 0; i < 8; i++) 
	    pixel_rows[i] = (frames[a->current_frame][row * 8 + i] >> (col * 5)) & 0x1F;

	  display_write_character(character++, pixel_rows);
	}
      }
    }
    
    if (++a->current_frame == a->num_frames)
      a->current_frame = 0;
  }
}

void animation_display(animation_t* a, uint8_t scr, uint8_t x, uint8_t y)
/*
  Displays an animation on the screen by writing its character codes to DDRAM
 */
{
  uint8_t character = a->first_character;

  for (uint8_t row = 0; row < a->height; row++) {
    display_setcursor(scr, y + row, x);
    for (uint8_t col = 0; col < a->width; col++) {
      display_print_char(character++);
    }
  }
}

void animation_init(animation_t* a)
{
  a->counter = 0;
  a->current_frame = 0;
}
