# omam8 - an 8-bit processor architecture

**Revision** 3.0

## Specifications

- little endian
- 32 digital I/O pins
- up to 64 ROM banks

## Memory layout

### `MRAM` - Main RAM

- `0x0000` - `0x7FFF` = RAM
- `0x8000` - `0xBFFF` = ROM bank 0 (lower)
- `0xC000` - `0xFFFF` = ROM bank 0-63 (upper)

## Registers

| Register        | Initial value | Argument representation | Bits |
| --------------- | ------------- | ----------------------- | ---- |
| Program counter | `0x8000`      | `0b000001`              | 16   |
| Stack pointer   | `0x00FF`      | `0b000010`              | 16   |
| `A`             | `0x00`        | `0b000100`              | 8    |
| `B`             | `0x00`        | `0b001000`              | 8    |
| `C`             | `0x00`        | `0b010000`              | 8    |
| `D`             | `0x00`        | `0b100000`              | 8    |

### Combined registers

Combined registers can be used to dynamically write to 16-bit addresses without copying and writing code at runtime.

16-bit registers can be internally handled as combined registers, and opcodes writing to combined registers should also handle the 16-bit registers.

- `BA` - Registers `B` and `A`. Represented by `0b001100`.
- `DC` - Registers `D` and `C`. Represented by `0b110000`.

Other register bit masks may be supported by the hardware, but may lead to undocumented behavior.

## Opcode chart

| U \ L | `0`   | `1`   | `2`     | `3`    | `4`    | `5`   | `6`   | `7` | `8` | `9` | `A` | `B` | `C` | `D` | `E` | `F`   |
| ----- | ----- | ----- | ------- | ------ | ------ | ----- | ----- | --- | --- | --- | --- | --- | --- | --- | --- | ----- |
| `0`   | `nop` | `mov` | `mov16` | `load` | `stor` |
| `1`   |       | `eq`  | `ne`    | `lt`   | `gt`   | `le`  | `ge`  |
| `2`   | `jmp` | `jeq` | `jne`   | `jlt`  | `jgt`  | `jle` | `jge` |
| `3`   | `sio` | `rio` | `wio`   |
| `4`   | `add` | `sub` | `shl`   | `shr`  |
| `5`   |
| `6`   |
| `7`   |
| `8`   |
| `9`   |
| `A`   |
| `B`   |
| `C`   |
| `D`   |
| `E`   |
| `F`   |       |       |         |        |        |       |       |     |     |     |     |     |     |     |     | `hlt` |

See [the opcode breakdown](ops.md) for a more detailed explanation.

## Banking

Writing to anywhere in the upper (banked) ROM area will control the current ROM bank.

You can write bytes 0-63 to change the current banks. If an invalid bank is loaded, reads to the upper ROM area will return `0xFF`.

Writing to the lower ROM area may result in undefined behavior.

## Pin modes

| Pin mode | Description                 |
| -------- | --------------------------- |
| `0b0`    | Pin disabled. Default mode. |
| `0b01`   | Read-only.                  |
| `0b10`   | Write-only.                 |

Other combinations are unsupported and may result in undefined behavior.
