#include "buffer.h"
#include "instructions.h"
#include "util.h"
#include "vm.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
  VirtualMachine vm = init_vm();

  uint8_t left = 0;
  uint8_t right = 0;

  u16_split(ROM_DATA_START, &left, &right);
  uint8_t text[] = {LDD,  0x0, 0x48, 0x0,  DIN, 0x0,  0x0, LDD,  0x0,
                    0x45, 0x0, DIN,  0x0,  0x0, LDD,  0x0, 0x4C, 0x0,
                    DIN,  0x0, 0x0,  LDD,  0x0, 0x4c, 0x0, DIN,  0x0,
                    0x0,  LDD, 0x0,  0x4f, 0x0, DIN,  0x0, 0x0,  HLT};
  uint8_t data[] = {0x0};
  init_text_vm(&vm, text, 36);
  init_data_vm(&vm, data, 1);

  while (!vm.halted) {
    step_vm(&vm);
    // debug_print(&vm);
  }

  Buffer buffer = init_buffer();
  push_buffer(&buffer, 0);
  push_buffer(&buffer, 1);
  push_buffer(&buffer, 2);

  printf("\n%d %d %d\n", pop_buffer(&buffer), pop_buffer(&buffer),
         pop_buffer(&buffer));

  delete_vm(&vm);
  delete_buffer(&buffer);
  return EXIT_SUCCESS;
}
