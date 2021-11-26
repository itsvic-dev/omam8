# omam8 - an 8-bit processor architecture

**Revision** 2.3

## Specifications

- little endian
- drives a 240 x 160 grayscale display
- 16 digital I/O pins
- 16-bit timer with 1ms precision

## Memory layout

### `MRAM` - Main RAM

Full size: `0x10000`

- `0x0000` - `0x00FF` = Stack
- `0x0100` - `0x7FFF` = RAM
- `0x8000` - `0xFFFF` (inclusive) = ROM

### `VRAM` - Video RAM

Full size: `0x9600`

- `0x0000` - `0x9600` (exclusive) = Display contents

## Registers

| Register        | Initial value | Bits |
| --------------- | ------------- | ---- |
| Program counter | `0x8000`      | 16   |
| Remainder       | `0x00`        | 8    |
| Stack pointer   | `0x00FF`      | 16   |
| Timer           | `0x0000`      | 16   |
| A               | `0x00`        | 8    |
| B               | `0x00`        | 8    |
| Zero flag       | `0x00`        | 8    |

## Instructions

| Assembler instruction | Representative byte | Parameters | Description |
| --------------------- | ------------------- | ---------- | ----------- |
| `hlt` | `0x00` | - | Halt the CPU. |
| `lda` | `0x01` | MRAM address | Load the value in the MRAM address to the A register. |
| `ldb` | `0x02` | MRAM address | Load the value in the MRAM address to the B register. |
| `sta` | `0x03` | MRAM address | Store the value in A register to the MRAM address. |
| `stb` | `0x04` | MRAM address | Store the value in B register to the MRAM address. |
| `adda` | `0x05` | MRAM address | Add the value in the MRAM address to the A register. |
| `addb` | `0x06` | MRAM address | Add the value in the memory address to the B register. |
| `suba` | `0x07` | MRAM address | Subtract the value in the MRAM address from the A register. |
| `subb` | `0x08` | MRAM address | Subtract the value in the MRAM address from the B register. |
| `j` | `0x09` | MRAM address | Jump to the MRAM address. |
| `jab` | `0x0A` | - | Jump to the MRAM address stored in registers, where the B register is treated as the upper byte and the A register is treated as the lower byte. |
| `spua` | `0x0B` | - | Push the value in the A register to the stack. |
| `spub` | `0x0C` | - | Push the value in the B register to the stack. |
| `spoa` | `0x0D` | - | Pop the value from the stack to the A register. |
| `spob` | `0x0E` | - | Pop the value from the stack to the B register. |
| `vlda` | `0x0F` | VRAM address | Load the value in the VRAM address to the A register. |
| `vldb` | `0x10` | VRAM address | Load the value in the VRAM address to the B register. |
| `vsta` | `0x11` | VRAM address | Store the value in A register to the VRAM address. |
| `vstb` | `0x12` | VRAM address | Store the value in B register to the VRAM address. |
| `inab` | `0x13` | - | Read the value stored in the pin - defined in the A register - into the B register. |
| `outab` | `0x14` | - | Write the value in the B register into the pin defined in the A register. |
| `pinab` | `0x15` | - | Set the mode of the pin - defined in the A register - with the value in the B register. |
| `jza` | `0x16` | MRAM address | Jump to the MRAM address if the value in the A register is zero. |
| `jzb` | `0x17` | MRAM address | Jump to the MRAM address if the value in the B register is zero. |
| `jnza` | `0x18` | MRAM address | Jump to the MRAM address if the value in the A register is not zero. |
| `jnzb` | `0x19` | MRAM address | Jump to the MRAM address if the value in the B register is not zero. |
| `jeqab` | `0x1A` | MRAM address | Jump to the MRAM address if the value in the A register is equal to one in the B register. |
| `tsta` | `0x1B` | - | Start the timer. |
| `tsto` | `0x1C` | - | Stop the timer. |
| `tldal` | `0x1D` | - | Load the lower byte of the timer register into the A register. |
| `tldau` | `0x1E` | - | Load the upper byte of the timer register into the A register. |
| `tldbl` | `0x1F` | - | Load the lower byte of the timer register into the B register. |
| `tldbu` | `0x20` | - | Load the upper byte of the timer register into the B register. |
| `ldra` | `0x21` | - | Load the remainder into the A register. |
| `ldrb` | `0x22` | - | Load the remainder into the B register. |
| `jnzr` | `0x23` | MRAM address | Jump to the MRAM address if the remainder is not zero. |
| `lsha` | `0x24` | MRAM address | Does an arithmetic shift left on the A register by the value in the MRAM address. |
| `lshb` | `0x25` | MRAM address | Does an arithmetic shift left on the B register by the value in the MRAM address. |
| `lshab` | `0x26` | - | Does an arithmetic shift left on the A register by the value in the B register. |
| `rsha` | `0x27` | MRAM address | Does an arithmetic shift right on the A register by the value in the MRAM address. |
| `rshb` | `0x28` | MRAM address | Does an arithmetic shift right on the B register by the value in the MRAM address. |
| `rshab` | `0x29` | - | Does an arithmetic shift right on the A register by the value in the B register. |
| `anda` | `0x2A` | MRAM address | Does an AND operation on the A register with the value in the MRAM address. |
| `andb` | `0x2B` | MRAM address | Does an AND operation on the B register with the value in the MRAM address. |
| `andab` | `0x2C` | - | Does an AND operation on the A register with the value in the B register. |
| `ora` | `0x2D` | MRAM address | Does an OR operation on the A register with the value in the MRAM address. |
| `orb` | `0x2E` | MRAM address | Does an OR operation on the B register with the value in the MRAM address. |
| `orab` | `0x2F` | - | Does an OR operation on the A register with the value in the B register. |
| `xora` | `0x30` | MRAM address | Does an XOR operation on the A register with the value in the MRAM address. |
| `xorb` | `0x31` | MRAM address | Does an XOR operation on the B register with the value in the MRAM address. |
| `xorab` | `0x32` | - | Does an XOR operation on the A register with the value in the B register. |
| `jz` | `0x33` | MRAM address | Jump to the MRAM address if the zero flag is equal to 1. |
| `jnz` | `0x34` | MRAM address | Jump to the MRAM address if the zero flag is equal to 0. |
| `setea` | `0x35` | - | Sets the A register to 1 if the zero flag is equal to 1, 0 otherwise. |
| `seteb` | `0x36` | - | Sets the B register to 1 if the zero flag is equal to 1, 0 otherwise. |
| `setnea` | `0x37` | - | Sets the A register to 1 if the zero flag is equal to 0, 0 otherwise. |
| `setneb` | `0x38` | - | Sets the B register to 1 if the zero flag is equal to 0, 0 otherwise. |
| `cmpa` | `0x39` | MRAM address | Sets the zero flag to 1 if the A register is equal to the value in the MRAM address, 0 otherwise. |
| `cmpb` | `0x3A` | MRAM address | Sets the zero flag to 1 if the B register is equal to the value in the MRAM address, 0 otherwise. |
| `cmpab` | `0x3B` | - | Sets the zero flag to 1 if the A register is equal to the B register, 0 otherwise. |
| `addab` | `0x3C` | - | Add the value in the B register to the A register. |
| `subab` | `0x3D` | - | Subtract the value in the B register from the A register. |

## Assembler pseudoinstructions
Parameter types:
 - `Any`: Register (`%a`, `%b`, `%r`), value (`$0x1`) or memory address (`0x0001`)

**TODO**

## Pin modes
| Pin mode | Description |
| -------- | ----------- |
| `0x00` | Pin disabled. Default mode. |
| `0x01` | Read-only. |
| `0x02` | Write-only. |
