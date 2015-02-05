/*
  Character animation
 */

#include "animation.h"
#include "display.h"
#include <avr/io.h>

void animation_animate(animation_t* a)
{  
  if (++a->counter == a->count) {
    a->counter = 0;
    
    uint8_t character = a->first_character;
    
    for (uint8_t row = 0; row < a->height; row++) {

      if (a->width > 1) {
	uint16_t (*frames)[a->height * 8] = a->frames;
	uint8_t pixel_rows[2][8];
	for (uint8_t i = 0; i < 8; i++) {
	  pixel_rows[0][i] = frames[a->current_frame][row * 8 + i] & 0x1F;
	  pixel_rows[1][i] = (frames[a->current_frame][row * 8 + i] >> 5) & 0x1F;
	}
	display_write_character(character++, pixel_rows[1]);
	display_write_character(character++, pixel_rows[0]);
      }

      else {
	uint8_t (*frames)[a->height * 8] = a->frames;
	display_write_character(character++, frames[a->current_frame] + row * 8); 
      }

    }
    
    if (++a->current_frame == a->num_frames)
      a->current_frame = 0;
  }
}

void animation_display(animation_t* a, uint8_t scr, uint8_t x, uint8_t y)
{
  uint8_t character = a->first_character;

  for (uint8_t row = 0; row < a->height; row++) {
    for (uint8_t col = 0; col < a->width; col++) {
      display_setcursor(scr, y + row, x + col);
      display_print_char(character++);
    }
  }
}

void animation_init(animation_t* a)
{
  a->counter = 0;
  a->current_frame = 0;
}
