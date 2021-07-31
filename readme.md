<p align="center"><img src="logo.png"/></p>
<h1 align="center">omam8</h1>
<h2 align="center">an 8-bit processor architecture</h2>

---

this repository contains all the files used for development of omam8.

## specifications
you can find the specifications in the [spec.md file](spec.md)

## file rundown
- `asm.py` - omam8 assembler
- `emu/` - omam8 emulator (written in C and SDL2, might not build on Windows - try under Cygwin)
- `genimg.py` - oasm generator for displaying images in VRAM with RLE (requires Pillow)
- `input.png` - required by `genimg.py`
- `rle.oasm` - RLE routine used by `genimg.py`
- `vrampng.py` - turns VRAM dumps into PNG files (requires Pillow)
