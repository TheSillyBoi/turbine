#include "instructions.h"
#include "util.h"
#include "vm.h"
#include <stdlib.h>

int main() {
  VirtualMachine vm = init_vm();

  uint8_t left = 0;
  uint8_t right = 0;

  u16_split(ROM_DATA_START, &left, &right);
  // hello world (length: 36)
  uint8_t text[] = {LDD,  0x0, 0x48, 0x0,  DIN, 0x0,  0x0, LDD,  0x0,
                    0x45, 0x0, DIN,  0x0,  0x0, LDD,  0x0, 0x4C, 0x0,
                    DIN,  0x0, 0x0,  LDD,  0x0, 0x4c, 0x0, DIN,  0x0,
                    0x0,  LDD, 0x0,  0x4f, 0x0, DIN,  0x0, 0x0,  HLT};

  // loop (length: 13)
  uint8_t data[] = {0x1};
  init_text_vm(&vm, text, 36);
  init_data_vm(&vm, data, 1);

  while (!vm.halted) {
    step_vm(&vm);
    debug_print_vm(&vm);
  }

  delete_vm(&vm);
  return EXIT_SUCCESS;
}
