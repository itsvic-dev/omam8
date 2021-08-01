# omam8 - an 8-bit processor architecture

## specs
- little endian
- drives a 240 x 160 grayscale display
- 10 I/O pins
- 16-bit timer with 1ms precision

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
| A | `0x00` |
| Timer | `0x0000` |


## instructions
`JMP x` (`0x00 0xXX`) - Jump to a 16-bit address in MRAM

`JNZ x` (`0x01 0xXX`) - Jump to a 16-bit address in MRAM **if the remainder register is not 0**

`JZ x` (`0x02 0xXX`) - Jump to a 16-bit address in MRAM **if the remainder register is 0**

`ADD x y` (`0x03 0xXX 0xYY`) - Add contents of `x` to contents of `y` (`y + x`)

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

`LDA x` (`0x0E 0xXX`) - Load contents of `x` to the A register

`STA x` (`0x0F 0xXX`) - Store contents of the A register in `x`

`JEQ x y` (`0x10 0xXX 0xYY`) - Jump to `y` if contents in `x` are equal to the A register

`SPIN x y` (`0x11 0xXX 0xYY`) - Set PIN `x` to be an input (`mram[y] = 0x0`) or output (`mram[y] = 0x1`) pin, setting the PIN value to `0` if output

`IN x y` (`0x12 0xXX 0xYY`) - Read contents of pin `x` to address `y`

`OUT x y` (`0x13 0xXX 0xYY`) - Write contents of address `y` to pin `x`

`TSTA` (`0x14`) - Start the timer

`TSTO` (`0x15`) - Stop the timer, putting the final time in the Timer register

`STT x` (`0x16 0xXX`) - Store the 16-bit value of the Timer register in addresses `x` and `x+1`

`LSH x y` (`0x17 0xXX 0xYY`) - Do an arithmetic left-shift operation on `x` with `y` (`x << y`)

`RSH x y` (`0x18 0xXX 0xYY`) - Do an arithmetic right-shift operation on `x` with `y` (`x >> y`)

`HLT` (`0xFF`) - Halt the CPU
