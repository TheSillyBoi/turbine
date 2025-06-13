#ifndef INCLUDE_SRC_DEVICE_H_
#define INCLUDE_SRC_DEVICE_H_

#define DEVICES_LENGTH 2

#include <stdint.h>

typedef struct Device {
  uint8_t id;
  void (*in)(uint8_t data);
  uint8_t (*out)();
  void (*init)();
  uint8_t buffer[];
} Device;

extern Device console;
extern Device screen;

#endif // INCLUDE_SRC_DEVICE_H_
