#ifndef INCLUDE_SRC_INSTRUCTIONS_H_
#define INCLUDE_SRC_INSTRUCTIONS_H_

enum Instructions {
  LOAD = 0x0,
  DUMP = 0x1,
  MOVE = 0x2,
  LDD = 0x3,
  PUSH = 0x4,
  POP = 0x5,
  ADD = 0x6,
  ADC = 0x7,
  SUB = 0x8,
  SBB = 0x9,
  NOT = 0xA,
  OR = 0xB,
  AND = 0xC,
  CMP = 0xD,
  JUMP = 0xE,
  DIN = 0xF,
  DOUT = 0x10,
  HLT = 0x11,
};

#endif // INCLUDE_SRC_INSTRUCTIONS_H_
