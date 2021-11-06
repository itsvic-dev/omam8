<img align="right" src="logo.png"/>

# omam8
*an 8-bit processor architecture*

---

this repository contains all the files used for development of omam8.

## specifications
you can find the specifications in the [spec.md file](spec.md)

## file rundown
- `omam8asm/` - omam8 assembler, callable with `python3 -m omam8asm`
- `omam8emu/` - omam8 emulator (written in C++ and SDL2, requires JetBrains Mono to be installed, might not build or work on Windows)
- `vrampng.py` - turns VRAM dumps into PNG files (requires Pillow)
