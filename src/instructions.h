#ifndef INCLUDE_SRC_INSTRUCTIONS_H_
#define INCLUDE_SRC_INSTRUCTIONS_H_

enum Instructions {
  LOAD = 0x0,
  DUMP = 0x1,
  LDA = 0x2,
  PUSH = 0x3,
  POP = 0x4,
  ADD = 0x5,
  ADC = 0x6,
  SUB = 0x7,
  SBB = 0x8,
  CMP = 0x9,
  JNZ = 0xA,
  AND = 0xB,
  NOT = 0xC,
  OR = 0xD,
  IN = 0xE,
  OUT = 0xF,
};

#endif // INCLUDE_SRC_INSTRUCTIONS_H_
