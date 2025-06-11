#include "instructions.h"
#include "util.h"
#include "vm.h"
#include <stdlib.h>
// int main(int argc, char *argv[])
int main() {
  VirtualMachine vm = init_vm();
  uint8_t left = 0;
  uint8_t right = 0;

  uint8_t * left_ptr = &left;
  uint8_t * right_ptr = &left;

  u16_split(ROM_DATA_START, left_ptr, right_ptr);
  uint8_t text[] = {LOAD, 0x1, left, right};
  uint8_t data[] = {0x1};
  init_text(&vm, text, 3);
  init_data(&vm, data, 1);

  delete_vm(&vm);
  return EXIT_SUCCESS;
}
