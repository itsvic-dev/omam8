# Definitions
- *immediate* - a byte stored in the ROM
- *16-bit immediate* - 2 bytes stored in the ROM, forming a 16-bit LE word
- address - a *16-bit immediate*

# Opcodes

## Opcode formatting
Unless specified otherwise, the opcode format is almost always:
- opcode
- **src** (if present)
- **dst** (if present)

## Opcode definitions

- `nop` (`0b00000000`) - no-op
- `hlt` (`0b11111111`) - halt the CPU
- `mov` (`0b1000000X`) - copy from **src** to register **dst**
    - `movi` (`0b10000000`) - **src** is an *immediate*
    - `movr` (`0b10000001`) - **src** is a register
- `mov16` (`0b1000100X`) - copy from 16-bit **src** to combined register **dst**
    - `mov16a` (`0b10001000`) - **src** is an address
    - `mov16r` (`0b10001001`) - **src** is a combined register
- `add` (`0b0100000X`) - add **src** into register **dst**
    - `addi` (`0b01000000`) - **src** is an *immediate*
    - `addr` (`0b01000001`) - **src** is a register
- `sub` (`0b0110000X`) - subtract **src** from register **dst**
    - `subi` (`0b01100000`) - **src** is an *immediate*
    - `subr` (`0b01100001`) - **src** is a register
- `push` (`0b1010000X`) - push **src** onto stack
    - `pushi` (`0b10100000`) - **src** is an *immediate*
    - `pushr` (`0b10100001`) - **src** is a register
- `pop` (`0b1110000X`) - pop from stack into **dst**
    - `popr` (`0b11100000`) - **dst** is a register
    - `popa` (`0b11100001`) - **dst** is an address
- `sio` (`0b0001000X`) - set IO pin mode
    - takes argument `0b0XXXXXYY`
        - `0bXXXXX` - target pin, from 0-31 (inclusive)
        - `0bYY` - IO mode, described in the spec readme
    - `sioi` (`0b00010000`) - argument is an *immediate*
    - `sior` (`0b00010001`) - argument is in a register
- `rio` (`0b001100XY`) - read IO pin **src** to **dst**
    - `rioia` (`0b00110000`) - **src** is an *immediate*, **dst** is an address
    - `rioir` (`0b00110001`) - **src** is an *immediate*, **dst** is a register
    - `riora` (`0b00110010`) - **src** is a register, **dst** is an address
    - `riorr` (`0b00110011`) - **src** is a register, **dst** is a register
- `wio` (`0b001110XY`) - write **src** to IO pin **dst**
    - `wioai` (`0b00111000`) - **src** is an address, **dst** is an *immediate*
    - `wioar` (`0b00111001`) - **src** is an address, **dst** is a register
    - `wiori` (`0b00111010`) - **src** is a register, **dst** is an *immediate*
    - `wiorr` (`0b00111011`) - **src** is a register, **dst** is a register
- `jmp` (`0b1011000X`) - jump to **dst**
    - `jmpa` (`0b10110000`) - **dst** is an address
    - `jmpr` (`0b10110001`) - **dst** is a combined register
- `shl` (`0b1111000X`) - shift register **dst** to the left by **src**
    - `shli` (`0b11110000`) - **src** is an *immediate*
    - `shlr` (`0b11110001`) - **src** is a register
- `shr` (`0b1111100X`) - shift register **dst** to the right by **src**
    - `shri` (`0b11111000`) - **src** is an *immediate*
    - `shrr` (`0b11111001`) - **src** is a register
