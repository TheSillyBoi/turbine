# Turbine

Custom CPU Architecture and emulator for that architecture made for fun!

## Details

- 16-bit processor
- Single-threaded
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

- 0: `LOAD [flag] [mem addr]` - Load data into a register from memory.
Flag determines whether to load 1 or 2 bytes to
 the general register or two bytes to
 the stack pointer or base pointer.
0x0 = 1 byte, 0x1 = 2 bytes, 0x2 = stack, 0x3 = base
- 1: `DUMP [flag] [mem addr]` - Dump data off the register into memory.
Flags same as `LOAD`
- 2: `LDA [flag] [mem addr]` - Loads the address of a region of memory into the register.
Flag determines which register to load to. 0x0 = general, 0x1 = stack, 0x2 = base.
- 3: `PUSH [flag]` - Push data onto the stack from register and adjusts rsp.
Flag determines which register to load to. Flags same as `LOAD`
- 4: `POP [flag]` - Pop data off the stack
into the register and adjusts rsp.
Flag determines whether to pop off one or two bytes.
- 5: `ADD [mem addr]` - Add data from given memory address(without carrying)
to data in register and write to register
- 6: `ADC [mem addr]` - Add data from given memory address to
 data in register and write to register
- 7: `SUB [mem addr]` - Subtract data from given memory address(without borrowing)
 from data in register and write to register.
- 8: `SBB [mem addr]` - Subtract data from given memory address
from data in register and write to register.
- 9: `CMP [mem addr]` - Compare data from memory and checks if register data is greater than, less than, or equal to data from register. Dumps result to flags register.
- A: `JNZ [mem addr]` - Jumps the instruction pointer to the
given memory address if register is not zero
- B: `AND [mem addr]` - Logical AND with register and given memory address
- C: `NOT [mem addr]` - NOT with given memory address
- D: `OR [mem addr]` - OR with register and given memory address
- E: `IN [mem addr]` - Using id of external device from memory,
dump data from external device into register
- F: `OUT [mem addr]` - Using id of external device from memory,
dump data from register into external device
