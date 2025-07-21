import argparse
import subprocess
import os
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

    print("Aseprite stdout:", result.stdout)
    print("Aseprite stderr:", result.stderr)

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

def main():
    parser = argparse.ArgumentParser(description="Export Aseprite frames and rotate PNGs 90° clockwise")
    parser.add_argument("--dithered", action="store_true", help="Export dithered version")
    args = parser.parse_args()

    export_folder = run_aseprite_export(dithered=args.dithered)
    rotate_images_90_clockwise(export_folder)

if __name__ == "__main__":
    main()
