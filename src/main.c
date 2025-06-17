#include "flags.h"
#include "instructions.h"
#include "util.h"
#include "vm.h"
#include <stdint.h>
#include <stdlib.h>

int main() {
  VirtualMachine vm = init_vm();

  uint8_t left = 0;
  uint8_t right = 0;

  u16_split(ROM_DATA_START, &left, &right);

  uint8_t text[] = {
      // LDD: Load Data
      // LDD RA_TWO_BYTES, 0x1234
      0x03, 0x01, 0x34, 0x12,

      // LDD RB_TWO_BYTES, 0x0005
      0x03, 0x03, 0x05, 0x00,

      // MOVE: Move Register
      // MOVE RC_TWO_BYTES, RA_TWO_BYTES (Move 0x1234 to RC)
      0x02, 0x05, 0x01,

      // PUSH: Push to Stack
      // PUSH RA_TWO_BYTES (Push 0x1234)
      0x04, 0x01,

      // POP: Pop from Stack
      // POP RD_TWO_BYTES (Pop 0x1234 into RD)
      0x05, 0x07,

      // ADD: Add Registers
      // ADD RA_TWO_BYTES, RB_TWO_BYTES (0x1234 + 0x0005 = 0x1239)
      0x06, 0x01, 0x03,

      // ADC: Add with Carry (assuming no initial carry for simplicity)
      // LDD RC_TWO_BYTES, 0x0001
      0x03, 0x05, 0x01, 0x00,
      // ADC RA_TWO_BYTES, RC_TWO_BYTES (0x1239 + 0x0001 = 0x123A)
      0x07, 0x01, 0x05,

      // SUB: Subtract Registers
      // SUB RA_TWO_BYTES, RB_TWO_BYTES (0x123A - 0x0005 = 0x1235)
      0x08, 0x01, 0x03,

      // SBB: Subtract with Borrow (assuming no initial borrow)
      // LDD RC_TWO_BYTES, 0x0001
      0x03, 0x05, 0x01, 0x00,
      // SBB RA_TWO_BYTES, RC_TWO_BYTES (0x1235 - 0x0001 = 0x1234)
      0x09, 0x01, 0x05,

      // NOT: Bitwise NOT
      // NOT RE_TWO_BYTES (Assuming RE is 0x0000 initially, becomes 0xFFFF)
      0x03, 0x09, 0x00, 0x00, // Load 0 into RE
      0x0A, 0x09,

      // OR: Bitwise OR
      // LDD RE_TWO_BYTES, 0x0F0F
      0x03, 0x09, 0x0F, 0x0F,
      // OR RA_TWO_BYTES, RE_TWO_BYTES (0x1234 | 0x0F0F = 0x1F3F)
      0x0B, 0x01, 0x09,

      // AND: Bitwise AND
      // LDD RE_TWO_BYTES, 0xFF00
      0x03, 0x09, 0x00, 0xFF,
      // AND RA_TWO_BYTES, RE_TWO_BYTES (0x1F3F & 0xFF00 = 0x1F00)
      0x0C, 0x01, 0x09,

      // CMP: Compare Registers
      // LDD RE_TWO_BYTES, 0x1F00
      0x03, 0x09, 0x00, 0x1F,
      // CMP RA_TWO_BYTES, RE_TWO_BYTES (0x1F00 vs 0x1F00, should set
      // CMP_EQUAL_TO)
      0x0D, 0x01, 0x09,

      // JUMP: Conditional Jump (if equal)
      // If CMP_EQUAL_TO is set, jump to address 0x000A (a dummy address for
      // testing)
      // This jump won't actually execute a real instruction in this snippet but
      // demonstrates its usage. In a real scenario, it would jump to a label.
      0x0E, 0x00, 0x0A, 0x00, // JUMP CMP_EQUAL_TO, 0x000A

      // HLT: Halt the processor
      0x11};

  init_text_vm(&vm, text, 67);

  // debug_print_vm(&vm);
  while (!vm.halted) {
    step_vm(&vm);
    // debug_print_vm(&vm);
  }

  delete_vm(&vm);
  return EXIT_SUCCESS;
}
