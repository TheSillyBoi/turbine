#include "flags.h"
#include "instructions.h"
#include "vm.h"
#include <stdint.h>
#include <stdlib.h>

int main() {
  VirtualMachine vm = init_vm();

  uint8_t text[] = {LDD, RA_BYTE, 0x73,    DUMP, RA_BYTE, 0x1,
                    0x0, DUMP,    RA_BYTE, 0x0,  0x0,     HLT};

  init_program_vm(&vm, text, 12);

  // debug_print_vm(&vm);
  while (!vm.halted) {
    step_vm(&vm);
    // debug_print_vm(&vm);
  }

  delete_vm(&vm);
  return EXIT_SUCCESS;
}
