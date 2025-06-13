#ifndef INCLUDE_SRC_BUFFER_H_
#define INCLUDE_SRC_BUFFER_H_

#include <stdint.h>

typedef struct Buffer {
  uint8_t *data;
  uint16_t size;
  uint16_t capacity;
} Buffer;

Buffer init_buffer();
void delete_buffer(Buffer *buffer);
void push_buffer(Buffer *buffer, uint8_t data);
uint8_t pop_buffer(Buffer *buffer);

// I don't think these functions are necessary so they'll be added as needed
// void insert_buffer(Buffer *buffer, uint16_t index);
// uint8_t remove_buffer(Buffer *buffer, uint16_t index);
// uint8_t get_buffer(Buffer *buffer, uint16_t index);

#endif // INCLUDE_SRC_BUFFER_H_
