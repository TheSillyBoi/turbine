#ifndef INCLUDE_SRC_VM_H_
#define INCLUDE_SRC_VM_H_

#include "device.h"
#include <stdbool.h>
#include <stdint.h>

#define MEMORY_SIZE 0xFFFF
#define ROM_TEXT_START 0x0
#define ROM_TEXT_END 0x400
#define ROM_DATA_START 0x401
#define ROM_DATA_END 0x800
#define RAM_START 0x801
#define RAM_END 0xFFFE

typedef struct VirtualMachine {
  uint16_t general_register;
  uint16_t stack_pointer;
  uint16_t base_pointer;
  uint16_t instruction_pointer;
  uint8_t flags_register;
  uint8_t *memory;
  Device *devices;
  bool halted;
} VirtualMachine;

VirtualMachine init_vm();
void init_text_vm(VirtualMachine *vm, uint8_t *text, uint16_t size);
void init_data_vm(VirtualMachine *vm, uint8_t *data, uint16_t size);
void delete_vm(VirtualMachine *vm);
void step_vm(VirtualMachine *vm);
uint8_t deref_indirect_vm(uint8_t reg, VirtualMachine *vm, uint8_t shift);
void debug_print_vm(VirtualMachine *vm);

#endif // INCLUDE_SRC_VM_H_
