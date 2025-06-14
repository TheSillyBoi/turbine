#ifndef INCLUDE_SRC_INSTRUCTIONS_H_
#define INCLUDE_SRC_INSTRUCTIONS_H_

enum Instructions {
  LOAD = 0x0,
  DUMP = 0x1,
  LDD = 0x2,
  PUSH = 0x3,
  POP = 0x4,
  ADD = 0x5,
  ADC = 0x6,
  SUB = 0x7,
  SBB = 0x8,
  NOT = 0x9,
  OR = 0xA,
  CMP = 0xC,
  JUMP = 0xD,
  DIN = 0xE,
  DOUT = 0xF,
  HLT = 0x10
};

#endif // INCLUDE_SRC_INSTRUCTIONS_H_
