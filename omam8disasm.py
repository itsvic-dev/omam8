#!/usr/bin/env python3

import os
import argparse
import omam8asm

parser = argparse.ArgumentParser(
    description="Disassembles a ROM file into (crude) omam8 assembler."
)

parser.add_argument(
    "input", type=str,
    help="input ROM file"
)
parser.add_argument(
    "-o", "--output", type=str,
    help="output omam8 assembler",
    default="rom.s"
)
parser.add_argument(
    "-v", "--version",
    action="version",
    version="omam8 disassembler v1.0.0, supporting spec v2.3.0"
)

args = parser.parse_args()

def get_16bit_from_8_bits(lower: int, upper: int):
    return (upper << 8) | lower


instructions = {
    0: ('hlt', 1),
    1: ('lda', 3),
    2: ('ldb', 3),
    3: ('sta', 3),
    4: ('stb', 3),
    5: ('adda', 3),
    6: ('addb', 3),
    7: ('suba', 3),
    8: ('subb', 3),
    9: ('j', 3),
    10: ('jab', 1),
    11: ('spua', 1),
    12: ('spub', 1),
    13: ('spoa', 1),
    14: ('spob', 1),
    15: ('vlda', 3),
    16: ('vldb', 3),
    17: ('vsta', 3),
    18: ('vstb', 3),
    19: ('inab', 1),
    20: ('outab', 1),
    21: ('pinab', 1),
    22: ('jza', 3),
    23: ('jzb', 3),
    24: ('jnza', 3),
    25: ('jnzb', 3),
    26: ('jeqab', 3),
    27: ('tsta', 1),
    28: ('tsto', 1),
    29: ('tldal', 1),
    30: ('tldau', 1),
    31: ('tldbl', 1),
    32: ('tldbu', 1),
    33: ('ldra', 1),
    34: ('ldrb', 1),
    35: ('jnzr', 1),
    36: ('lsha', 3),
    37: ('lshb', 3),
    38: ('lshab', 1),
    39: ('rsha', 3),
    40: ('rshb', 3),
    41: ('rshab', 1),
    42: ('anda', 3),
    43: ('andb', 3),
    44: ('andab', 1),
    45: ('ora', 3),
    46: ('orb', 3),
    47: ('orab', 1),
    48: ('xora', 3),
    49: ('xorb', 3),
    50: ('xorab', 1),
    51: ('jz', 3),
    52: ('jnz', 3),
    53: ('setea', 1),
    54: ('seteb', 1),
    55: ('setnea', 1),
    56: ('setneb', 1),
    57: ('cmpa', 3),
    58: ('cmpb', 3),
    59: ('cmpab', 3),
    60: ('addab', 1),
    61: ('subab', 1)
}

output = []

with open(args.input, "rb") as file:
    data = file.read()

pos = 0
force_raw = False

while pos < len(data):
    inst = data[pos]
    if inst not in instructions or force_raw:
        output.append(f"0x{inst:X} ; 0x{(0x8000 + pos):X}")
        pos += 1
        force_raw = False
    else:
        inst_name = instructions[inst][0]
        inst_size = instructions[inst][1]
        inst_args = []

        if inst_size > 1:
            for arg in range(int((inst_size - 1) / 2)):
                try:
                    lower = data[pos + 1]
                    upper = data[pos + 2]
                except:
                    # arguments go out of range, force raw byte output
                    force_raw = True
                    break
                arg16 = get_16bit_from_8_bits(data[pos + 1], data[pos + 2])
                inst_args.append(f"0x{arg16:X}")
        
        if force_raw:
            continue
        
        output.append(f"{inst_name}{' ' if inst_args else ''}{', '.join(inst_args)} ; 0x{(0x8000 + pos):X}")
        pos += inst_size

with open(args.output, "w+") as file:
    file.write('\n'.join(output))
