#!/usr/bin/env python3

import os
import argparse
import omam8asm

parser = argparse.ArgumentParser(
    prog="omam8asm",
    description="Assembles omam8 assembler and produces a ROM file."
)

parser.add_argument(
    "input", type=str,
    help="input assembler file"
)
parser.add_argument(
    "-o", "--output", type=str,
    help="output ROM file",
    default="rom.bin"
)
parser.add_argument(
    "-v", "--version",
    action="version",
    version=f"omam8 assembler version {'.'.join([str(i) for i in omam8asm.__version__])}"
)

args = None

labels = {"root": []}
current_label = "root"
data = []
rom_start = 0x8000

instructions = {
    "hlt": 0x00,
    "lda": 0x01,
    "ldb": 0x02,
    "sta": 0x03,
    "stb": 0x04,
    "adda": 0x05,
    "addb": 0x06,
    "suba": 0x07,
    "subb": 0x08,
    "j": 0x09,
    "jab": 0x0A,
    "spua": 0x0B,
    "spub": 0x0B,
    "spoa": 0x0D,
    "spob": 0x0E,
    "vlda": 0x0F,
    "vldb": 0x10,
    "vsta": 0x11,
    "vstb": 0x12,
    "inab": 0x13,
    "outab": 0x14,
    "pinab": 0x15,
    "jza": 0x16,
    "jzb": 0x17,
    "jnza": 0x18,
    "jnzb": 0x19,
    "jeqab": 0x1A,
    "tsta": 0x1B,
    "tsto": 0x1C,
    "tldal": 0x1D,
    "tldau": 0x1E,
    "tldbl": 0x1F,
    "tldbu": 0x20,
    "ldra": 0x21,
    "ldrb": 0x22,
    "jnzr": 0x23
}

def get_16bit_as_8bit(value: int) -> tuple:
    lower = value & 0xFF
    upper = value >> 8 & 0xFF
    return lower, upper

def get_opcode_size(opcode: list) -> int:
    return 1 + (len(opcode) - 1) * 2

def get_label_size(label: str) -> int:
    size = 0
    for opcode in labels[label]:
        size += get_opcode_size(opcode)
    return size

def get_label_address(label: str) -> int:
    offset = 0
    for i in range(list(labels.keys()).index(label)):
        label_name = list(labels.keys())[i]
        offset += get_label_size(label_name)
    return rom_start + offset

def handle_add(parameters: list, line_number: int):
    instruction = "add"
    new_params = []
    if parameters[1].startswith("%"):
        if parameters[1] == "%r":
            raise omam8asm.exceptions.UnsupportedPseudoinstructionParameter(f"Attempted use of remainder register in add at {args.input}:{line_number}")
        instruction += parameters[1][1:]
        new_params = parameters[:-1]
    return [[instructions[instruction], *new_params]]

def handle_mv(parameters: list, line_number: int):
    instruction = "mv"
    new_params = []
    no_registers = False
    if parameters[0].startswith("%"):
        instruction = "st"
        instruction += parameters[0][1:]
        new_params = parameters[1:]
    if parameters[1].startswith("%"):
        if instruction == "ldr":
            instruction += parameters[1][1:]
            new_params = []
        else:
            instruction = "ld"
            instruction += parameters[1][1:]
            new_params = parameters[:-1]
    if instruction == "mv": # we haven't used any registers in the mv
        no_registers = True
    if not no_registers and new_params[0].startswith("v"):
        instruction = "v" + instruction
        new_params[0] = new_params[0][1:]
    return [[instructions[instruction], *new_params]] if not no_registers else [
        [instructions["spua"]],
        [instructions["lda"], parameters[0]],
        [instructions["sta"], parameters[1]],
        [instructions["spoa"]]
    ]

pseudoinstructions = {
    "add": handle_add,
    "mv": handle_mv
}

def handle_line(line: str, line_number: int):
    global current_label
    line = line.strip()
    if not line or line.startswith(";"):
        return
    if line.endswith(":"):
        current_label = line[:-1]
        if current_label in labels:
            raise omam8asm.exceptions.LabelAlreadyDefinedException(f"{current_label} is being redefined at {args.input}:{line_number}.")
        labels.update({current_label: []})
    else:
        instruction = line.split(" ")[0]
        parameters = [a.strip() for a in line[len(instruction):].split(",")]
        for i in range(len(parameters)):
            param = parameters[i]
            if param.startswith("$"):
                value = int(param[1:], 16 if param[1:].startswith("0x") else 10)
                if value not in data:
                    data.append(value)
                parameters[i] = "._data+" + str(data.index(value))
        output = []
        if instruction in pseudoinstructions:
            output = pseudoinstructions[instruction](parameters, line_number)
        else:
            output = [[instructions[instruction], *parameters]]
        labels[current_label] += output

def main():
    global args
    args = parser.parse_args()

    if not os.path.exists(args.input):
        raise FileNotFoundError(args.input)

    if os.path.exists(args.output):
        print(f"WARNING: {args.output} already exists, will overwrite!")
    
    with open(args.input) as file:
        lines = file.readlines()
    
    print("Processing...")
    
    current_line = 0
    for line in lines:
        handle_line(line, current_line)
        current_line += 1
    
    labels.update({"_data": data})
    print(labels)

    rom_data = []

    print("Building ROM...")
    for label in labels:
        print(label, "-", hex(get_label_address(label)))
        if label != "_data":
            for opcode in labels[label]:
                for value in opcode:
                    if isinstance(value, int):
                        rom_data.append(value)
                    else:
                        if value.startswith("."):
                            label = value[1:].split("+")[0]
                            value = value.replace(f".{label}", hex(get_label_address(label)))
                        rom_data += get_16bit_as_8bit(eval(value))
        else:
            rom_data += data
    print(rom_data)
    with open(args.output, "wb+") as file:
        file.write(bytes(rom_data))
