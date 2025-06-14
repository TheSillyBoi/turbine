# Turbine

Custom CPU Architecture and emulator for that architecture made for fun!

## Details

- 16-bit processor
- Single-threaded
- 0xfff bytes of memory
- Little-endian

## Registers

- `ra`  -> General Purpose Register
- `rb`  -> General Purpose Register
- `rsp` -> Stack Pointer
- `rbp` -> Base Pointer
- `rpc` -> Instruction Pointer
- `rfl` -> Status Register

## Memory

- `0x0000 - 0x0400` -> ROM(TEXT)
- `0x0401 - 0x0800` -> ROM(DATA)
- `0x0801 - 0xFFFF` -> RAM

## Instructions

| Opcode |  Name  | Description |
| :----- | :----- | :---------- |
| `0x0`  | `LOAD` | Load data into a register from memory. |
| `0x1`  | `DUMP` | Dump data off the register into memory. |
| `0x2`  | `LDD`  | Loads data specified into the register. Data is two bytes. |
| `0x3`  | `PUSH` | Push data onto the stack from register and adjusts RSP. |
| `0x4`  | `POP`  | Pop data off the stack into the register and adjusts RSP. Flag determines whether to pop off one or two bytes. |
| `0x5`  | `ADD`  | Add both general registers (without carrying) and save to accumulator, setting appropriate flags in status register. |
| `0x6`  | `ADC`  | Add both general registers and save to accumulator, setting appropriate flags in status register. |
| `0x7`  | `SUB`  | Subtract both general registers (without borrowing) and save to accumulator, setting appropriate flags in status register. |
| `0x8`  | `SBB`  | Subtract both general registers and save to accumulator, setting appropriate flags in status register. |
| `0x9`  | `CMP`  | Compare data from memory and checks if register data is greater than, less than, or equal to data from register. Dumps result to flags register. |
| `0xA`  | `JNZ`  | Jumps the instruction pointer to the given memory address if register is not zero. |
| `0xB`  | `AND`  | Bitwise AND with register and given memory address. |
| `0xC`  | `NOT`  | NOT with given memory address. |
| `0xD`  | `OR`   | Bitwise OR with register and given memory address. |
| `0xE`  | `DIN`  | Using ID of external device from flag, dump data from general register into device where second flag determines how many bytes. |
| `0xF`  | `DOUT` | Using ID of external device from flag, dump data from device into general register where second flag determines how many bytes. |
| `0x10` | `HLT`  | Halts the machine. |

### Register Flags

- `0x0` -> 1 byte to register A
- `0x1` -> 2 bytes to register A
- `0x2` -> 1 byte to register B
- `0x3` -> 2 bytes to register B
- `0x4` -> 2 bytes to stack pointer*
- `0x5` -> 2 bytes to base pointer*

### Status Flags

Flags that can be set in the status register. Not to be confused with flag arguements

*Only two byte versions exist since it's pointless to load
 just one byte to something that only should store 16-bit memory addresses
