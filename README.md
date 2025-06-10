# Turbine

Custom CPU Architecture and emulator for that architecture made for fun!

## Details

- 16-bit processor
- 0xfff bytes of memory
- Little-endian

## Registers

- `ra` - General Purpose Register
- `rsp` - Stack Pointer
- `rbp` - Base Pointer
- `rpc` - Instruction Pointer
- `rfl` - Flags Register

## Memory

### Address Allocation

- `0x0000 - 0x0400` - ROM(TEXT)
- `0x0401 - 0x0800` - ROM(DATA)
- `0x0801 - 0xFFFF` - RAM

## Instructions

Realistically everything is memory. If a constant needs to be loaded into memory
, there will be special areas in ROM just for that

- 0: `LOAD [mem addr]` - Load data into a register from memory
- 1: `DUMP [mem addr]` - Dump data off the register into memory
- 2: `LDA [mem addr]` - Loads the address of a region of memory into the register
- 3: `PUSH` - Push data onto the stack from register and adjusts rsp
- 4: `POP` - Pop data off the stack into the register and adjusts rsp
- 5: `ADD [mem addr]` - Add data from given memory address(without carrying)
to data in register and write to register
- 6: `ADC [mem addr]` - Add data from given memory address to
 data in register and write to register
- 7: `SUB [mem addr]` - Subtract data from given memory address(without borrowing)
 from data in register and write to register.
- 8: `SBB [mem addr]` - Subtract data from given memory address
from data in register and write to register.
- 9: `CMP [mem addr]` - Compare data from memory and checks if it is
less than, equal to, or greater than
- A: `JNZ [mem addr]` - Jumps the instruction pointer to the
given memory address if register is not zero
- B: `AND [mem addr]` - Logical AND with register and given memory address
- C: `NOT [mem addr]` - NOT with given memory address
- D: `OR [mem addr]` - OR with register and given memory address
- E: `IN [mem addr]` - Using id of external device from memory,
dump data from external device into register
- F: `OUT [mem addr]` - Using id of external device from memory,
dump data from register into external device
