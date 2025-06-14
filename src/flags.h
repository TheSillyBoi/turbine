#ifndef INCLUDE_SRC_FLAGS_H_
#define INCLUDE_SRC_FLAGS_H_

enum Flags {
  RA_BYTE = 0x0,
  RA_TWO_BYTES = 0x1,
  RB_BYTE = 0x2,
  RB_TWO_BYTES = 0x3,
  STACK_PTR = 0x4,
  BASE_PTR = 0x5
};

#endif // INCLUDE_SRC_FLAGS_H_
