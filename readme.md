<img align="center" src="logo.png"/>
<h1 align="center">omam8</h1>
<h2 align="center">an 8-bit processor architecture</h2>

---

this repository contains all the files used for development of omam8.

## specifications
you can find the specifications in the [spec.md file](spec.md)

## file rundown
- `asm.py` - omam8 assembler
- `emu.py` - omam8 emulator
- `genimg.py` - oasm generator for displaying images in VRAM with RLE (requires Pillow)
- `input.png` - required by `genimg.py`
- `rle.oasm` - RLE routine used by `genimg.py`
- `vrampng.py` - turns VRAM dumps into PNG files (requires Pillow)
