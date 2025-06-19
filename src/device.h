#ifndef INCLUDE_SRC_DEVICE_H_
#define INCLUDE_SRC_DEVICE_H_

#include <stdint.h>

#define DEVICES_LENGTH 1
typedef struct VirtualMachine VirtualMachine;
typedef struct Device Device;

typedef struct Device {
  // for memory mapping purposes
  uint16_t start;
  uint16_t end;
  uint8_t prev_change_indic;
  // on each cpu cycle, this function gets ran
  void (*on_cycle)(VirtualMachine *vm);
} Device;

extern Device console;
extern Device *devices[];

#endif // INCLUDE_SRC_DEVICE_H_
