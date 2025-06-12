#ifndef INCLUDE_SRC_DEVICE_H_
#define INCLUDE_SRC_DEVICE_H_

#define DEVICES_LENGTH 1

#include <stdint.h>

typedef struct Device {
  uint8_t id;
  void (*in)(uint8_t data);
  uint8_t (*out)();
} Device;

extern Device console;

#endif // INCLUDE_SRC_DEVICE_H_
