#include "fixed_point.h"

void scale_up(fixed_t* target, uint8_t exp);
void scale_down(fixed_t* target, uint8_t exp);


fixed_t fixed_add(fixed_t a, fixed_t b) 
{
  fixed_t result;

  fixed_t* min = (a.scale > b.scale) ? &b : &a;
  fixed_t* max = (a.scale > b.scale) ? &a : &b;

  uint8_t diff = max->scale - min->scale;
  
  result.scale = min->scale + diff;

  scale_up(min, diff);

  result.value = max->value + min->value;

  if (result.scale > 2) 
    scale_down(&result, result.scale - 2);

  return result;
}

fixed_t fixed_sub(fixed_t a, fixed_t b) 
{
  fixed_t result;

  fixed_t* min = (a.scale > b.scale) ? &b : &a;
  fixed_t* max = (a.scale > b.scale) ? &a : &b;

  uint8_t diff = max->scale - min->scale;
  
  result.scale = min->scale + diff;
  
  scale_up(min, diff);

  result.value = a.value - b.value;

  if (result.scale > 2) 
    scale_down(&result, result.scale - 2);

  return result;
}

fixed_t fixed_div(fixed_t a, fixed_t b)
{
  fixed_t result;
  
  int8_t diff = a.scale - b.scale;

  if (diff < 2) {
    scale_up(&a, 2 - diff);
  }
  
  result.scale = a.scale - b.scale;

  result.value = a.value / b.value;

  return result;
}

fixed_t fixed_mul(fixed_t a, fixed_t b) 
{
  fixed_t result = {.value = a.value * b.value,
		    .scale = a.scale + b.scale};

  if (result.scale > 2) 
    scale_down(&result, result.scale - 2);

  return result;
}


// Helper functions
void scale_up(fixed_t* target, uint8_t exp)
{
  for (uint8_t i = 0; i < exp; i++) 
    target->value *= 10;

  target->scale += exp;
}

void scale_down(fixed_t* target, uint8_t exp)
{
  for (uint8_t i = 0; i < exp; i++)
    target->value /= 10;

  target->scale -= exp;
}
