from PIL import Image

im = Image.new("L", (240, 160))

with open("vram.bin", "rb") as file:
    data = file.read()
    for y in range(160):
        for x in range(240):
            im.putpixel((x, y), data[y * 240 + x])

im.save("vram.png")
