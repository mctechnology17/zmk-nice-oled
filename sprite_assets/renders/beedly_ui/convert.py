from PIL import Image

def convert_png_to_lvgl_c_array(png_path):
    img = Image.open(png_path).convert('P')
    width, height = img.size

    # Get palette (flat list [R,G,B,R,G,B,...])
    palette = img.getpalette()
    if palette is None:
        raise ValueError("Image has no palette. Convert to indexed mode with 2 colors first.")

    # We only want first 2 colors (indexed 1-bit)
    # Check image has at least 2 palette colors
    num_colors = len(palette) // 3
    if num_colors < 2:
        raise ValueError("Palette has less than 2 colors, need exactly 2 for 1-bit indexed.")

    # Transparency index (if any)
    transparency_index = img.info.get('transparency', None)

    # Build LVGL palette: 2 colors × 4 bytes (RGBA)
    lvgl_palette = []
    for i in range(2):
        r = palette[i * 3 + 0]
        g = palette[i * 3 + 1]
        b = palette[i * 3 + 2]
        a = 0 if transparency_index == i else 255
        lvgl_palette.extend([r, g, b, a])

    pixels = list(img.getdata())

    # Pack pixels row by row, pad each row to full bytes
    row_bytes = (width + 7) // 8
    packed_pixels = []

    for row in range(height):
        byte = 0
        bits_filled = 0
        for col in range(width):
            px = pixels[row * width + col]
            bit = 1 if px else 0
            byte = (byte << 1) | bit
            bits_filled += 1
            if bits_filled == 8:
                packed_pixels.append(byte)
                byte = 0
                bits_filled = 0
        # Pad leftover bits in this row
        if bits_filled > 0:
            byte = byte << (8 - bits_filled)
            packed_pixels.append(byte)

    total_size = len(lvgl_palette) + len(packed_pixels)

    # Print C array output
    print(f"/* Image size: {width} x {height} */")
    print(f"/* Data size: {total_size} bytes (palette + pixel data) */")
    print(f"static const uint8_t img_data[] = {{")

    # Palette bytes
    for i in range(0, len(lvgl_palette), 4):
        print(f"  0x{lvgl_palette[i]:02X}, 0x{lvgl_palette[i+1]:02X}, 0x{lvgl_palette[i+2]:02X}, 0x{lvgl_palette[i+3]:02X},")

    # Pixel data bytes (grouped 12 per line)
    for i in range(0, len(packed_pixels), 12):
        line = ", ".join(f"0x{b:02X}" for b in packed_pixels[i:i+12])
        print(f"  {line},")

    print("};\n")

    # Print lv_img_dsc_t struct
    print("lv_img_dsc_t my_img = {")
    print("  .header = {")
    print("    .cf = LV_IMG_CF_INDEXED_1BIT,")
    print("    .always_zero = 0,")
    print("    .reserved = 0,")
    print(f"    .w = {width},")
    print(f"    .h = {height},")
    print("  },")
    print(f"  .data_size = {total_size},")
    print("  .data = img_data,")
    print("};")

if __name__ == "__main__":
    import sys
    if len(sys.argv) < 2:
        print("Usage: python3 convert_lvgl.py your_image.png")
    else:
        convert_png_to_lvgl_c_array(sys.argv[1])
