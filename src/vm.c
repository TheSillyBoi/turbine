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
    case RC_BYTE: {
      vm->register_c = vm->memory[mem_addr];
      break;
    }
    case RC_TWO_BYTES: {
      vm->register_c =
          u16_combine(vm->memory[mem_addr], vm->memory[mem_addr + 1]);
      break;
    }
    case RD_BYTE: {
      vm->register_d = vm->memory[mem_addr];
      break;
    }
    case RD_TWO_BYTES: {
      vm->register_d =
          u16_combine(vm->memory[mem_addr], vm->memory[mem_addr + 1]);
      break;
    }
    case RE_BYTE: {
      vm->register_e = vm->memory[mem_addr];
      break;
    }
    case RE_TWO_BYTES: {
      vm->register_e =
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
    printf("DUMP\n");
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
    case RC_BYTE: {
      vm->memory[mem_addr] = vm->register_c;
      break;
    }
    case RC_TWO_BYTES: {
      uint8_t left = 0;
      uint8_t right = 0;
      u16_split(vm->register_c, &left, &right);
      vm->memory[mem_addr] = left;
      vm->memory[mem_addr + 1] = right;
      break;
    }
    case RD_BYTE: {
      vm->memory[mem_addr] = vm->register_d;
      break;
    }
    case RD_TWO_BYTES: {
      uint8_t left = 0;
      uint8_t right = 0;
      u16_split(vm->register_d, &left, &right);
      vm->memory[mem_addr] = left;
      vm->memory[mem_addr + 1] = right;
      break;
    }
    case RE_BYTE: {
      vm->memory[mem_addr] = vm->register_e;
      break;
    }
    case RE_TWO_BYTES: {
      uint8_t left = 0;
      uint8_t right = 0;
      u16_split(vm->register_e, &left, &right);
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
    switch (vm->memory[vm->instruction_pointer++]) {
    case RA_BYTE: {
      vm->register_a = vm->memory[vm->instruction_pointer++];
      break;
    }
    case RA_TWO_BYTES: {
      uint8_t left = vm->memory[vm->instruction_pointer++];
      uint8_t right = vm->memory[vm->instruction_pointer++];
      vm->register_a = u16_combine(left, right);
      break;
    }
    case RB_BYTE: {
      vm->register_b = vm->memory[vm->instruction_pointer++];
      break;
    }
    case RB_TWO_BYTES: {
      uint8_t left = vm->memory[vm->instruction_pointer++];
      uint8_t right = vm->memory[vm->instruction_pointer++];
      vm->register_b = u16_combine(left, right);
      break;
    }
    case RC_BYTE: {
      vm->register_c = vm->memory[vm->instruction_pointer++];
      break;
    }
    case RC_TWO_BYTES: {
      uint8_t left = vm->memory[vm->instruction_pointer++];
      uint8_t right = vm->memory[vm->instruction_pointer++];
      vm->register_c = u16_combine(left, right);
      break;
    }
    case RD_BYTE: {
      vm->register_d = vm->memory[vm->instruction_pointer++];
      break;
    }
    case RD_TWO_BYTES: {
      uint8_t left = vm->memory[vm->instruction_pointer++];
      uint8_t right = vm->memory[vm->instruction_pointer++];
      vm->register_d = u16_combine(left, right);
      break;
    }
    case RE_BYTE: {
      vm->register_e = vm->memory[vm->instruction_pointer++];
      break;
    }
    case RE_TWO_BYTES: {
      uint8_t left = vm->memory[vm->instruction_pointer++];
      uint8_t right = vm->memory[vm->instruction_pointer++];
      vm->register_e = u16_combine(left, right);
      break;
    }
    case STACK_PTR: {
      uint8_t left = vm->memory[vm->instruction_pointer++];
      uint8_t right = vm->memory[vm->instruction_pointer++];
      vm->stack_pointer = u16_combine(left, right);
      break;
    }
    case BASE_PTR: {
      uint8_t left = vm->memory[vm->instruction_pointer++];
      uint8_t right = vm->memory[vm->instruction_pointer++];
      vm->base_pointer = u16_combine(left, right);
      break;
    }
    case ACCUMULATOR: {
      uint8_t left = vm->memory[vm->instruction_pointer++];
      uint8_t right = vm->memory[vm->instruction_pointer++];
      vm->accumulator = u16_combine(left, right);
      break;
    }
    case STATUS: {
      vm->status_register = vm->memory[vm->instruction_pointer];
      break;
    }
    }
    break;
  }
  // left first, right last for push and reverse for pop
  case PUSH: {
    printf("PUSH\n");
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
      vm->memory[vm->stack_pointer] = vm->register_b;
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
    case RC_BYTE: {
      vm->memory[vm->stack_pointer] = vm->register_c;
      break;
    }
    case RC_TWO_BYTES: {
      uint8_t left = 0;
      uint8_t right = 0;
      u16_split(vm->register_c, &left, &right);
      vm->memory[vm->stack_pointer--] = left;
      vm->memory[vm->stack_pointer--] = right;
      break;
    }
    case RD_BYTE: {
      vm->memory[vm->stack_pointer] = vm->register_d;
      break;
    }
    case RD_TWO_BYTES: {
      uint8_t left = 0;
      uint8_t right = 0;
      u16_split(vm->register_d, &left, &right);
      vm->memory[vm->stack_pointer--] = left;
      vm->memory[vm->stack_pointer--] = right;
      break;
    }
    case RE_BYTE: {
      vm->memory[vm->stack_pointer] = vm->register_e;
      break;
    }
    case RE_TWO_BYTES: {
      uint8_t left = 0;
      uint8_t right = 0;
      u16_split(vm->register_e, &left, &right);
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
    printf("POP\n");
    switch (vm->memory[vm->instruction_pointer++]) {
    case RA_BYTE: {
      vm->register_a = vm->memory[++vm->stack_pointer];
      break;
    }
    case RA_TWO_BYTES: {
      uint8_t right = vm->memory[vm->stack_pointer++];
      uint8_t left = vm->memory[vm->stack_pointer++];

      vm->register_a = u16_combine(left, right);
      break;
    }
    case RB_BYTE: {
      vm->register_b = vm->memory[++vm->stack_pointer];
      break;
    }
    case RB_TWO_BYTES: {
      uint8_t right = vm->memory[vm->stack_pointer++];
      uint8_t left = vm->memory[vm->stack_pointer++];

      vm->register_b = u16_combine(left, right);
      break;
    }
    case RC_BYTE: {
      vm->register_c = vm->memory[++vm->stack_pointer];
      break;
    }
    case RC_TWO_BYTES: {
      uint8_t right = vm->memory[vm->stack_pointer++];
      uint8_t left = vm->memory[vm->stack_pointer++];

      vm->register_c = u16_combine(left, right);
      break;
    }
    case RD_BYTE: {
      vm->register_d = vm->memory[++vm->stack_pointer];
      break;
    }
    case RD_TWO_BYTES: {
      uint8_t right = vm->memory[vm->stack_pointer++];
      uint8_t left = vm->memory[vm->stack_pointer++];

      vm->register_d = u16_combine(left, right);
      break;
    }
    case RE_BYTE: {
      vm->register_e = vm->memory[++vm->stack_pointer];
      break;
    }
    case RE_TWO_BYTES: {
      uint8_t right = vm->memory[vm->stack_pointer++];
      uint8_t left = vm->memory[vm->stack_pointer++];

      vm->register_e = u16_combine(left, right);
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
