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
