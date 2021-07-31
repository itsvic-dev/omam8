import sys
from inspect import signature


# memory
mram = [0] * 0xFFFF
vram = [0] * 0x9600


# registers
program_counter = 0x8000
stack_pointer = 0x0000
remainder = 0


# instructions
def jmp(addr):
    global program_counter

    program_counter = addr
    return 1

def jnz(addr):
    global program_counter

    if remainder != 0:
        program_counter = addr
        return 1

def jz(addr):
    global program_counter

    if remainder == 0:
        program_counter = addr
        return 1

def add(x, y):
    global remainder

    if y > 0x7FFF:
        print("WARNING: ADD tried to write to ROM; not executing")
        return
    mram[y] = mram[x] + mram[y]
    if (mram[y] > 255):
        remainder = mram[y] >> 0x8 & 0xff
        mram[y] = mram[y] & 0xff

def sub(x, y):
    if y > 0x7FFF:
        print("WARNING: SUB tried to write to ROM; not executing")
        return
    mram[y] = mram[y] - mram[x]

def mov(x, y):
    if y > 0x7FFF:
        print("WARNING: MOV", hex(x), hex(y), "tried to write to ROM; not executing")
        return
    mram[y] = mram[x]

def vmov(x, y):
    if y >= len(vram):
        print("WARNING: VMOV tried to write beyond VRAM; halting")
        hlt()
    vram[y] = mram[x]

def mul(x, y):
    if y > 0x7FFF:
        print("WARNING: MUL tried to write to ROM; not executing")
        return
    mram[y] = mram[x] * mram[y]

def div(x, y):
    if y > 0x7FFF:
        print("WARNING: DIV tried to write to ROM; not executing")
        return
    mram[y] = mram[y] / mram[x]

def spu(x):
    global stack_pointer
    mram[stack_pointer] = mram[x]
    stack_pointer += 1

def spo(x):
    global stack_pointer
    stack_pointer -= 1
    mram[x] = mram[stack_pointer]

def clr():
    global remainder
    remainder = 0

def _and(x, y):
    mram[y] = mram[x] & mram[y]

def vrd(x, y):
    if x >= len(vram):
        print("WARNING: VRD tried to read beyond VRAM; halting")
        hlt()
    mram[y] = vram[x]

def hlt():
    print("CPU halted.")
    print(" ----- Registers -----")
    print("program counter:", hex(program_counter))
    print("remainder:", hex(remainder))
    print("stack pointer:", hex(stack_pointer))
    print(" ----- dumping RAM -----")
    with open("mram.bin", "wb+") as file:
        file.write(bytes(mram))
    with open("vram.bin", "wb+") as file:
        file.write(bytes(vram))
    sys.exit(0)


instruction_map = {
    0x00: jmp,
    0x01: jnz,
    0x02: jz,
    0x03: add,
    0x04: sub,
    0x05: mov,
    0x06: vmov,
    0x07: mul,
    0x08: div,
    0x09: spu,
    0x0A: spo,
    0x0B: clr,
    0x0C: _and,
    0x0D: vrd,
    0xFF: hlt
}


# internal stuff
def loadROM():
    print("Loading ROM...")
    with open("rom.bin", "rb") as file:
        data = file.read()
        for i in range(len(data)):
            mram[0x8000 + i] = data[i]
    print("Loaded ROM.")

def execute():
    global program_counter

    # FETCH/DECODE
    instruction = mram[program_counter]
    if instruction not in instruction_map.keys():
        print("WARNING: unknown instruction", hex(instruction), "- halting")
        hlt()

    args = []
    func = instruction_map[instruction]
    sig = signature(func)
    for i in range(len(sig.parameters)):
        j = i * 2 + 1
        args.append(mram[program_counter + j + 1] * 0x100 + mram[program_counter + j])
    
    # EXECUTE
    output = func(*args)
    if output != 1:
        program_counter = program_counter + (len(sig.parameters) * 2) + 1


if __name__ == "__main__":
    print("Initializing emulator...")
    loadROM()
    print("Starting loop...")
    while True:
        execute()
