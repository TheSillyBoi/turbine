#include "vm.h"
#include "instructions.h"
#include "util.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

VirtualMachine init_vm() {
  uint8_t memory[MEMORY_SIZE];

  memset(memory, 0x0, MEMORY_SIZE);

  VirtualMachine vm = {0};
  vm.base_pointer = RAM_END;
  vm.stack_pointer = RAM_END;
  vm.general_register = 0x0;
  memset(vm.memory, 0, MEMORY_SIZE);
  vm.instruction_pointer = 0x0;
  vm.flags_register = 0x0;

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

void step(VirtualMachine *vm) {
  if (vm->instruction_pointer >= MEMORY_SIZE) {
    printf("[vm] PROGRAM END\n");
  }
  switch (vm->memory[vm->instruction_pointer++]) {
  case LOAD: {
    uint8_t flag = deref_indirect(vm->instruction_pointer++, vm, 0);
    uint8_t mem_addr = deref_indirect(vm->instruction_pointer++, vm, 0);
    switch (flag) {
    case 0x0: {
      vm->general_register = vm->memory[mem_addr];
      break;
    }
    case 0x1: {
      vm->general_register =
          u16_combine(vm->memory[mem_addr], vm->memory[mem_addr + 1]);
      break;
    }
    case 0x2: {
      vm->stack_pointer =
          u16_combine(vm->memory[mem_addr], vm->memory[mem_addr + 1]);
      break;
    }
    case 0x3: {
      vm->base_pointer =
          u16_combine(vm->memory[mem_addr], vm->memory[mem_addr + 1]);
      break;
    }
    }
    break;
  }
  case DUMP: {
    uint8_t flag = deref_indirect(vm->instruction_pointer++, vm, 0);
    uint8_t mem_addr = deref_indirect(vm->instruction_pointer++, vm, 0);
    switch (flag) {
    case 0x0: {
      vm->memory[mem_addr] = vm->general_register;
      break;
    }
    case 0x1: {
      uint8_t *left = 0;
      uint8_t *right = 0;
      u16_split(vm->general_register, left, right);
      vm->memory[mem_addr] = *left;
      vm->memory[mem_addr + 1] = *right;
      break;
    }
    case 0x2: {
      uint8_t *left = 0;
      uint8_t *right = 0;
      u16_split(vm->stack_pointer, left, right);
      vm->memory[mem_addr] = *left;
      vm->memory[mem_addr + 1] = *right;
      break;
    }
    case 0x3: {
      uint8_t *left = 0;
      uint8_t *right = 0;
      u16_split(vm->base_pointer, left, right);
      vm->memory[mem_addr] = *left;
      vm->memory[mem_addr + 1] = *right;
      break;
    }
    }
    break;
  }
  case LDA: {
    uint8_t flag = deref_indirect(vm->instruction_pointer++, vm, 0);
    uint8_t mem_addr = deref_indirect(vm->instruction_pointer++, vm, 0);
    switch (flag) {
    case 0x0: {
      vm->general_register = mem_addr;
      break;
    }
    case 0x1: {
      vm->stack_pointer = mem_addr;
      break;
    }
    case 0x2: {
      vm->base_pointer = mem_addr;
      break;
    }
    }
    break;
  }
  // least significant first, most significant last for push
  // reverse for pop
  case PUSH: {
    uint8_t flag = deref_indirect(vm->instruction_pointer++, vm, 0);
    uint8_t mem_addr = deref_indirect(vm->instruction_pointer++, vm, 0);
    uint8_t *left = 0;
    uint8_t *right = 0;
    u16_split(vm->general_register, left, right);
    switch (flag) {
    case 0x0: {
      vm->memory[vm->stack_pointer--] = *left;
      break;
    }
    case 0x1: {
      vm->memory[vm->stack_pointer--] = *right;
      break;
    }
    case 0x2: {
      vm->memory[vm->stack_pointer--] = *left;
      vm->memory[vm->stack_pointer--] = *right;
      break;
    }
    }
    break;
  }
  case POP: {
    uint8_t flag = deref_indirect(vm->instruction_pointer++, vm, 0);
    uint8_t mem_addr = deref_indirect(vm->instruction_pointer++, vm, 0);
    switch (flag) {
    case 0x0: {
      vm->general_register = vm->memory[vm->stack_pointer++];
      break;
    }
    case 0x1: {
      vm->general_register = u16_combine(vm->memory[vm->stack_pointer++],
                                         vm->memory[vm->stack_pointer++]);
      break;
    }
    case 0x2: {
      vm->stack_pointer = u16_combine(vm->memory[vm->stack_pointer++],
                                      vm->memory[vm->stack_pointer++]);
      break;
    }
    case 0x3: {
      vm->base_pointer = u16_combine(vm->memory[vm->stack_pointer++],
                                     vm->memory[vm->stack_pointer++]);
      break;
    }
    }
    break;
  }
  case ADC: {
    break;
  }
  case ADD: {
    break;
  }
  case SUB: {
    break;
  }
  case SBB: {
    break;
  }
  case CMP: {
    break;
  }
  case JNZ: {
    break;
  }
  case AND: {
    break;
  }
  case NOT: {
    break;
  }
  case OR: {
    break;
  }
  case IN: {
    break;
  }
  case OUT: {
    break;
  }
  }
}

// Will most likely be used for instruction ptr as that often points to
// instructions which point to another instruction
uint8_t deref_indirect(uint8_t reg, VirtualMachine *vm, uint8_t shift) {
  return vm->memory[vm->memory[reg] + shift];
}
