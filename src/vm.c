#include "vm.h"
#include "device.h"
#include "flags.h"
#include "instructions.h"
#include "util.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

VirtualMachine init_vm() {
  uint8_t *memory = malloc(sizeof(uint8_t) * MEMORY_SIZE);
  Device *devices = malloc(sizeof(Device) * DEVICES_LENGTH);

  VirtualMachine vm = {0};
  vm.base_pointer = RAM_END;
  vm.stack_pointer = RAM_END;
  vm.register_a = 0x0;
  vm.register_b = 0x0;
  vm.accumulator = 0x0;
  vm.instruction_pointer = 0x0;
  vm.status_register = 0x0;
  vm.halted = false;
  vm.memory = memory;
  vm.devices = devices;
  vm.devices[0] = console;
  vm.devices[1] = screen;

  for (int i = 0; i < DEVICES_LENGTH; i++) {
    if (vm.devices[i].init != NULL) {
      vm.devices[i].init();
    }
  }

  return vm;
}

void delete_vm(VirtualMachine *vm) {
  free(vm->memory);
  free(vm->devices);
}

void init_text_vm(VirtualMachine *vm, uint8_t *text, uint16_t size) {
  assert(size < ROM_TEXT_END - ROM_TEXT_START + 1);
  for (int i = 0; i < size; i++) {
    vm->memory[ROM_TEXT_START + i] = text[i];
  }
}

void init_data_vm(VirtualMachine *vm, uint8_t *data, uint16_t size) {
  assert(size < ROM_DATA_END - ROM_DATA_START + 1);
  for (int i = 0; i < size; i++) {
    vm->memory[ROM_DATA_START + i] = data[i];
  }
}

void step_vm(VirtualMachine *vm) {
  if (vm->halted) {
    return;
  }
  switch (vm->memory[vm->instruction_pointer++]) {
  case LOAD: {
    uint8_t reg_flag = vm->memory[vm->instruction_pointer++];
    uint8_t left_mem_addr = vm->memory[vm->instruction_pointer++];
    uint8_t right_mem_addr = vm->memory[vm->instruction_pointer++];
    uint8_t mem_addr = u16_combine(left_mem_addr, right_mem_addr);

    switch (reg_flag) {
    case RA_BYTE: {
      vm->register_a = vm->memory[mem_addr];
      break;
    }
    case RA_TWO_BYTES: {
      vm->register_a =
          u16_combine(vm->memory[mem_addr], vm->memory[mem_addr + 1]);
      break;
    }
    case RB_BYTE: {
      vm->register_b = vm->memory[mem_addr];
      break;
    }
    case RB_TWO_BYTES: {
      vm->register_b =
          u16_combine(vm->memory[mem_addr], vm->memory[mem_addr + 1]);
      break;
    }
    case STACK_PTR: {
      vm->stack_pointer =
          u16_combine(vm->memory[mem_addr], vm->memory[mem_addr + 1]);
      break;
    }
    case BASE_PTR: {
      vm->base_pointer =
          u16_combine(vm->memory[mem_addr], vm->memory[mem_addr + 1]);
      break;
    }
    case ACCUMULATOR: {
      vm->accumulator =
          u16_combine(vm->memory[mem_addr], vm->memory[mem_addr + 1]);
      break;
    }
    case STATUS: {
      vm->status_register = vm->memory[mem_addr];
      break;
    }
    }

    break;
  }
  case DUMP: {
    uint8_t reg_flag = vm->memory[vm->instruction_pointer++];
    uint8_t left_mem_addr = vm->memory[vm->instruction_pointer++];
    uint8_t right_mem_addr = vm->memory[vm->instruction_pointer++];
    uint8_t mem_addr = u16_combine(left_mem_addr, right_mem_addr);

    switch (reg_flag) {
    case RA_BYTE: {
      vm->memory[mem_addr] = vm->register_a;
      break;
    }
    case RA_TWO_BYTES: {
      uint8_t left = 0;
      uint8_t right = 0;
      u16_split(vm->register_a, &left, &right);
      vm->memory[mem_addr] = left;
      vm->memory[mem_addr + 1] = right;
      break;
    }
    case RB_BYTE: {
      vm->memory[mem_addr] = vm->register_b;
      break;
    }
    case RB_TWO_BYTES: {
      uint8_t left = 0;
      uint8_t right = 0;
      u16_split(vm->register_b, &left, &right);
      vm->memory[mem_addr] = left;
      vm->memory[mem_addr + 1] = right;
      break;
    }
    case STACK_PTR: {
      uint8_t left = 0;
      uint8_t right = 0;
      u16_split(vm->stack_pointer, &left, &right);
      vm->memory[mem_addr] = left;
      vm->memory[mem_addr + 1] = right;
      break;
    }
    case BASE_PTR: {
      uint8_t left = 0;
      uint8_t right = 0;
      u16_split(vm->base_pointer, &left, &right);
      vm->memory[mem_addr] = left;
      vm->memory[mem_addr + 1] = right;
      break;
    }
    case ACCUMULATOR: {
      uint8_t left = 0;
      uint8_t right = 0;
      u16_split(vm->accumulator, &left, &right);
      vm->memory[mem_addr] = left;
      vm->memory[mem_addr + 1] = right;
      break;
    }
    case STATUS: {
      vm->memory[mem_addr] = vm->status_register;
      break;
    }
    }

    break;
  }
  case MOVE: {
    uint16_t *reg1 = NULL;
    uint16_t *reg2 = NULL;

    reg_flag_parser(vm, &reg1);
    reg_flag_parser(vm, &reg2);

    *reg2 = *reg1;

    break;
  }
  case LDD: {
    switch (vm->memory[vm->instruction_pointer++]) {
    case RA_BYTE: {
      vm->register_a = vm->memory[vm->instruction_pointer];
      break;
    }
    case RA_TWO_BYTES: {
      uint8_t left = vm->memory[vm->instruction_pointer++];
      uint8_t right = vm->memory[vm->instruction_pointer];
      vm->register_a = u16_combine(left, right);
      break;
    }
    case RB_BYTE: {
      vm->register_b = vm->memory[vm->instruction_pointer];
      break;
    }
    case RB_TWO_BYTES: {
      uint8_t left = vm->memory[vm->instruction_pointer++];
      uint8_t right = vm->memory[vm->instruction_pointer];
      vm->register_b = u16_combine(left, right);
      break;
    }
    case STACK_PTR: {
      uint8_t left = vm->memory[vm->instruction_pointer++];
      uint8_t right = vm->memory[vm->instruction_pointer];
      vm->stack_pointer = u16_combine(left, right);
      break;
    }
    case BASE_PTR: {
      uint8_t left = vm->memory[vm->instruction_pointer++];
      uint8_t right = vm->memory[vm->instruction_pointer];
      vm->base_pointer = u16_combine(left, right);
      break;
    }
    case ACCUMULATOR: {
      uint8_t left = vm->memory[vm->instruction_pointer++];
      uint8_t right = vm->memory[vm->instruction_pointer];
      vm->accumulator = u16_combine(left, right);
      break;
    }
    case STATUS: {
      vm->status_register = vm->memory[vm->instruction_pointer];
      break;
    }
    }
  }
  // left first, right last for push and reverse for pop
  case PUSH: {
    switch (vm->memory[vm->instruction_pointer++]) {
    case RA_BYTE: {
      vm->memory[vm->stack_pointer--] = vm->register_a;
      break;
    }
    case RA_TWO_BYTES: {
      uint8_t left = 0;
      uint8_t right = 0;
      u16_split(vm->register_a, &left, &right);
      vm->memory[vm->stack_pointer--] = left;
      vm->memory[vm->stack_pointer--] = right;
      break;
    }
    case RB_BYTE: {
      vm->memory[vm->stack_pointer] = vm->register_a;
      break;
    }
    case RB_TWO_BYTES: {
      uint8_t left = 0;
      uint8_t right = 0;
      u16_split(vm->register_b, &left, &right);
      vm->memory[vm->stack_pointer--] = left;
      vm->memory[vm->stack_pointer--] = right;
      break;
    }
    case STACK_PTR: {
      uint8_t left = 0;
      uint8_t right = 0;
      u16_split(vm->stack_pointer, &left, &right);
      vm->memory[vm->stack_pointer--] = left;
      vm->memory[vm->stack_pointer--] = right;
      break;
    }
    case BASE_PTR: {
      uint8_t left = 0;
      uint8_t right = 0;
      u16_split(vm->base_pointer, &left, &right);
      vm->memory[vm->stack_pointer--] = left;
      vm->memory[vm->stack_pointer--] = right;
      break;
    }
    case ACCUMULATOR: {
      uint8_t left = 0;
      uint8_t right = 0;
      u16_split(vm->accumulator, &left, &right);
      vm->memory[vm->stack_pointer--] = left;
      vm->memory[vm->stack_pointer--] = right;
      break;
    }
    case STATUS: {
      vm->memory[vm->stack_pointer--] = vm->status_register;
      break;
    }
    }
    break;
  }
  case POP: {
    switch (vm->memory[vm->instruction_pointer++]) {
    case RA_BYTE: {
      vm->register_a = vm->memory[vm->stack_pointer++];
      break;
    }
    case RA_TWO_BYTES: {
      uint8_t right = vm->memory[vm->stack_pointer++];
      uint8_t left = vm->memory[vm->stack_pointer++];

      vm->register_a = u16_combine(left, right);
      break;
    }
    case RB_BYTE: {
      vm->register_b = vm->memory[vm->stack_pointer++];
      break;
    }
    case RB_TWO_BYTES: {
      uint8_t right = vm->memory[vm->stack_pointer++];
      uint8_t left = vm->memory[vm->stack_pointer++];

      vm->register_b = u16_combine(left, right);
      vm->register_b = vm->memory[vm->stack_pointer++];
      break;
    }
    case STACK_PTR: {
      uint8_t right = vm->memory[vm->stack_pointer++];
      uint8_t left = vm->memory[vm->stack_pointer++];

      vm->stack_pointer = u16_combine(left, right);
      break;
    }
    case BASE_PTR: {
      uint8_t right = vm->memory[vm->stack_pointer++];
      uint8_t left = vm->memory[vm->stack_pointer++];

      vm->base_pointer = u16_combine(left, right);
      break;
    }
    case ACCUMULATOR: {
      uint8_t right = vm->memory[vm->stack_pointer++];
      uint8_t left = vm->memory[vm->stack_pointer++];

      vm->accumulator = u16_combine(left, right);
      break;
    }
    case STATUS: {
      vm->status_register = vm->memory[vm->stack_pointer++];
      break;
    }
    }
    break;
  }
  case ADD: {
    uint16_t *reg1 = NULL;
    uint16_t *reg2 = NULL;

    reg_flag_parser(vm, &reg1);
    reg_flag_parser(vm, &reg2);

    uint16_t result = *reg1 + *reg2;
    vm->accumulator = (uint8_t)result;
    vm->status_register = (result > 255) ? ADD_CARRY : ADD_NO_CARRY;

    break;
  }
  case ADC: {
    uint16_t *reg1 = NULL;
    uint16_t *reg2 = NULL;

    reg_flag_parser(vm, &reg1);
    reg_flag_parser(vm, &reg2);

    uint16_t result =
        *reg1 + *reg2 + (vm->status_register == ADD_CARRY ? 1 : 0);
    vm->accumulator = (uint8_t)result;
    vm->status_register = (result > 255) ? ADD_CARRY : ADD_NO_CARRY;

    break;
  }
  case SUB: {
    uint16_t *reg1 = NULL;
    uint16_t *reg2 = NULL;

    reg_flag_parser(vm, &reg1);
    reg_flag_parser(vm, &reg2);

    bool borrowed = (*reg1 < *reg2);

    vm->accumulator = *reg1 - *reg2;
    vm->status_register = (borrowed) ? SUB_BORROW : SUB_NO_BORROW;

    break;
  }

  case SBB: {
    uint16_t *reg1 = NULL;
    uint16_t *reg2 = NULL;

    reg_flag_parser(vm, &reg1);
    reg_flag_parser(vm, &reg2);

    uint8_t borrow_in_value = (vm->status_register == SUB_BORROW ? 1 : 0);
    uint16_t total_subtract_amount = (uint16_t)*reg2 + borrow_in_value;
    bool new_borrow_occurred = (*reg1 < total_subtract_amount);
    *reg1 = *reg1 - (uint8_t)total_subtract_amount;
    vm->status_register = new_borrow_occurred ? SUB_BORROW : SUB_NO_BORROW;

    break;
  }
  case NOT: {
    uint16_t *reg1 = NULL;
    reg_flag_parser(vm, &reg1);

    vm->accumulator = ~(*reg1);
    break;
  }
  case OR: {
    uint16_t *reg1 = NULL;
    uint16_t *reg2 = NULL;

    reg_flag_parser(vm, &reg1);
    reg_flag_parser(vm, &reg2);

    vm->accumulator = *reg1 | *reg2;
    break;
  }
  case AND: {
    uint16_t *reg1 = NULL;
    uint16_t *reg2 = NULL;

    reg_flag_parser(vm, &reg1);
    reg_flag_parser(vm, &reg2);

    vm->accumulator = *reg1 & *reg2;
    break;
  }
  case CMP: {
    uint16_t *reg1 = NULL;
    uint16_t *reg2 = NULL;

    reg_flag_parser(vm, &reg1);
    reg_flag_parser(vm, &reg2);

    if (*reg1 > *reg2) {
      vm->status_register = CMP_GREATER_THAN;
    } else if (*reg1 == *reg2) {
      vm->status_register = CMP_EQUAL_TO;
    } else {
      vm->status_register = CMP_LESS_THAN;
    }

    break;
  }
  case JUMP: {
    uint8_t status = vm->memory[vm->instruction_pointer++];
    uint8_t left_memory_addr = vm->memory[vm->instruction_pointer++];
    uint8_t right_memory_addr = vm->memory[vm->instruction_pointer++];
    uint16_t memory_addr = u16_combine(left_memory_addr, right_memory_addr);
    if (status == vm->status_register) {
      vm->instruction_pointer = vm->memory[memory_addr];
    }

    break;
  }
  case DIN: {
    uint8_t device_id = vm->memory[vm->instruction_pointer++];
    uint16_t *reg = NULL;
    reg_flag_parser(vm, &reg);
    if (device_id > 0 && device_id < DEVICES_LENGTH) {
      if (vm->devices[device_id].in != NULL) {
        vm->devices[device_id].in(*reg);
      }
    }
    break;
  }
  case DOUT: {
    uint8_t device_id = vm->memory[vm->instruction_pointer++];
    if (device_id > 0 && device_id < DEVICES_LENGTH) {
      if (vm->devices[device_id].out != NULL) {
        vm->accumulator = vm->devices[device_id].out();
      }
    }
    break;
  }
  case HLT: {
    vm->halted = true;
    break;
  }
  }
}

void reg_flag_parser(VirtualMachine *vm, uint16_t **ptr) {
  switch (vm->memory[vm->instruction_pointer++]) {
  case RA_BYTE:
  case RA_TWO_BYTES: {
    *ptr = &vm->register_a;
    break;
  }
  case RB_BYTE:
  case RB_TWO_BYTES: {
    *ptr = &vm->register_b;
    break;
  }
  case STACK_PTR: {
    *ptr = &vm->stack_pointer;
    break;
  }
  case BASE_PTR: {
    *ptr = &vm->base_pointer;
    break;
  }
  case ACCUMULATOR: {
    *ptr = &vm->accumulator;
    break;
  }
  case STATUS: {
    *ptr = (uint16_t *)&vm->status_register;
    break;
  }
  }
}

void debug_print_vm(VirtualMachine *vm) {
  printf("Register A: 0x%x\n", vm->register_a);
  printf("Register B: 0x%x\n", vm->register_b);
  printf("Accumulator: 0x%x\n", vm->accumulator);
  printf("Instruction Pointer: 0x%x\n", vm->instruction_pointer);
  printf("Stack Pointer: 0x%x\n", vm->stack_pointer);
  printf("Base Pointer: 0x%x\n", vm->base_pointer);
  printf("Status Register: 0x%x\n", vm->status_register);
  printf("Memory: \n");
  for (uint16_t i = 0; i < MEMORY_SIZE; i++) {
    if (vm->memory[i] != 0) {
      printf("0x%x -> 0x%x\n", i, vm->memory[i]);
    }
  }
  printf("\n");
}
