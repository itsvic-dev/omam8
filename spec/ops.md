# Definitions
- *immediate* - a byte stored in the ROM.
- *16-bit immediate* - 2 bytes stored in the ROM, forming a 16-bit LE word.

# Opcodes

- `nop` (`0b00000000`) - no-op
- `hlt` (`0b11111111`) - halt the CPU
- `mov` (`0b1000000X`) - copy from **src** to register **dst**
    - `movi` (`0b10000000`) - **src** is an *immediate*
    - `movr` (`0b10000001`) - **src** is a register
- `add` (`0b0100000X`) - add from **src** into register **dst**
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
- `sio` (`0b00010000`) - set IO pin mode
    - takes argument `0b0XXXXXYY`
        - `0bXXXXX` - target pin, from 0-31 (inclusive)
        - `0bYY` - IO mode, described in the spec readme
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
