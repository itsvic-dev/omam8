<img align="right" src="assets/logo.png" alt="omam8 logo" />

# omam8
*an 8-bit processor architecture*

---

this repository contains all the files used for development of omam8.

## specification
[the architecture specification is here.](spec/readme.md)

## file rundown
- `omam8asm/` - omam8 assembler, callable with `python3 -m omam8asm`
- `omam8emu/` - omam8 emulator (written in C++ and SDL2, requires JetBrains Mono to be installed, might not build or work on Windows)
- `vrampng.py` - turns VRAM dumps into PNG files (requires Pillow)

# test programs
```
loop:
    add $1, %a
    mv %a, v0x0000
    j .loop
```

![test program 1](assets/testprog.png)
