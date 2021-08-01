import sys

input_file = sys.argv[1]

with open(input_file) as file:
    data = file.readlines()

str_to_instruction = {
    "JMP": 0x00,
    "JNZ": 0x01,
    "JZ": 0x02,
    "ADD": 0x03,
    "SUB": 0x04,
    "MOV": 0x05,
    "VMOV": 0x06,
    "MUL": 0x07,
    "DIV": 0x08,
    "SPU": 0x09,
    "SPO": 0x0A,
    "CLR": 0x0B,
    "AND": 0x0C,
    "VRD": 0x0D,
    "LDA": 0x0E,
    "STA": 0x0F,
    "JEQ": 0x10,
    "HLT": 0xFF
}

buffer = []
labels = {"root": []}
current_label = "root"

rom_start = 0x8000

def get_opcode_size(opcode):
    return 0x01 + (0x02 * len(opcode[1:]))

def get_label_size(label):
    size = 0
    for opcode in labels[label]:
        size += get_opcode_size(opcode)
    return size

known_offsets = {}

def get_label_offset(label):
    if label in known_offsets.keys():
        return known_offsets[label]
    index = list(labels.keys()).index(label)
    offset = 0
    for i in range(index):
        offset += get_label_size(list(labels.keys())[i])
    value = rom_start + offset
    known_offsets[label] = value
    return value


print("Parsing labels...")
for line in data:
    opcode = line.split(";")[0].strip().split(" ")
    instruction = opcode.pop(0)
    if len(instruction) == 0:
        continue
    if instruction.endswith(":"):
        current_label = instruction[:-1]
        if current_label not in labels.keys():
            labels[current_label] = []
        continue
    labels[current_label].append([instruction, *opcode])

print("Building ROM...")

def replace_label_with_offset(arg):
    for label in labels.keys():
        arg = arg.replace(f".{label}", str(get_label_offset(label)))
    return arg

for line in data:
    opcode = line.split(";")[0].strip().split(" ")
    instruction = opcode.pop(0)
    if len(instruction) == 0:
        continue
    if instruction.endswith(":"):
        current_label = instruction[:-1]
        if current_label not in labels.keys():
            labels[current_label] = []
        continue
    if instruction not in str_to_instruction.keys():
        print("WARNING: Unknown instruction", instruction)
        continue
    buffer.append(str_to_instruction[instruction])
    for arg in opcode:
        if arg.startswith("%"):
            if "__data" not in labels.keys():
                labels["__data"] = []
            data = eval(arg[1:])
            if data not in labels["__data"]:
                labels["__data"].append(data)
            index = labels["__data"].index(data)
            arg = f".__data+{index}"
        fullbyte = eval(replace_label_with_offset(arg))
        upper = fullbyte >> 0x8 & 0xff
        lower = fullbyte & 0xff

        buffer.append(lower)
        buffer.append(upper)

if "__data" in labels.keys():
    for data in labels["__data"]:
        buffer.append(data)

with open("rom.bin", "wb+") as file:
    file.write(bytes(buffer))

print("Done.")
