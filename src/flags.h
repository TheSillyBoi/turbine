#ifndef INCLUDE_SRC_FLAGS_H_
#define INCLUDE_SRC_FLAGS_H_

typedef enum RegisterFlag {
  RA_BYTE = 0x0,
  RA_TWO_BYTES = 0x1,
  RB_BYTE = 0x2,
  RB_TWO_BYTES = 0x3,
  STACK_PTR = 0x4,
  BASE_PTR = 0x5,
  ACCUMULATOR = 0x6,
  STATUS = 0x7
} RegisterFlag;

typedef enum DeviceFlag {
  CONSOLE = 0x0,
  SCREEN = 0x1,
} DeviceFlag;

typedef enum StatusFlag {
  CMP_EQUAL_TO = 0x0,
  CMP_GREATER_THAN = 0x1,
  CMP_LESS_THAN = 0x2,
  ADD_CARRY = 0x3,
  ADD_NO_CARRY = 0x4,
  SUB_BORROW = 0x5,
  SUB_NO_BORROW = 0x6,
} StatusFlag;

#endif // INCLUDE_SRC_FLAGS_H_
