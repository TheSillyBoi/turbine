#ifndef INCLUDE_SRC_DEVICE_H_
#define INCLUDE_SRC_DEVICE_H_

#include <stdint.h>
typedef struct Device {
  uint8_t id;
  void (*in)(uint8_t data);
  uint8_t (*out)();
} Device;

#endif // INCLUDE_SRC_DEVICE_H_
