#include <avr/io.h>

#define F_CPU 8000000L

#include <util/delay.h>
#include <math.h>
#include "pulse.h"
#include "display.h"
#include "input.h"
#include "animation.h"

int main()
{
  //while(1);
  display_setup(); 
  input_setup();
  pulse_setup();
    
  while(1) {
    input_update();

    if (buffer_unread_elements() >= 8) {
      float distance_sum = 0;
      float speed_mean = 0;
      float distances[8];
      const float delta_t = 0.033792;
      for (uint8_t i = 0; i < 8; i++) {
	distances[i] = (buffer_read() * 340.0) / (2 * 1000000.0);
	distance_sum += distances[i];
      }
      float mean = distance_sum / 8.0;

      float speed = 0;
      for (uint8_t i = 0; i < 7; i++) {
	speed = (distances[i + 1] - distances[i]) / delta_t;
	speed_mean += speed;
      }
      speed_mean /= 7.0;

      display_clear();
      display_setcursor(1,1,1);
      display_print_float(distances[7] * 100);
      display_setcursor(1,2,1);
      display_print_float(speed);
    }

    if (input_pressed(BUTTON_RIGHT))
      display_move_window(2);
    if (input_pressed(BUTTON_LEFT))
      display_move_window(1);
      
  }
}
