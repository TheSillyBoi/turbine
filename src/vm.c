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
    printf("LOAD\n");
    uint16_t *reg = NULL;
    reg_flag_parser(vm, &reg);
    switch (vm->memory[vm->instruction_pointer - 1]) {
    case RA_BYTE:
    case RB_BYTE:
    case RC_BYTE:
    case RD_BYTE:
    case RE_BYTE:
    case STATUS: {
      uint8_t left = vm->memory[vm->instruction_pointer++];
      uint8_t right = vm->memory[vm->instruction_pointer++];
      *reg = vm->memory[u16_combine(left, right)];
      break;
    }
    case RA_TWO_BYTES:
    case RB_TWO_BYTES:
    case RC_TWO_BYTES:
    case RD_TWO_BYTES:
    case RE_TWO_BYTES:
    case STACK_PTR:
    case BASE_PTR:
    case ACCUMULATOR: {
      uint8_t left = vm->memory[vm->instruction_pointer++];
      uint8_t right = vm->memory[vm->instruction_pointer++];
      uint16_t memory = u16_combine(left, right);
      uint8_t left_data = vm->memory[memory];
      uint8_t right_data = vm->memory[memory + 1];
      *reg = u16_combine(left_data, right_data);
      break;
    }
    }
    break;
  }
  case DUMP: {
    printf("DUMP\n");
    uint16_t *reg = NULL;
    reg_flag_parser(vm, &reg);
    switch (vm->memory[vm->instruction_pointer - 1]) {
    case RA_BYTE:
    case RB_BYTE:
    case RC_BYTE:
    case RD_BYTE:
    case RE_BYTE:
    case STATUS: {
      uint8_t left = vm->memory[vm->instruction_pointer++];
      uint8_t right = vm->memory[vm->instruction_pointer++];
      vm->memory[u16_combine(left, right)] = *reg;
      break;
    }
    case RA_TWO_BYTES:
    case RB_TWO_BYTES:
    case RC_TWO_BYTES:
    case RD_TWO_BYTES:
    case RE_TWO_BYTES:
    case STACK_PTR:
    case BASE_PTR:
    case ACCUMULATOR: {
      uint8_t left_mem = vm->memory[vm->instruction_pointer++];
      uint8_t right_mem = vm->memory[vm->instruction_pointer++];
      uint8_t memory_addr = u16_combine(left_mem, right_mem);
      uint8_t left_reg = 0;
      uint8_t right_reg = 0;
      u16_split(*reg, &left_reg, &right_mem);
      vm->memory[memory_addr] = left_reg;
      vm->memory[memory_addr + 1] = right_reg;
      break;
    }
    }
    break;
  }
  case MOVE: {
    printf("MOVE\n");
    uint16_t *reg1 = NULL;
    uint16_t *reg2 = NULL;

    reg_flag_parser(vm, &reg1);
    reg_flag_parser(vm, &reg2);

    *reg2 = *reg1;

    break;
  }
  case LDD: {
    printf("LDD\n");
    uint16_t *reg = NULL;
    reg_flag_parser(vm, &reg);
    switch (vm->memory[vm->instruction_pointer - 1]) {
    case RA_BYTE:
    case RB_BYTE:
    case RC_BYTE:
    case RD_BYTE:
    case RE_BYTE:
    case STATUS: {
      *reg = vm->memory[vm->instruction_pointer++];
      break;
    }
    case RA_TWO_BYTES:
    case RB_TWO_BYTES:
    case RC_TWO_BYTES:
    case RD_TWO_BYTES:
    case RE_TWO_BYTES:
    case STACK_PTR:
    case BASE_PTR:
    case ACCUMULATOR: {
      uint8_t left = vm->memory[vm->instruction_pointer++];
      uint8_t right = vm->memory[vm->instruction_pointer++];
      *reg = u16_combine(left, right);
    }
    }
    break;
  }
  // left first, right last for push and reverse for pop
  case PUSH: {
    printf("PUSH\n");
    uint16_t *reg = NULL;
    reg_flag_parser(vm, &reg);
    switch (vm->memory[vm->instruction_pointer - 1]) {
    case RA_BYTE:
    case RB_BYTE:
    case RC_BYTE:
    case RD_BYTE:
    case RE_BYTE:
    case STATUS: {
      vm->memory[vm->stack_pointer--] = *reg;
      break;
    }
    case RA_TWO_BYTES:
    case RB_TWO_BYTES:
    case RC_TWO_BYTES:
    case RD_TWO_BYTES:
    case RE_TWO_BYTES:
    case STACK_PTR:
    case BASE_PTR:
    case ACCUMULATOR: {
      uint8_t left = 0;
      uint8_t right = 0;
      u16_split(vm->stack_pointer, &left, &right);
      vm->memory[vm->stack_pointer--] = left;
      vm->memory[vm->stack_pointer--] = right;
    }
    }
    break;
  }
  case POP: {
    printf("POP\n");
    uint16_t *reg = NULL;
    reg_flag_parser(vm, &reg);
    switch (vm->memory[vm->instruction_pointer - 1]) {
    case RA_BYTE:
    case RB_BYTE:
    case RC_BYTE:
    case RD_BYTE:
    case RE_BYTE:
    case STATUS: {
      *reg = vm->memory[vm->stack_pointer++];
      break;
    }
    case RA_TWO_BYTES:
    case RB_TWO_BYTES:
    case RC_TWO_BYTES:
    case RD_TWO_BYTES:
    case RE_TWO_BYTES:
    case STACK_PTR:
    case BASE_PTR:
    case ACCUMULATOR: {
      uint8_t right = vm->memory[vm->stack_pointer++];
      uint8_t left = vm->memory[vm->stack_pointer++];
      *reg = u16_combine(left, right);
    }
    }
    break;
  }
  case ADD: {
    printf("ADD\n");
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
    printf("ADC\n");
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
    printf("SUB\n");
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
    printf("SBB\n");
    uint16_t *reg1 = NULL;
    uint16_t *reg2 = NULL;

    reg_flag_parser(vm, &reg1);
    reg_flag_parser(vm, &reg2);

    uint8_t borrow_in_value = (vm->status_register == SUB_BORROW ? 1 : 0);
    uint16_t total_subtract_amount = (uint16_t)*reg2 + borrow_in_value;
    bool new_borrow_occurred = (*reg1 < total_subtract_amount);
    vm->accumulator = *reg1 - (uint8_t)total_subtract_amount;
    vm->status_register = new_borrow_occurred ? SUB_BORROW : SUB_NO_BORROW;

    break;
  }
  case NOT: {
    printf("NOT\n");
    uint16_t *reg1 = NULL;
    reg_flag_parser(vm, &reg1);

    vm->accumulator = ~(*reg1);
    break;
  }
  case OR: {
    printf("OR\n");
    uint16_t *reg1 = NULL;
    uint16_t *reg2 = NULL;

    reg_flag_parser(vm, &reg1);
    reg_flag_parser(vm, &reg2);

    vm->accumulator = *reg1 | *reg2;
    break;
  }
  case AND: {
    printf("AND\n");
    uint16_t *reg1 = NULL;
    uint16_t *reg2 = NULL;

    reg_flag_parser(vm, &reg1);
    reg_flag_parser(vm, &reg2);

    vm->accumulator = *reg1 & *reg2;
    break;
  }
  case CMP: {
    printf("CMP\n");
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
    printf("JUMP\n");
    uint8_t status = vm->memory[vm->instruction_pointer++];
    uint8_t left_memory_addr = vm->memory[vm->instruction_pointer++];
    uint8_t right_memory_addr = vm->memory[vm->instruction_pointer++];
    uint16_t memory_addr = u16_combine(left_memory_addr, right_memory_addr);
    if (status == vm->status_register) {
      vm->instruction_pointer = memory_addr;
    }

    break;
  }
  case DIN: {
    printf("DIN\n");
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
    printf("DOUT\n");
    uint8_t device_id = vm->memory[vm->instruction_pointer++];
    if (device_id > 0 && device_id < DEVICES_LENGTH) {
      if (vm->devices[device_id].out != NULL) {
        vm->accumulator = vm->devices[device_id].out();
      }
    }
    break;
  }
  case HLT: {
    printf("HLT\n");
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
  case RC_BYTE:
  case RC_TWO_BYTES: {
    *ptr = &vm->register_c;
    break;
  }
  case RD_BYTE:
  case RD_TWO_BYTES: {
    *ptr = &vm->register_d;
    break;
  }
  case RE_BYTE:
  case RE_TWO_BYTES: {
    *ptr = &vm->register_e;
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
  printf("Register C: 0x%x\n", vm->register_c);
  printf("Register D: 0x%x\n", vm->register_d);
  printf("Register E: 0x%x\n", vm->register_e);
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
