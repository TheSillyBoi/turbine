#include "buffer.h"
#include <stdlib.h>
#include <string.h>

Buffer init_buffer() {
  Buffer buffer = {
      .data = malloc(sizeof(uint8_t) * 1), .size = 0, .capacity = 2};
  memset(buffer.data, 0, sizeof(uint8_t) * 1);

  return buffer;
}

void delete_buffer(Buffer *buffer) { free(buffer->data); }
void push_buffer(Buffer *buffer, uint8_t data) {
  if (++buffer->size >= buffer->capacity) {
    buffer->data = realloc(buffer->data, sizeof(uint8_t) * buffer->capacity * 1.5);
  }
  buffer->data[buffer->size - 1] = data;
}
uint8_t pop_buffer(Buffer *buffer) { return buffer->data[buffer->size-- - 1]; }
