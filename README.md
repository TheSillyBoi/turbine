# Turbine

[![asciicast](https://asciinema.org/a/bkevNpgd5Q4aMDVD2sc0ZMEN4.svg)](https://asciinema.org/a/bkevNpgd5Q4aMDVD2sc0ZMEN4)

Custom CPU Architecture and emulator for that architecture made for fun!

## Build

You need GNU make and a C23 compiler installed. GCC and Clang work. Run `make release` and the compiled binary should be in the build folder.

## Details

- 16-bit processor
- Single-threaded
- 0x10000 bytes of memory
- Little-endian

## Registers

- `ra`  -> General Purpose Register
- `rb`  -> General Purpose Register
- `rc`  -> General Purpose Register
- `rd`  -> General Purpose Register
- `re`  -> General Purpose Register
- `rsp` -> Stack Pointer
- `rbp` -> Base Pointer
- `rpc` -> Instruction Pointer
- `rac` -> Accumulator
- `rfl` -> Status Register

## Memory

- `0x0`          -> Console
- `0x1 - 0xFFFF` -> RAM

## Instructions

- `0x0`  -> `LOAD [reg flag] [mem addr]`
- `0x1`  -> `DUMP [reg flag] [mem addr]`
- `0x2`  -> `MOVE [reg flag] [reg flag]`*
- `0x3`  -> `LDD [reg flag] [data]`
- `0x4`  -> `PUSH [reg flag]`
- `0x5`  -> `POP [reg flag]`
- `0x6`  -> `ADD [reg flag] [reg flag]`*\*
- `0x7`  -> `ADC [reg flag] [reg flag]`*\*
- `0x8`  -> `SUB [reg flag] [reg flag]`*\*
- `0x9`  -> `SBB [reg flag] [reg flag]`*\*
- `0xA`  -> `NOT [reg flag]`*\*
- `0xB`  -> `OR [reg flag] [reg flag]`*\*
- `0xC`  -> `AND [reg flag] [reg flag]`*\*
- `0xD`  -> `CMP [reg flag] [reg flag]`
- `0xE`  -> `JUMP [status flag] [mem addr]`
- `0xF`  -> `HLT`

*Two byte and one byte instructions do the same for this.
This instruction also moves from register 1 to register 2

*\*These instructions save the result to the accumulator

### Important Details

- When passing in data with a command that uses a register flag,
be wary of the amount of bytes that you type in.
- If it has `TWO_BYTES` attached to it, type in two different bytes
in little-endian order. Messing this up can mean that a command can be
interpreted as data and stuff gets messed up.
- Please don't try to manually bit fiddle to build binaries! Use the assembly script
I wrote in lua.
- Calling convention: Push arguments to stack

### Register Flags

- `0x0` -> `RA_BYTE`               -> 1 byte to register A
- `0x1` -> `RA_TWO_BYTES`          -> 2 bytes to register A
- `0x2` -> `RB_BYTE`               -> 1 byte to register B
- `0x3` -> `RB_TWO_BYTES`          -> 2 bytes to register B
- `0x4` -> `RC_BYTE`               -> 1 byte to register C
- `0x5` -> `RC_TWO_BYTES`          -> 2 bytes to register C
- `0x6` -> `RD_BYTE`               -> 1 byte to register D
- `0x7` -> `RD_TWO_BYTES`          -> 2 bytes to register D
- `0x8` -> `RE_BYTE`               -> 1 byte to register E
- `0x9` -> `RE_TWO_BYTES`          -> 2 bytes to register E
- `0xa` -> `STACK_PTR`             -> 2 bytes to stack pointer
- `0xb` -> `BASE_PTR`              -> 2 bytes to base pointer
- `0xc` -> `ACCUMULATOR_BYTE`      -> 1 byte to accumulator
- `0xd` -> `ACCUMULATOR_TWO_BYTE`  -> 2 byte to accumulator
- `0xe` -> `STATUS`                -> Status flag(1 byte) to status register

### Status Flags

- `0x0` -> `CMP_EQUAL_TO`     -> When `CMP` finds both registers equal to each other
- `0x1` -> `CMP_GREATER_THAN` -> When `CMP` finds register a greater than b
- `0x2` -> `CMP_LESS_THAN`    -> When `CMP` finds register a less than b
- `0x3` -> `ADD_CARRY`        -> When `ADC` or `ADD` set a carry flag
- `0x4` -> `ADD_NO_CARRY`     -> When `ADC` or `ADD` don't set a carry flag
- `0x5` -> `SUB_BORROW`       -> When `SBB` or `SUB` set a carry flag
- `0x6` -> `SUB_NO_BORROW`    -> When `SBB` or `SUB` don't set a carry flag
