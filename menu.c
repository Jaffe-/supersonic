//#include "menu.h"
#include <stdint.h>
#include "display.h"
#include "input.h"

typedef struct {
  char* name;
  void (*function)(uint8_t);
} menu_item_t;

#define NUM_ITEMS 3
#define NUM_ELEMENTS 32

void distance(uint8_t);
void speed(uint8_t);
void time(uint8_t);
void fuckyou(uint8_t);

menu_item_t menu[NUM_ITEMS] = {
  {.name = "Distance",
   .function = &distance},
  {.name = "Speed",
   .function = &speed},
  {.name = "Time",
   .function = &time},
};

void menu_print(void)
{
  for (uint8_t i = 0; i < NUM_ITEMS; i++) {
    screen_setcursor(0, i, 2);
    screen_print_integer(i + 1);
    screen_print_string(". ");
    screen_print_string(menu[i].name); 
  }
  
  window_update(0, 1);
}

float get_time(uint16_t counter_value) 
{
  return counter_value / (2 * 1000000.0);
}

#define TOL 0.30

void distance(uint8_t line)
{
  screen_setcursor(1, line, 0);
  screen_print_string("Mean: ");

  if (buffer_unread_elements() >= NUM_ELEMENTS) {
    float distance_sum = 0;

    float min = 100000.0;
    float max = 0;

    uint8_t cnt = 0;
    for (uint8_t i = 0; i < NUM_ELEMENTS; i++) {
      float distance = get_time(buffer_read()) * 340.0;
//      if (distance > TOL) {
      distance_sum += distance;
      if (distance > max) 
	max = distance;
      if (distance < min)
	min = distance;
      cnt++;
      //}
    }

    float mean;
    if (cnt > 0) 
      mean = distance_sum / (float)cnt;
    else
      mean = 0;

    screen_setcursor(1, line, 6);
    screen_print_string("        ");
    screen_setcursor(1, line, 6); 
    if (mean >= 1.0) {
      screen_print_float(mean);
      screen_print_string(" m");
    }
      
    else {
      screen_print_integer(mean * 100);
      screen_print_string(" cm");
    }

    screen_setcursor(1, line+1, 0);
    screen_print_string("Max:            ");
    screen_setcursor(1, line+1, 6);

    if (max >= 1.0) {
      screen_print_float(max);
      screen_print_string(" m");
    }
      
    else {
      screen_print_integer(max * 100);
      screen_print_string(" cm");
    }

    
  }

}

void speed(uint8_t line) 
{

  screen_setcursor(1, line, 0);
  screen_print_string("Speed: ");

  if (buffer_unread_elements() >= NUM_ELEMENTS) {
    float distance_mean;
    float distance_sum;
    float distances[NUM_ELEMENTS];

    uint8_t cnt = 0;
    for (uint8_t i = 0; i < NUM_ELEMENTS; i++) {
      float distance = get_time(buffer_read()) * 340.0;
      if (distance > TOL) 
	distances[cnt++] = distance; 
    } 

    float speed_mean = 0;
    for (uint8_t i = 0; i < cnt - 1; i++) {
      speed_mean += (distances[i+1] - distances[i]) * 976;
    }
    
    speed_mean /= (float)(cnt - 1);
    
    screen_setcursor(1, line + 1, 0);
    screen_print_string("              ");
    screen_setcursor(1, line + 1, 0); 
    
    if (speed_mean >= 1.0) {
      screen_print_float(speed_mean);
      screen_print_string(" m/s");
    }
    
    else {
      screen_print_integer(speed_mean * 100);
      screen_print_string(" cm/s");
    }
    
  }
      
}

void time(uint8_t line)
{
  screen_setcursor(1, line, 0);
  screen_print_string("Time: ");

  if (buffer_unread_elements() >= 8) {
    screen_setcursor(1, line, 6);
    screen_print_string("         ");
    screen_setcursor(1, line, 6);
    screen_print_float(get_time(buffer_read()) * 1000.0);
    screen_print_string(" ms");
  }
}

void fuckyou(uint8_t line)
{

}

#define MODE_MENU 0
#define MODE_FUNCTION 1

void menu_handler(void)
{
  uint8_t current_item = 0;
  uint8_t current_line = 0;
  uint8_t mode = MODE_MENU;

  menu_print();

  window_update(0, 0);
  display_setcursor(1,1,1);
  display_print_char(0x7E);

  while (1) {
    input_update();

    if (mode == MODE_MENU) {
      if (input_pressed(BUTTON_DOWN)) {
	if (current_item < NUM_ITEMS - 1) { 
	  current_item++;
	  screen_setcursor(0, current_item, 0);
	  screen_print_char(0x7E);
	  screen_setcursor(0, current_item - 1, 0);
	  screen_print_char(' ');
	 	  
	}

	if (current_item >= 1 && current_item < NUM_ITEMS) {
	  current_line = current_item - 1;
	}

	window_update(0, current_line); 

      }
      
      if (input_pressed(BUTTON_UP)) {
	if (current_item > 0) {
	  current_item--;
	  screen_setcursor(0, current_item, 0);
	  screen_print_char(0x7E);
	  screen_setcursor(0, current_item + 1, 0);
	  screen_print_char(' ');
	  
	  if (current_item >= 0 && current_item < NUM_ITEMS - 1) {
	    current_line = current_item;
	  }
	  window_update(0, current_line);
	}
      }
      
      if (input_pressed(BUTTON_RIGHT)) {
	screen_clear(1);
	menu[current_item].function(current_line);
	display_move_window(2);
	mode = MODE_FUNCTION;
      }
      
    }
    
    else if (mode == MODE_FUNCTION) {
      menu[current_item].function(current_line);
      window_update(1, current_line);
      
      if (input_pressed(BUTTON_LEFT)) {
	display_move_window(1);
	mode = MODE_MENU;
      }
    }
      
  }
}

