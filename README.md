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
- `rac` -> Accumulator
- `rfl` -> Status Register

## Memory

- `0x0000 - 0x0400` -> ROM(TEXT)
- `0x0401 - 0x0800` -> ROM(DATA)
- `0x0801 - 0xFFFF` -> RAM

## Instructions

- `0x0`  -> `LOAD [reg flag] [mem addr]`
- `0x1`  -> `DUMP [reg flag] [mem addr]`
- `0x2`  -> `MOVE [reg flag] [reg flag]`*
- `0x3`  -> `LDD [reg flag] [data]`
- `0x4`  -> `PUSH [reg flag]`
- `0x5`  -> `POP [reg flag]`
- `0x6`  -> `ADD [reg flag] [reg flag]`
- `0x7`  -> `ADC [reg flag] [reg flag]`
- `0x8`  -> `SUB [reg flag] [reg flag]`
- `0x9`  -> `SBB [reg flag] [reg flag]`
- `0xA`  -> `NOT [reg flag]`
- `0xB`  -> `OR [reg flag] [reg flag]`
- `0xC`  -> `AND [reg flag] [reg flag]`
- `0xD`  -> `CMP [reg flag] [reg flag]`
- `0xE`  -> `JUMP [status flag] [mem addr]`
- `0xF`  -> `DIN [device flag] [reg flag]`
- `0x10` -> `DOUT [device flag] [reg flag]`
- `0x11` -> `HLT`

*`0x1` and `0x3` are the same as `0x0` and `0x2` respectively

### Register Flags

- `0x0` -> `RA_BYTE`      -> 1 byte to register A
- `0x1` -> `RA_TWO_BYTES` -> 2 bytes to register A
- `0x2` -> `RB_BYTE`      -> 1 byte to register B
- `0x3` -> `RB_TWO_BYTES` -> 2 bytes to register B
- `0x4` -> `STACK_PTR`    -> 2 bytes to stack pointer
- `0x5` -> `BASE_PTR`     -> 2 bytes to base pointer
- `0x6` -> `ACCUMULATOR`  -> 1 byte to accumulator
- `0x7` -> `STATUS`       -> Status flag(1 byte) to status register

### Device Flags

- `0x0` -> Console
- `0x1` -> Screen

### Status Flags

- `0x0` -> `CMP_EQUAL_TO`     -> When `CMP` finds both registers equal to each other
- `0x1` -> `CMP_GREATER_THAN` -> When `CMP` finds register a greater than b
- `0x2` -> `CMP_LESS_THAN`    -> When `CMP` finds register a less than b
- `0x3` -> `ADD_CARRY`        -> When `ADC` or `ADD` set a carry flag
- `0x3` -> `SUB_BORROW`       -> When `SBB` or `SUB` set a carry flag
