# GIF to ZMK Animation Converter

The [`gif-convert.py`](gif-convert.py) script converts GIF animations into LVGL-compatible C code for use with the ZMK nice_oled widget system. It handles image preprocessing, dithering, and automatic registration in the build system.

## Overview

This script performs the following operations:

1. **Preprocessing** (optional): Uses ImageMagick to coalesce GIF frames, convert to grayscale, and optionally crop
2. **Dithering**: Applies Atkinson dithering to convert grayscale images to 1-bit black and white
3. **Code Generation**: Creates `.c` and `.h` files containing LVGL image descriptors
4. **Registration**: Automatically registers the animation in the project build files

## Requirements

- Python 3.x
- Pillow (PIL) library: `pip install Pillow`
- ImageMagick (optional, for preprocessing): Install via your system package manager

## Usage

```bash
python3 gif-convert.py <input_gif> [options]
```

### Arguments

| Argument | Short | Type | Default | Description |
|----------|-------|------|---------|-------------|
| `input_gif` | - | positional | (required) | Path to the input GIF file |
| `--name` | `-n` | string | (from filename) | Base name for output files |
| `--output-dir` | `-o` | string | `boards/shields/nice_oled/assets` | Output directory for generated files |
| `--width` | `-W` | int | 64 | Target width in pixels |
| `--height` | `-H` | int | 64 | Target height in pixels |
| `--rotate` | `-r` | int | 90 | Rotation angle in degrees |
| `--no-rotate` | `-nr` | flag | - | Disable rotation |
| `--center` | - | flag | - | Set image gravity to center |
| `--crop` | `-c` | string | - | Crop geometry for ImageMagick |
| `--no-preprocess` | - | flag | - | Skip ImageMagick preprocessing |
| `--threshold` | `-t` | int | 128 | Dithering threshold (0-255) |
| `--animation-ms` | `-ms` | int | 5000 | Animation duration in milliseconds |
| `--no-register` | - | flag | - | Skip registering in project files |

### Examples

```bash
# Basic usage (name derived from filename)
python3 gif-convert.py /path/to/myanimation.gif

# Custom name and dimensions
python3 gif-convert.py /path/to/animation.gif -n custom_anim -W 48 -H 48

# With cropping (48x48 from top-left or with offset)
python3 gif-convert.py /path/to/animation.gif --crop 48x48
python3 gif-convert.py /path/to/animation.gif --crop 48x48+16+16

# Skip preprocessing and registration
python3 gif-convert.py /path/to/animation.gif --no-preprocess --no-register

# Custom animation duration
python3 gif-convert.py /path/to/animation.gif -ms 3000
```

## Output Files

The script generates two files in the output directory:

### `<name>.c`
Contains:
- Frame data as byte arrays (`<name>_00_map[]`, `<name>_01_map[]`, etc.)
- LVGL image descriptors for each frame
- Color palette configuration (supports inverted mode via `CONFIG_NICE_OLED_WIDGET_INVERTED`)

### `<name>.h`
Contains:
- `LV_IMG_DECLARE()` declarations for each frame
- Array of image descriptor pointers (`<name>_images[]`)
- Frame count macro (`<NAME>_IMAGES_NUM_IMAGES`)

## Automatic Registration

By default, the script registers the animation in three project files. It uses existing animation entries as insertion anchors.

### 1. [`animation.c`](boards/shields/nice_oled/widgets/animation.c)

Adds:
- `LV_IMG_DECLARE()` declarations for all frames
- Image array definition (`<name>_imgs[]`)
- `lv_animimg_set_src()` call in `draw_animation()` function

### 2. [`CMakeLists.txt`](boards/shields/nice_oled/CMakeLists.txt)

Adds:
```cmake
target_sources_ifdef(CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_<NAME> app PRIVATE assets/<name>.c)
```

### 3. [`Kconfig.defconfig`](boards/shields/nice_oled/Kconfig.defconfig)

Adds:
```kconfig
config NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_<NAME>
    bool "Enable <Name> animation on peripheral"
    default n
```

And adds the animation duration default:
```kconfig
default <animation-ms> if NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_<NAME>
```

## Troubleshooting

### ImageMagick Not Found
If you see "ImageMagick 'magick' command not found":
- Install ImageMagick: `sudo apt install imagemagick` (Debian/Ubuntu)
- Or use `--no-preprocess` to skip preprocessing

### Registration Fails
If registration cannot find insertion points:
- Ensure the default animation files exist at the expected paths
- Check that the animation entries are present (used as insertion anchors)
- Use `--no-register` and manually add the entries

### Large Frame Counts
For GIFs with many frames, the generated C file can become very large. Consider:
- Reducing frame count in the source GIF
- Using a smaller canvas size
- Skipping registration if not needed

## See Also

- [LVGL Image Documentation](https://docs.lvgl.io/latest/en/html/overview/image.html)
- [ZMK Display Documentation](https://zmk.dev/docs/features/displays)
- [ImageMagick Command-line Options](https://imagemagick.org/script/command-line-options.php)