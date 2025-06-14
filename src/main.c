#include "flags.h"
#include "instructions.h"
#include "util.h"
#include "vm.h"
#include <stdlib.h>

int main() {
  VirtualMachine vm = init_vm();

  uint8_t left = 0;
  uint8_t right = 0;

  u16_split(ROM_DATA_START, &left, &right);
  // LDD test
  // #define TEXT_LENGTH 4
  // uint8_t text[] = {LDD, RA_BYTE, 0x2, HLT};
  
  // Stack test
  #define TEXT_LENGTH 5 
  uint8_t text[] = {LDD, RB_TWO_BYTES, 0x9, 0x8, HLT};

  // loop (length: 13)
  init_text_vm(&vm, text, TEXT_LENGTH);

  while (!vm.halted) {
    step_vm(&vm);
    debug_print_vm(&vm);
  }

  delete_vm(&vm);
  return EXIT_SUCCESS;
}
