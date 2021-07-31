# omam8 - an 8-bit processor architecture

## specs
- little endian
- drives a 240 x 160 black and white display

## memory layout
### MRAM - Main RAM
Full size: `0xFFFF`

`0x0000` - `0x00FF` = Stack

`0x0100` - `0x7FFF` = RAM

`0x8000` - `0xFFFF` = ROM


### VRAM - Video RAM
Full size: `0x9600`

`0x0000` - `0x9600` = VRAM


## registers
| register | initial value |
| -------- | ------------- |
| Program counter | `0x8000` |
| Remainder | `0x00` |
| Stack pointer | `0x0000` |


## instructions
`JMP x` (`0x00 0xXX`) - Jump to a 16-bit address in MRAM

`JNZ x` (`0x01 0xXX`) - Jump to a 16-bit address in MRAM **if the remainder register is not 0**

`JZ x` (`0x02 0xXX`) - Jump to a 16-bit address in MRAM **if the remainder register is 0**

`ADD x y` (`0x03 0xXX 0xYY`) - Add contents of `x` to contents of `y` (`x + y`)

`SUB x y` (`0x04 0xXX 0xYY`) - Subtract contents of `x` from contents of `y` (`y - x`)

`MOV x y` (`0x05 0xXX 0xYY`) - Move/copy contents of `x` to address `y`

`VMOV x y` (`0x06 0xXX 0xYY`) - Move/copy contents of `x` in MRAM to address `y` in VRAM

`MUL x y` (`0x07 0xXX 0xYY`) - Multiply contents of `y` with contents of `x` (`y * x`)

`DIV x y` (`0x08 0xXX 0xYY`) - Divide contents of `y` with contents of `x` (`y / x`)

`SPU x` (`0x09 0xXX`) - Push contents of `x` to the stack

`SPO x` (`0x0A 0xXX`) - Pop an element from the stack into `x`

`CLR` (`0x0B`) - Clear remainder

`AND x y` (`0x0C 0xXX 0xYY`) - AND `x` and `y` together, storing the result in `y` (`y = x & y`)

`VRD x y` (`0x0D 0xXX 0xYY`) - Move/copy contents of `x` in VRAM to address `y` in MRAM

`HLT` (`0xFF`) - Halt the CPU
