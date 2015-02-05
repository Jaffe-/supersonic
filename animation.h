/*
  Character animation
*/

#include <avr/io.h>

typedef struct {
  uint8_t width;
  uint8_t height;
  void* frames;
  uint8_t num_frames;
  uint8_t first_character;
  uint8_t count;

  // Internal 
  uint8_t counter;
  uint8_t current_frame;
} animation_t;

void animation_display(animation_t* a, uint8_t scr, uint8_t x, uint8_t y);
void animation_animate(animation_t* a);
