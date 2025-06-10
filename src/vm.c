#include "vm.h"
#include <assert.h>
#include <stdint.h>
#include <string.h>

VirtualMachine init_vm() {
  uint8_t memory[MEMORY_SIZE];

  memset(memory, 0x0, MEMORY_SIZE);

  const VirtualMachine vm = {.general_register = 0,
                             // stack moves backwards
                             .stack_pointer = RAM_END,
                             .base_pointer = RAM_END,
                             .flags_register = 0,
                             .instruction_pointer = ROM_TEXT_START,
                             .memory = memory};

  return vm;
}

void init_text(VirtualMachine *vm, uint8_t *text, uint16_t size) {
  assert(size > ROM_TEXT_END - ROM_TEXT_START + 1);
  for (int i = ROM_TEXT_START; i < size; i++) {
    vm->memory[i] = text[i];
  }
}

void init_data(VirtualMachine *vm, uint8_t *data, uint16_t size) {
  assert(size > ROM_DATA_END - ROM_DATA_START + 1);
  for (int i = ROM_DATA_START; i < size; i++) {
    vm->memory[i] = data[i];
  }
}

void step(VirtualMachine *vm) {}
