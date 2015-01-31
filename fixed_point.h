#pragma once

#include <avr/io.h>

typedef struct {
  int32_t value;
  uint8_t scale;
} fixed_t;

fixed_t fixed_add(fixed_t a, fixed_t b);
fixed_t fixed_sub(fixed_t a, fixed_t b);
fixed_t fixed_div(fixed_t a, fixed_t b);
fixed_t fixed_mul(fixed_t a, fixed_t b);
