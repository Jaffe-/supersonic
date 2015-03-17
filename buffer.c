#include "buffer.h"

void buffer_write(buffer_t* buffer, uint16_t value)
{
  buffer->values[buffer->write_pos] = value;
    
  if (++buffer->write_pos == buffer->length) 
    buffer->write_pos = 0;
  
}

uint16_t buffer_read(buffer_t* buffer) 
{
  static cnt = 0;
  uint16_t value = buffer->values[buffer->read_pos];
  
  if (++buffer->read_pos == buffer->length) 
    buffer->read_pos = 0;

  return value;
}

uint8_t buffer_unread_elements(buffer_t* buffer)
{
  if (buffer->write_pos >= buffer->read_pos)
    return buffer->write_pos - buffer->read_pos;
  else {
    return buffer->length - (buffer->read_pos - buffer->write_pos);
  }  
}
