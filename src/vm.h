#ifndef INCLUDE_SRC_VM_H_
#define INCLUDE_SRC_VM_H_

#include "device.h"
#include "flags.h"
#include <stdbool.h>
#include <stdint.h>

#define MEMORY_SIZE 0xFFFF
#define RAM_START 0x2
#define RAM_END 0xFFFE

typedef struct VirtualMachine {
  uint16_t register_a;
  uint16_t register_b;
  uint16_t register_c;
  uint16_t register_d;
  uint16_t register_e;
  uint16_t stack_pointer;
  uint16_t base_pointer;
  uint16_t accumulator;
  uint16_t instruction_pointer;
  StatusFlag status_register;
  uint8_t *memory;
  Device **devices;
  bool halted;
} VirtualMachine;

VirtualMachine init_vm();
void init_program_vm(VirtualMachine *vm, uint8_t *text, uint16_t size);
void delete_vm(VirtualMachine *vm);
void step_vm(VirtualMachine *vm);
void debug_print_vm(VirtualMachine *vm);
void reg_flag_parser(VirtualMachine *vm, uint16_t **ptr);

#endif // INCLUDE_SRC_VM_H_
