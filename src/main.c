#include "instructions.h"
#include "util.h"
#include "vm.h"
#include <stdlib.h>
// int main(int argc, char *argv[])
int main() {
  VirtualMachine vm = init_vm();

  uint8_t left = 0;
  uint8_t right = 0;

  u16_split(ROM_DATA_START, &left, &right);

  uint8_t text[] = {LOAD, 0x1, left, right, DUMP, 0x0, 0x4, 0x4, LDA, 0x0, 0x4, 0x0, PUSH, 0x0};
  uint8_t data[] = {0x12};
  init_text(&vm, text, 14);
  init_data(&vm, data, 1);

  debug_print(&vm);
  step(&vm);
  debug_print(&vm);
  step(&vm);
  debug_print(&vm);
  step(&vm);
  debug_print(&vm);
  step(&vm);
  debug_print(&vm);
  step(&vm);
  debug_print(&vm);

  delete_vm(&vm);
  return EXIT_SUCCESS;
}
