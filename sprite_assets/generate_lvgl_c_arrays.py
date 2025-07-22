import argparse
import subprocess
import os
from pathlib import Path
from PIL import Image

ASEPRITE_PATH = r"C:\Program Files\Aseprite\Aseprite.exe"
SPRITE_PATHS = "sprite_assets/ase/"
EXPORT_BASE_PATH = "sprite_assets/renders/"

def run_aseprite_export(dithered=False):
    folder_name = "beedly_ui_dithered" if dithered else "beedly_ui"
    export_folder = os.path.abspath(os.path.join(EXPORT_BASE_PATH, folder_name))
    os.makedirs(export_folder, exist_ok=True)

    ase_file = folder_name + ".aseprite"
    ase_file_path = os.path.abspath(os.path.join(SPRITE_PATHS, ase_file))

    print(f"Exporting frames from '{ase_file_path}' to '{export_folder}' (dithered={dithered})...")

    result = subprocess.run([
        ASEPRITE_PATH,
        "-b",
        ase_file_path,
        "--save-as",
        os.path.join(export_folder, "frame_{frame}.png"),  # export frames individually
    ], capture_output=True, text=True)
    
    result.check_returncode()
    print("Aseprite export complete.")
    return export_folder


def rotate_images_90_clockwise(folder_path):
    print(f"Rotating PNG images 90° clockwise in folder '{folder_path}'...")
    for filename in os.listdir(folder_path):
        if filename.lower().endswith(".png"):
            path = os.path.join(folder_path, filename)
            with Image.open(path) as img:
                rotated = img.rotate(-90, expand=True)  # -90 degrees = 90 clockwise
                rotated.save(path)
            print(f"Rotated {filename}")


def convert_image_to_lvgl_c_array(img: Image.Image, array_name: str) -> str:
    if img.mode != '1':
        img = img.convert('1')

    width, height = img.size
    pixels = img.tobytes()

    def reverse_bits(byte):
        b = byte
        b = (b & 0xF0) >> 4 | (b & 0x0F) << 4
        b = (b & 0xCC) >> 2 | (b & 0x33) << 2
        b = (b & 0xAA) >> 1 | (b & 0x55) << 1
        return b

    reversed_bytes = bytes(reverse_bits(b) for b in pixels)

    lines = []
    line_len = 12
    for i in range(0, len(reversed_bytes), line_len):
        chunk = reversed_bytes[i:i+line_len]
        line = ", ".join(f"0x{b:02X}" for b in chunk)
        lines.append(f"    {line}")

    c_array = f"const uint8_t {array_name}[] = {{\n" + ",\n".join(lines) + "\n};\n"
    return c_array


from pathlib import Path
from PIL import Image

def generate_lvgl_c_array(dithered=False):
    base_path = Path(EXPORT_BASE_PATH)
    input_dir = base_path / ("beedly_ui_dithered" if dithered else "beedly_ui")

    if not input_dir.exists():
        raise RuntimeError(f"Input folder does not exist: {input_dir}")

    png_files = sorted([f for f in input_dir.glob("*.png") if f.is_file()])
    if not png_files:
        raise RuntimeError(f"No PNGs found in {input_dir}")

    print(f"Found {len(png_files)} PNGs in {input_dir}")

    # Output file is saved in the same folder as the PNGs
    output_file = input_dir / ("beedly_ui_dithered.c" if dithered else "beedly_ui.c")

    with open(output_file, "w") as f_out:
        f_out.write("// Generated LVGL indexed 1-bit images\n\n")
        for idx, png_path in enumerate(png_files):
            img = Image.open(png_path)
            if img.mode not in ['1', 'P']:
                img = img.convert('1')

            base_name = png_path.stem.replace('-', '_').replace(' ', '_')
            array_name = f"{base_name}_frame_{idx}"

            c_array_str = convert_image_to_lvgl_c_array(img, array_name)
            f_out.write(c_array_str + "\n")

    print(f"Exported LVGL arrays to {output_file}")

def main():
    parser = argparse.ArgumentParser(description="Export Aseprite sprites to LVGL C arrays.")
    parser.add_argument("--dithered", action="store_true", help="Export dithered sprite frames")

    args = parser.parse_args()

    # Step 1: Export frames with Aseprite
    export_folder = run_aseprite_export(dithered=args.dithered)

    # Step 2: Rotate the exported frames
    rotate_images_90_clockwise(export_folder)

    # Step 3: Generate LVGL C arrays
    print("Calling LVGL conversion script...")

    script_path = os.path.join(os.path.dirname(__file__), "convert_to_lvgl.py")
    result = subprocess.run(
        ["python", script_path, export_folder],
        capture_output=True,
        text=True
    )
    result.check_returncode()


if __name__ == "__main__":
    main()
