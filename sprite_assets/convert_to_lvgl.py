from PIL import Image
import os
import sys
sys.stdout.reconfigure(encoding='utf-8')
sys.stderr.reconfigure(encoding='utf-8')

def convert_png_to_lvgl_c_array(png_path):
    img = Image.open(png_path).convert('P')
    width, height = img.size

    palette = img.getpalette()
    if palette is None:
        raise ValueError(f"{png_path} has no palette. Convert to indexed mode with 2 colors first.")

    num_colors = len(palette) // 3
    if num_colors < 2:
        raise ValueError(f"{png_path} palette has less than 2 colors. Need exactly 2 for 1-bit indexed.")

    transparency_index = img.info.get('transparency', None)

    lvgl_palette = []
    for i in range(2):
        r = palette[i * 3 + 0]
        g = palette[i * 3 + 1]
        b = palette[i * 3 + 2]
        a = 0 if transparency_index == i else 255
        lvgl_palette.extend([r, g, b, a])

    pixels = list(img.getdata())
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
        if bits_filled > 0:
            byte = byte << (8 - bits_filled)
            packed_pixels.append(byte)

    total_size = len(lvgl_palette) + len(packed_pixels)

    output_lines = []
    output_lines.append(f"/* Image size: {width} x {height} */")
    output_lines.append(f"/* Data size: {total_size} bytes (palette + pixel data) */")
    output_lines.append(f"static const uint8_t img_data[] = {{")

    for i in range(0, len(lvgl_palette), 4):
        output_lines.append(f"  0x{lvgl_palette[i]:02X}, 0x{lvgl_palette[i+1]:02X}, 0x{lvgl_palette[i+2]:02X}, 0x{lvgl_palette[i+3]:02X},")

    for i in range(0, len(packed_pixels), 12):
        line = ", ".join(f"0x{b:02X}" for b in packed_pixels[i:i+12])
        output_lines.append(f"  {line},")

    output_lines.append("};\n")
    output_lines.append("lv_img_dsc_t my_img = {")
    output_lines.append("  .header = {")
    output_lines.append("    .cf = LV_IMG_CF_INDEXED_1BIT,")
    output_lines.append("    .always_zero = 0,")
    output_lines.append("    .reserved = 0,")
    output_lines.append(f"    .w = {width},")
    output_lines.append(f"    .h = {height},")
    output_lines.append("  },")
    output_lines.append(f"  .data_size = {total_size},")
    output_lines.append("  .data = img_data,")
    output_lines.append("};")

    base_name = os.path.splitext(os.path.basename(png_path))[0]
    output_dir = os.path.dirname(png_path)
    output_filename = os.path.join(output_dir, f"{base_name}.c")

    with open(output_filename, "w") as f:
        f.write("\n".join(output_lines))

    print(f"OK - Converted {png_path} → {output_filename}")

def convert_all_pngs_in_directory(directory="."):
    for filename in os.listdir(directory):
        if filename.lower().endswith(".png"):
            png_path = os.path.join(directory, filename)
            try:
                convert_png_to_lvgl_c_array(png_path)
            except Exception as e:
                print(f"X Skipped {filename}: {e}")

if __name__ == "__main__":
    import sys
    target_dir = sys.argv[1] if len(sys.argv) > 1 else "."
    convert_all_pngs_in_directory(target_dir)
