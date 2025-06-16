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

#define TEXT_LENGTH 23
  uint8_t text[] = {LDD,  RA_BYTE,          0x10,    LDD, RB_BYTE, 0x0,
                    LDD,  RC_BYTE,          0x1,     SUB, RA_BYTE, RC_BYTE,
                    MOVE, ACCUMULATOR,      RA_BYTE, CMP, RA_BYTE, RB_BYTE,
                    JUMP, CMP_GREATER_THAN, 0x9,     0x0, HLT};

  init_text_vm(&vm, text, TEXT_LENGTH);

  debug_print_vm(&vm);
  while (!vm.halted) {
    step_vm(&vm);
    debug_print_vm(&vm);
  }

  delete_vm(&vm);
  return EXIT_SUCCESS;
}
