from PIL import Image

im = Image.open("input.png")
im = im.convert("L")

itermediary = Image.new("L", (240, 160))

buffer = ""
previous_pixel = 0

for y in range(160):
    for x in range(240):
        pixel = im.getpixel((x, y))
        if previous_pixel != pixel:
            buffer += f"VMOV %255 {hex(y * 240 + x)}\n"
            itermediary.putpixel((x, y), 255)
            previous_pixel = pixel

with open("rle.oasm") as file:
    buffer += file.read()

with open("genimg.oasm", "w+") as file:
    file.write(buffer)

itermediary.save("intermediate.png")
