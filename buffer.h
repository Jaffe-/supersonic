#pragma once

#include <stdint.h>

typedef struct {
  uint8_t length;
  uint8_t read_pos;
  uint8_t write_pos;
  uint16_t* values;
} buffer_t;

void buffer_write(buffer_t* buffer, uint16_t value);
uint16_t buffer_read(buffer_t* buffer);
uint8_t buffer_unread_elements(buffer_t* buffer);
