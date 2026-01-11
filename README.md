# nice!oled - nice!epaper - nice!custom
![nice_oled_demo](./assets/nice_oled_demo.GIF)
![nice_epaper_demo](./assets/nice_epaper_demo.GIF)
![nice_epaper_demo](./assets/nice_custom_demo.GIF)

## Table of Contents

<a name="top"></a>

- [Overview](#overview)
- [Gallery](#gallery)
- [Quick Installation](#quick-installation)
- [Configuration](#configuration)
- [ZMK RAW HID Implementation](#zmk-raw-hid-implementation)
- [Suggestions](#suggestions)
- [Limitations](#limitations)
- [Acknowledgments & Credits](#acknowledgments)
- [Support](#if-you-like-my-contributions-please-dont-forget-the-following)
- [My other projects](#my-other-projects)

# Overview
> [!WARNING]
> TESTED USING ZMK v0.3.0 please see [Pin your ZMK version](https://zmk.dev/blog/2025/06/20/pinned-zmk)

Vertical widgets for [oled] and [niceview] screens using zmk (for split and non-split keyboards)


## System Architecture & Implementation Summary

## Modular & Submodular Design Strategy
This ZMK module implements a highly decoupled architecture designed for maximum flexibility and extensibility.

*   **Modular Widgets**: The view layer is fully modular, permitting the runtime selection and composition of any available widget without heavy coupling to the core display logic.
*   **Submodular RAW HID**: The RAW HID implementation is granular. Submodules such as Temperature, Time, Layout, and Volume can be independently compiled and activated via Kconfig, minimizing flash usage and processing overhead by excluding unused logic.

## Abstraction Layer: `nice_custom` Shield
A dedicated hardware abstraction layer, the `nice_custom` shield, was developed to decouple driver positioning from physical hardware definitions.
*   **Independent Driver Coordinate System**: All X/Y rendering coordinates are configurable via the configuration system (runtime/Kconfig) completely independent of the underlying shield definition.
*   **Code-Free Customization**: Users can reposition drivers and elements to suit non-standard layouts or personal preferences purely through configuration variables, eliminating the need to fork or modify the C source code of the display driver.

## Host-Side Integration (Rust)

The companion host application, `zmk-hid-host`, provides the necessary data bridge for universal Raw HID features (System Time, Volume, Layout) across Linux, Windows, and macOS.

*   **Technical Contributions (macOS Enhancements)**:
    *   **macOS Temperature Support**: Implemented platform-specific temperature monitoring for macOS within the Rust codebase.
    *   **Spotify Integration**: Engineered a native AppleScript/Rust bridge to fetch real-time track metadata from Spotify for display on the peripheral (macOS only).
    *   **Cross-Architecture Support**: Developed a universal macOS application wrapper, bundling native binaries for both Intel (`x86_64`) and Apple Silicon (`arm64`) architectures, ensuring seamless deployment across the Apple ecosystem.

## Shields supported:

- `nice_oled`
- `nice_epaper`
- `nice_custom`

## General characteristics:

### General
- **Vertical Layout**: Optimized for split keyboard OLED/e-Ink/ePaper screens.
- **Compact Design**: Maximizes information density.
- **Modular Widgets**: Enable/disable components independently.
- **`nice_custom` Shield**: Position drivers via config, no code required.
- **Neck Strain Reduction**: View info on keyboard instead of monitor.
- **Dark Mode Friendly**: Optimized for nice!oled.

### Central Screen
- **WPM Widgets**: Graphs, Speedometer, Luna, Bongo Cat.
- **HID Indicators**: CapsLock status with Luna/Bongo Cat animations.
- **Modifiers Widget**: Real-time display of Ctrl, Shift, Alt, Gui.
- **Raw HID Integration**:
    - **Spotify (macOS)**: Real-time track information.
    - **Weather**: Current temperature display.
    - **System Sync**: Volume, Time, and Keyboard Layout.

### Peripheral Screen
- **Vertical Animations**: Gem, Cat, Head, Pokemon, Spaceman.
- **Smart Battery**: Efficient battery status animation.
- **Sleep Art**: Custom images during idle/sleep.
- **Static Images**: Vim, Custom Logos (low power mode).

# Gallery

### [Click here for more photos or go to the gallery section photos](./fotos)

<div align="center">

  <img align="center" alt="demo3" width="400px" src="./assets/demo3.jpg" />
  <img align="center" alt="demo2" width="400px" src="./assets/demo2.jpg" />
  <img align="center" alt="demo1" width="400px" src="./assets/demo1.jpg" />
  <img align="center" alt="demo4" width="400px" src="./assets/demo4.jpg" />
  <img align="center" alt="Animation Preview" width="400px" src="./assets/animation.gif" />
  <img align="center" alt="preview1" width="400px" src="./assets/preview1.JPG" />

</div>

# Quick Installation
> [!WARNING]
>
> TESTED USING ZMK v0.3.0 please see [Pin your ZMK version](https://zmk.dev/blog/2025/06/20/pinned-zmk)

> [!TIP]
>
> All widgets are enabled `hid`,` modifiers`, `wpm` etc., if you want to
> disable them, look at the configuration section

Installation in 4 simple steps:

0. Configuration example? You should see this `->` [zmk-config]

1. Make sure to enable the custom status screen in your `*.conf` file:

```conf
CONFIG_ZMK_DISPLAY=y
CONFIG_ZMK_DISPLAY_STATUS_SCREEN_CUSTOM=y
```

2. Copy and paste this into your `config/west.yml` file:

```yaml
manifest:
  remotes:
    - name: zmkfirmware
      url-base: https://github.com/zmkfirmware
    # support nice_oled and nice_epaper
    - name: mctechnology17
      url-base: https://github.com/mctechnology17
  projects:
    - name: zmk
      remote: zmkfirmware
      revision: v0.3.0
      import: app/west.yml
    # support nice_oled and nice_epaper
    - name: zmk-nice-oled
      remote: mctechnology17
      revision: main
  self:
    path: config
```

3. In the `build.yaml` file, add the `nice_oled` shield (using the corne as an
   example, but it could be another shield).

For [oled] screen:
```yaml
---
include:
  - board: nice_nano_v2
    shield: corne_left nice_oled
  - board: nice_nano_v2
    shield: corne_right nice_oled
```

For [niceview] screen:
```yaml
---
include:
  - board: nice_nano_v2
    shield: corne_left nice_view_adapter nice_epaper
  - board: nice_nano_v2
    shield: corne_right nice_view_adapter nice_epaper
```

4. Build the firmware, flash it to your keyboard, and enjoy!

# Configuration
> [!IMPORTANT]
> Make sure to enable the custom status screen in your ZMK configuration:

Make sure to enable the custom status screen in your ZMK configuration:

```conf
CONFIG_ZMK_DISPLAY=y
CONFIG_ZMK_DISPLAY_STATUS_SCREEN_CUSTOM=y
```

Anatomy of the variables of this repository

This document lists the available configuration options for the `nice_oled` shield. These variables can be set in your `.conf` file to customize the behavior of the display widgets.

## General Configuration

| Variable | Type | Default | Description |
|----------|------|---------|-------------|
| `CONFIG_NICE_OLED_ON` | bool | `y` | Enable nice oled widget compatibility |
| `CONFIG_NICE_EPAPER_ON` | bool | `n` | Enable nice view widget compatibility |
| `CONFIG_LV_Z_VDB_SIZE` | int | `64` (OLED), `100` (ePaper) | Buffer size for LVGL |
| `CONFIG_LV_DPI_DEF` | int | `148` (OLED), `161` (ePaper) | Dots per inch (DPI) setting |
| `CONFIG_LV_Z_BITS_PER_PIXEL` | int | `1` | Bits per pixel |
| `CONFIG_LV_COLOR_DEPTH` | choice | `LV_COLOR_DEPTH_1` | Color depth |
| `CONFIG_ZMK_DISPLAY_WORK_QUEUE` | choice | `ZMK_DISPLAY_WORK_QUEUE_DEDICATED` | Work queue type |
| `CONFIG_ZMK_DISPLAY_STATUS_SCREEN` | choice | `ZMK_DISPLAY_STATUS_SCREEN_CUSTOM` | Status screen type |
| `CONFIG_ZMK_DISPLAY_BLANK_ON_IDLE` | bool | `n` (OLED), `y` (ePaper) | Control display blanking on idle for nice_custom/nice_epaper |
| `CONFIG_LV_Z_MEM_POOL_SIZE` | int | `8192` | Memory pool size |
| `CONFIG_ZMK_DISPLAY_DEDICATED_THREAD_STACK_SIZE` | int | `2560` (`3072` with RAW HID) | Display thread stack size |
| `CONFIG_ZMK_DISPLAY_DEDICATED_THREAD_PRIORITY` | int | `5` | Display thread priority (lower = higher priority) |
| `CONFIG_NICE_OLED_WIDGET_STATUS` | bool | - | Custom nice oled status widget (Selects LVGL features) |
| `CONFIG_NICE_OLED_WIDGET_INVERTED` | bool | `n` | Invert display colors |
| `CONFIG_NICE_OLED_WIDGET_OUTPUT_BACKGROUND` | bool | `n` (OLED), `y` (ePaper) | Enable output background on central and peripheral |
| `CONFIG_NICE_OLED_CUSTOM_CANVAS_WIDTH` | int | `68` (ePaper), `32` (OLED) | Custom Canvas Width overwriting default calculation |
| `CONFIG_NICE_OLED_CUSTOM_CANVAS_HEIGHT` | int | `160` (ePaper), `128` (OLED) | Custom Canvas Height overwriting default calculation |

## Sleep & Animation (Central & Peripheral)

| Variable | Type | Default | Description |
|----------|------|---------|-------------|
| `CONFIG_NICE_OLED_SHOW_SLEEP_ART_ON_IDLE` | bool | `n` | Whether the sleep art should be shown on idle |
| `CONFIG_NICE_OLED_SHOW_SLEEP_ART_ON_SLEEP` | bool | `n` | Whether the sleep art should be shown on sleep |

## Peripheral Specific

| Variable | Type | Default | Description |
|----------|------|---------|-------------|
| `CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_SMART_BATTERY` | bool | `n` | Enable smart battery animation on peripheral |
| `CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_WPM` | bool | `n` | Enable PERIPHERAL WPM widget compatibility |
| `CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL` | bool | `y` | Enable animation on peripheral |
| `CONFIG_NICE_OLED_WIDGET_STATIC_IMAGE_PERIPHERAL` | bool | `y` (if no anim) | Enable static image on peripheral |
| `CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_GEM` | bool | `n` | Enable gem animation on peripheral |
| `CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_CAT` | bool | `y` | Enable cat animation on peripheral |
| `CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_HEAD` | bool | `n` | Enable head animation on peripheral |
| `CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_POKEMON` | bool | `n` | Enable pokemon animation on peripheral |
| `CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_SPACEMAN` | bool | `n` | Enable spaceman animation on peripheral |
| `CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_MS` | int | varies | Animation length in milliseconds |
| `CONFIG_NICE_OLED_WIDGET_STATIC_IMAGE_PERIPHERAL_VIM` | bool | `n` | Enable static vim on peripheral |
| `CONFIG_NICE_OLED_WIDGET_STATIC_IMAGE_PERIPHERAL_VIP_MARCOS` | bool | `n` | Enable static vim_marcos on peripheral |
| `CONFIG_NICE_OLED_WIDGET_PERIPHERAL` | bool | `n` | Enable test widget on peripheral |

## Central Specific

| Variable | Type | Default | Description |
|----------|------|---------|-------------|
| `CONFIG_NICE_OLED_WIDGET_WPM` | bool | `y` | Enable Nice OLED WPM Widget |
| `CONFIG_NICE_OLED_WIDGET_WPM_NUMBER` | bool | `y` | Enable NUMBER for WPM |
| `CONFIG_NICE_OLED_WIDGET_WPM_SPEEDOMETER` | bool | `y` (OLED) | Enable SPEEDOMETER for WPM |
| `CONFIG_NICE_OLED_WIDGET_WPM_GRAPH` | bool | `n` (OLED) | Enable GRAPH for WPM gauge/chart |
| `CONFIG_NICE_OLED_WIDGET_WPM_GRAPH_FIXED_RANGE` | bool | `y` | Enable fixed range for WPM gauge/chart |
| `CONFIG_NICE_OLED_WIDGET_WPM_GRAPH_FIXED_RANGE_MAX` | int | `100` | Fixed range maximum for WPM gauge/chart |
| `CONFIG_NICE_OLED_WIDGET_WPM_LUNA` | bool | `y` | Enable wpm luna |
| `CONFIG_NICE_OLED_WIDGET_WPM_LUNA_ANIMATION_MS` | int | `300` | Luna Animation in ms for WPM Widget |
| `CONFIG_NICE_OLED_WIDGET_WPM_BONGO_CAT` | bool | `n` | Enable wpm bongo cat |
| `CONFIG_NICE_OLED_WIDGET_WPM_BONGO_CAT_ANIMATION_MS` | int | `300` | Bongo Cat Animation in ms for WPM Widget |
| `CONFIG_NICE_OLED_WIDGET_RESPONSIVE` | bool | `n` | Enable widget responsive (overrides stack to `4096`, priority to `3`) |
| `CONFIG_NICE_OLED_WIDGET_RESPONSIVE_BONGO_CAT` | bool | `n` | Enable widget responsive bongo cat |

## HID Indicators

| Variable                                                         | Type | Default | Description                                       |
|------------------------------------------------------------------|------|---------|---------------------------------------------------|
| `CONFIG_NICE_OLED_WIDGET_HID_INDICATORS`                         | bool | `y`     | Enable HID indicators                             |
| `CONFIG_ZMK_HID_INDICATORS`                                      | bool | `y`     | Enable HID indicators (Core ZMK)                  |
| `CONFIG_NICE_OLED_WIDGET_HID_INDICATORS_LUNA`                    | bool | `n`     | Enable HID indicators luna                        |
| `CONFIG_NICE_OLED_WIDGET_HID_INDICATORS_LUNA_ONLY_CAPSLOCK`      | bool | `y`     | Enable HID indicators luna only on CapsLock       |
| `CONFIG_NICE_OLED_WIDGET_HID_INDICATORS_LUNA_ANIMATION_MS`       | int  | `300`   | Luna Animation in ms for HID Indicators           |
| `CONFIG_NICE_OLED_WIDGET_HID_INDICATORS_BONGO_CAT`               | bool | `n`     | Enable HID indicators bongo cat                   |
| `CONFIG_NICE_OLED_WIDGET_HID_INDICATORS_BONGO_CAT_ONLY_CAPSLOCK` | bool | `y`     | Enable HID indicators bongo cat only on CapsLock  |
| `CONFIG_NICE_OLED_WIDGET_HID_INDICATORS_BONGO_CAT_ANIMATION_MS`  | int  | `300`   | Luna Animation in ms for HID Indicators bongo cat |

## Raw HID

| Variable | Type | Default | Description |
|----------|------|---------|-------------|
| `CONFIG_NICE_OLED_WIDGET_RAW_HID` | bool | - | Enable RAW HID |
| `CONFIG_NICE_OLED_WIDGET_RAW_HID_DRIVER` | bool | `y` | Enable Raw HID |
| `CONFIG_USB_HID_DEVICE_COUNT` | int | `2` | Enable CONFIG USB HID DEVICE COUNT |
| `CONFIG_NICE_OLED_WIDGET_RAW_HID_TIME` | bool | `y` | Show time |
| `CONFIG_NICE_OLED_WIDGET_RAW_HID_VOLUME` | bool | `y` | Show volume |
| `CONFIG_NICE_OLED_WIDGET_RAW_HID_USAGE_PAGE` | hex | `0xFF60` | Raw HID Usage Page |
| `CONFIG_NICE_OLED_WIDGET_RAW_HID_USAGE` | hex | `0x61` | Raw HID Usage |
| `CONFIG_NICE_OLED_WIDGET_RAW_HID_REPORT_SIZE` | int | `32` | Raw HID Report Size |
| `CONFIG_NICE_OLED_WIDGET_RAW_HID_DEVICE` | string | `HID_1` | Raw HID Device |
| `CONFIG_NICE_OLED_WIDGET_RAW_HID_LAYOUT` | bool | `y` | Show layout |
| `CONFIG_NICE_OLED_WIDGET_RAW_HID_LAYOUT_LIST` | string | `"DE,US,LA,AB"` | Comma-separated layout list |
| `CONFIG_NICE_OLED_WIDGET_RAW_HID_WEATHER` | bool | `n` | nice!OLED Weather Widget |
| `CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_SPOTIFY_MACOS` | bool | `n` | nice!OLED Spotify MacOS Media Player Widget |

## Modifiers Indicators

| Variable | Type | Default | Description |
|----------|------|---------|-------------|
| `CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS` | bool | `y` | Enable modifiers indicators |
| `CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED` | bool | `y` | Enable modifiers indicators fixed |
| `CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED_LETTER` | bool | `n` | Style: Letters (Text C/S/A/G) |
| `CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED_SYMBOL` | bool | `y` | Style: Real icons (Default) |
| `CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED_HOR` | bool | `n` | Layout: Horizontal |
| `CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED_VER` | bool | `n` | Layout: Vertical |
| `CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED_BOX` | bool | `y` | Layout: Box/Grid (Default) |
| `CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED_VER_ALIGN_LEFT` | bool | `n` | Vertical Align: Left |
| `CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED_VER_ALIGN_CENTER` | bool | `y` | Vertical Align: Center (Default) |
| `CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED_VER_ALIGN_RIGHT` | bool | `n` | Vertical Align: Right |
| `CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED_SYMBOL_MACOS` | bool | `y` | OS Style: macOS (Default) |
| `CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED_SYMBOL_WINDOWS` | bool | `n` | OS Style: Windows |
| `CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_LUNA` | bool | `n` | Enable modifiers indicators luna |
| `CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_LUNA_ANIMATION_MS` | int | `300` | Luna Animation in ms for Modifiers Indicators |
| `CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_BONGO_CAT` | bool | `n` | Enable Bongo Cat animation for Modifiers Indicators |
| `CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_BONGO_CAT_ANIMATION_MS` | int | `300` | Animation speed (ms) for Modifiers Bongo Cat |

### Symbol Icons

When using `SYMBOL` style, the following real icons are displayed:

**macOS mode (default):**
- Control: `⌃` (caret symbol)
- Shift: `⇧` (upward arrow)
- Option: `⌥` (option key symbol)
- Command: `⌘` (command/clover symbol)

**Windows mode:**
- Control: `⌃` (caret symbol)
- Shift: `⇧` (upward arrow)
- Alt: `Alt` text icon
- Win: Windows logo

## Other Widgets (Central)

| Variable | Type | Default | Description |
|----------|------|---------|-------------|
| `CONFIG_NICE_OLED_WIDGET_LAYER` | bool | `y` | Enable layer widget |
| `CONFIG_NICE_OLED_WIDGET_PROFILE_BIG` | bool | `n` | Enable show widget profile bigger |
| `CONFIG_NICE_OLED_WIDGET_CENTRAL` | bool | `n` | Enable test widget on central |

## Battery Widget (Experimental)

| Variable | Type | Default | Description |
|----------|------|---------|-------------|
| `CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_ALL` | bool | `n` | View all available batteries on the central screen, including the central battery |
| `CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_ONLY` | bool | `n` | View all peripheral batteries, but not the central battery |
| `CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_AND_CENTRAL` | bool | `n` | View only 1 peripheral batterie and the central battery |

## Widget Position Coordinates

These variables allow users to customize widget positions. Each widget has X and Y coordinates that can be set in your `.conf` file. The defaults are set per shield (OLED vs ePaper).

### Layer Widget Position

| Variable | Type | Default OLED | Default ePaper | Description |
|----------|------|--------------|----------------|-------------|
| `CONFIG_NICE_OLED_WIDGET_LAYER_CUSTOM_X` | int | `0` | `0` | Layer widget X position |
| `CONFIG_NICE_OLED_WIDGET_LAYER_CUSTOM_Y` | int | `146` | `146` | Layer widget Y position |

### Battery Widget Position

| Variable | Type | Default OLED | Default ePaper | Description |
|----------|------|--------------|----------------|-------------|
| `CONFIG_NICE_OLED_WIDGET_BATTERY_CUSTOM_X` | int | `0` | `26` | Battery widget X position |
| `CONFIG_NICE_OLED_WIDGET_BATTERY_CUSTOM_Y` | int | `50` | `19` | Battery widget Y position |

### Profile Widget Position

| Variable | Type | Default OLED | Default ePaper | Description |
|----------|------|--------------|----------------|-------------|
| `CONFIG_NICE_OLED_WIDGET_PROFILE_CUSTOM_X` | int | `0` | `18` | Profile widget X position |
| `CONFIG_NICE_OLED_WIDGET_PROFILE_CUSTOM_Y` | int | `137` | `129` | Profile widget Y position |
| `CONFIG_NICE_OLED_WIDGET_PROFILE_TEXT_CUSTOM_X` | int | `25` | `25` | Profile text X position |
| `CONFIG_NICE_OLED_WIDGET_PROFILE_TEXT_CUSTOM_Y` | int | `32` | `32` | Profile text Y position |

### Output Widget Position

| Variable | Type | Default OLED | Default ePaper | Description |
|----------|------|--------------|----------------|-------------|
| `CONFIG_NICE_OLED_WIDGET_OUTPUT_USB_CUSTOM_X` | int | `0` | `45` | Output USB icon X position |
| `CONFIG_NICE_OLED_WIDGET_OUTPUT_USB_CUSTOM_Y` | int | `34` | `2` | Output USB icon Y position |
| `CONFIG_NICE_OLED_WIDGET_OUTPUT_BT_CUSTOM_X` | int | `4` | `49` | Output BT icon X position |
| `CONFIG_NICE_OLED_WIDGET_OUTPUT_BT_CUSTOM_Y` | int | `32` | `0` | Output BT icon Y position |
| `CONFIG_NICE_OLED_WIDGET_OUTPUT_BT_UNBONDED_CUSTOM_X` | int | `-1` | `44` | Output BT unbonded icon X position |
| `CONFIG_NICE_OLED_WIDGET_OUTPUT_BT_UNBONDED_CUSTOM_Y` | int | `32` | `0` | Output BT unbonded icon Y position |

### WPM Widget Position

| Variable | Type | Default OLED | Default ePaper | Description |
|----------|------|--------------|----------------|-------------|
| `CONFIG_NICE_OLED_WIDGET_WPM_GAUGE_CUSTOM_X` | int | `0` | `16` | WPM gauge X position |
| `CONFIG_NICE_OLED_WIDGET_WPM_GAUGE_CUSTOM_Y` | int | `70` | `43` | WPM gauge Y position |
| `CONFIG_NICE_OLED_WIDGET_WPM_NEEDLE_CENTER_CUSTOM_X` | int | `12` | `33` | WPM needle center X position |
| `CONFIG_NICE_OLED_WIDGET_WPM_NEEDLE_CENTER_CUSTOM_Y` | int | `90` | `66` | WPM needle center Y position |
| `CONFIG_NICE_OLED_WIDGET_WPM_LABEL_CUSTOM_X` | int | `0` | `0` | WPM label X position |
| `CONFIG_NICE_OLED_WIDGET_WPM_LABEL_CUSTOM_Y` | int | `75` | `103` | WPM label Y position |

### Luna Widget Position

| Variable | Type | Default OLED | Default ePaper | Description |
|----------|------|--------------|----------------|-------------|
| `CONFIG_NICE_OLED_WIDGET_LUNA_CUSTOM_X` | int | `65` | `100` | Luna widget X position |
| `CONFIG_NICE_OLED_WIDGET_LUNA_CUSTOM_Y` | int | `0` | `15` | Luna widget Y position |

### Bongo Cat Widget Position

| Variable | Type | Default OLED | Default ePaper | Description |
|----------|------|--------------|----------------|-------------|
| `CONFIG_NICE_OLED_WIDGET_BONGO_CAT_CUSTOM_X` | int | `64` (POS1) / `33` (POS2) | `100` | Bongo cat widget X position |
| `CONFIG_NICE_OLED_WIDGET_BONGO_CAT_CUSTOM_Y` | int | `-9` | `8` | Bongo cat widget Y position |

### HID Indicators Widget Position

| Variable | Type | Default OLED | Default ePaper | Description |
|----------|------|--------------|----------------|-------------|
| `CONFIG_NICE_OLED_WIDGET_HID_INDICATORS_CUSTOM_X` | int | `36` | `100` | HID indicators widget X position |
| `CONFIG_NICE_OLED_WIDGET_HID_INDICATORS_CUSTOM_Y` | int | `0` | `15` (Luna) / `8` (Bongo) | HID indicators widget Y position |

### Modifiers Widget Position

| Variable | Type | Default OLED | Default ePaper | Description |
|----------|------|--------------|----------------|-------------|
| `CONFIG_NICE_OLED_WIDGET_MODIFIERS_CUSTOM_X` | int | `0` | `2` | Modifiers widget X position |
| `CONFIG_NICE_OLED_WIDGET_MODIFIERS_CUSTOM_Y` | int | `100` | `110` | Modifiers widget Y position |

### Animation Peripheral Position

| Variable | Type | Default OLED | Default ePaper | Description |
|----------|------|--------------|----------------|-------------|
| `CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_CUSTOM_X` | int | `18` | `36` | Animation peripheral X position |
| `CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_CUSTOM_Y` | int | `-18` | `0` | Animation peripheral Y position |

### Raw HID Widget Position

The Raw HID widget has multiple submodules that can be positioned independently.

#### Base Position (fallback for "HID not found" message)

| Variable | Type | Default OLED | Default ePaper | Description |
|----------|------|--------------|----------------|-------------|
| `CONFIG_NICE_OLED_WIDGET_RAW_HID_CUSTOM_X` | int | `0` | `0` | Raw HID base X position |
| `CONFIG_NICE_OLED_WIDGET_RAW_HID_CUSTOM_Y` | int | `62` | `38` | Raw HID base Y position |

#### Weather (Temperature) Submodule

| Variable | Type | Default OLED | Default ePaper | Description |
|----------|------|--------------|----------------|-------------|
| `CONFIG_NICE_OLED_WIDGET_RAW_HID_WEATHER_CUSTOM_X` | int | `0` | `0` | Weather X position |
| `CONFIG_NICE_OLED_WIDGET_RAW_HID_WEATHER_CUSTOM_Y` | int | `62` | `38` | Weather Y position |

#### Time Submodule

| Variable | Type | Default OLED | Default ePaper | Description |
|----------|------|--------------|----------------|-------------|
| `CONFIG_NICE_OLED_WIDGET_RAW_HID_TIME_CUSTOM_X` | int | `0` | `0` | Time X position |
| `CONFIG_NICE_OLED_WIDGET_RAW_HID_TIME_CUSTOM_Y` | int | `74` | `50` | Time Y position |

#### Layout Submodule

| Variable | Type | Default OLED | Default ePaper | Description |
|----------|------|--------------|----------------|-------------|
| `CONFIG_NICE_OLED_WIDGET_RAW_HID_LAYOUT_CUSTOM_X` | int | `0` | `0` | Layout X position |
| `CONFIG_NICE_OLED_WIDGET_RAW_HID_LAYOUT_CUSTOM_Y` | int | `86` | `62` | Layout Y position |

#### Volume Submodule

| Variable | Type | Default OLED | Default ePaper | Description |
|----------|------|--------------|----------------|-------------|
| `CONFIG_NICE_OLED_WIDGET_RAW_HID_VOLUME_CUSTOM_X` | int | `0` | `0` | Volume X position |
| `CONFIG_NICE_OLED_WIDGET_RAW_HID_VOLUME_CUSTOM_Y` | int | `98` | `74` | Volume Y position |

#### Media Player (Spotify) Submodule

| Variable | Type | Default OLED | Default ePaper | Description |
|----------|------|--------------|----------------|-------------|
| `CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_CUSTOM_X` | int | `0` | `0` | Media Player X position |
| `CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_CUSTOM_Y` | int | `110` | `86` | Media Player Y position |

### Responsive Bongo Cat Widget Position

| Variable | Type | Default OLED | Default ePaper | Description |
|----------|------|--------------|----------------|-------------|
| `CONFIG_NICE_OLED_WIDGET_RESPONSIVE_BONGO_CAT_CUSTOM_X` | int | `64` | `64` | Responsive bongo cat widget X position |
| `CONFIG_NICE_OLED_WIDGET_RESPONSIVE_BONGO_CAT_CUSTOM_Y` | int | `-9` | `-9` | Responsive bongo cat widget Y position |

### Sleep Status Widget Position

| Variable | Type | Default OLED | Default ePaper | Description |
|----------|------|--------------|----------------|-------------|
| `CONFIG_NICE_OLED_WIDGET_SLEEP_STATUS_CUSTOM_X` | int | `0` | `0` | Sleep status widget X position |
| `CONFIG_NICE_OLED_WIDGET_SLEEP_STATUS_CUSTOM_Y` | int | `0` | `0` | Sleep status widget Y position |


# ZMK RAW HID Implementation

This document describes the RAW HID implementation for the nice!OLED, nice!view and nice!custom.

## 0. Quick Start
1. Compile your `.uf2` and flash your keyboard including the `zmk-nice-oled` shield as indicated in the instructions in the README.md
   - don't forget to activate the flag `CONFIG_NICE_OLED_WIDGET_RAW_HID=y`
2. Download and install the `zmk-hid-host` program to your computer and configure it to always start from login
   - `zmk-hid-host` does not spend much `0%` cpu resources!
   - modify the `json` file if necessary!
   - `zmk-hid-host` is compatible for `intel` and apple `m` series processors, it is a universal binary!
3. Enjoy your keyboard!!!
   - remember to customize to your liking.
   - TIP: you can activate seeing the weather and seeing your spotify song on macos!

## 1. Introduction to RAW HID

RAW HID provides a flexible way to send and receive custom data between the keyboard and the host computer. This allows for advanced features that are not covered by standard HID profiles.

To enable RAW HID, you need to set the following options in your `Kconfig.defconfig`:

```
CONFIG_NICE_OLED_WIDGET_RAW_HID=y
```

This will enable the RAW HID interface and the corresponding widgets.

## 2. Available RAW HID Features

The following features are implemented using RAW HID:

### 2.1. Layout Switching

This feature allows you to display the current keyboard layout on the OLED screen. The layout can be sent from the host computer.

**Kconfig Options:**

```
CONFIG_NICE_OLED_WIDGET_RAW_HID_LAYOUT=y
CONFIG_NICE_OLED_WIDGET_RAW_HID_LAYOUT_LIST="DE,US,LA,AB"
CONFIG_NICE_OLED_WIDGET_RAW_HID_LAYOUT_CUSTOM_X=0
CONFIG_NICE_OLED_WIDGET_RAW_HID_LAYOUT_CUSTOM_Y=86
```

The `LAYOUT_LIST` is a comma-separated list of layout names that will be displayed on the screen based on the index received from the host.

### 2.2. Weather Widget (macOS Only)

This widget displays the current temperature received from the host computer.

**Kconfig Options:**

```
CONFIG_NICE_OLED_WIDGET_RAW_HID_WEATHER=y
CONFIG_NICE_OLED_WIDGET_RAW_HID_WEATHER_CUSTOM_X=0
CONFIG_NICE_OLED_WIDGET_RAW_HID_WEATHER_CUSTOM_Y=62
```

The host application needs to send the temperature value to the keyboard. **This feature is currently optimized for macOS.**

### 2.3. Spotify Media Player Widget (macOS Only)

This widget displays the current song playing on Spotify on a macOS computer.

**Kconfig Options:**

```
CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_SPOTIFY_MACOS=y
CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_CUSTOM_X=0
CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_CUSTOM_Y=110
```

A host application is required to read the song information from Spotify and send it to the keyboard. **This feature is exclusive to macOS.**

### 2.4. System Time Widget

This widget displays the current system time synchronized with the host computer.

**Kconfig Options:**

```
CONFIG_NICE_OLED_WIDGET_RAW_HID_TIME=y
CONFIG_NICE_OLED_WIDGET_RAW_HID_TIME_CUSTOM_X=0
CONFIG_NICE_OLED_WIDGET_RAW_HID_TIME_CUSTOM_Y=74
```

**This feature is compatible with Windows, Linux, and macOS.**

### 2.5. System Volume Widget

This widget displays the current system audio volume.

**Kconfig Options:**

```
CONFIG_NICE_OLED_WIDGET_RAW_HID_VOLUME=y
CONFIG_NICE_OLED_WIDGET_RAW_HID_VOLUME_CUSTOM_X=0
CONFIG_NICE_OLED_WIDGET_RAW_HID_VOLUME_CUSTOM_Y=98
```

**This feature is compatible with Windows, Linux, and macOS.**

## 3. Host-side application

The companion host application, `qmk-hid-host`, provides the necessary data bridge for universal Raw HID features (System Time, Volume, Layout) across Linux, Windows, and macOS.

*   **Technical Contributions (macOS Enhancements)**:
    *   **macOS Temperature Support**: Implemented platform-specific temperature monitoring for macOS within the Rust codebase.
    *   **Spotify Integration**: Engineered a native AppleScript/Rust bridge to fetch real-time track metadata from Spotify for display on the peripheral (macOS only).
    *   **Cross-Architecture Support**: Developed a universal macOS application wrapper, bundling native binaries for both Intel (`x86_64`) and Apple Silicon (`arm64`) architectures, ensuring seamless deployment across the Apple ecosystem.

You can compile the application from the source by following these steps:

1.  Clone the original repository:
    ```bash
    git clone https://github.com/zzeneg/qmk-hid-host.git
    ```
2.  Navigate to the project directory:
    ```bash
    cd qmk-hid-host
    ```
3.  Compile the project in release mode:
    ```bash
    cargo build --release
    ```
4.  The binary will be available in `target/release/`.

Once compiled, you can run the application. It will automatically detect your keyboard and start sending the required data.

## 4. Creating a `.dmg` for the macOS application

Once you have your host-side application, you can create a `.dmg` file for easy distribution and installation.

You can use the `create-dmg` tool, which can be installed via Homebrew:

```bash
brew install create-dmg
```

Then, you can create the `.dmg` file with the following command:

```bash
create-dmg \
  --volname "ZMK RAW HID Host" \
  --background "path/to/your/background.png" \
  --window-pos 200 120 \
  --window-size 600 400 \
  --icon-size 100 \
  --icon "YourApp.app" 170 200 \
  --hide-extension "YourApp.app" \
  --app-drop-link 430 200 \
  "ZMK_RAW_HID_Host.dmg" \
  "source_folder/"
```

Replace `"YourApp.app"` and `"source_folder/"` with your actual application and source folder.

## 5. Running the application on startup

To make the host application run automatically every time you start your Mac, you can add it to the "Login Items".

1.  Open **System Settings**.
2.  Go to **General** > **Login Items**.
3.  Click the **+** button.
4.  Select your application from the `/Applications` folder.
5.  Click **Open**.

Now, the application will start automatically when you log in, ensuring that the RAW HID features of your keyboard are always available.

## 6. Updating the host-side application

If you need to update the host-side application with a new binary or configuration file, you can do so by replacing the files inside the application bundle.

1.  Go to your `Applications` folder.
2.  Right-click on `ZMK HID Runner.app` and select "Show Package Contents".
3.  Navigate to `Contents/Resources/`.
4.  You will see the following structure:
    ```
    .
    ├── applet.icns
    ├── applet.rsrc
    ├── description.rtfd
    │   └── TXT.rtf
    ├── Scripts
    │   └── main.scpt
    ├── zmk-hid-host
    └── zmk-hid-host.json
    ```
5.  You can replace the `zmk-hid-host` binary and the `zmk-hid-host.json` configuration file with your new versions. Make sure the new files have the same names.

> [!NOTE]
> You can also use the `cp` command to replace the files. For example, if your new files are in the current directory, you can use the following commands:
> ```bash
> cp zmk-hid-host "/Applications/ZMK HID Runner.app/Contents/Resources/zmk-hid-host"
> cp zmk-hid-host.json "/Applications/ZMK HID Runner.app/Contents/Resources/zmk-hid-host.json"
> ```
> If for some reason the application does not work after replacing the files, check the file permissions. The binary should have execution permissions. You can set them with `chmod +x`.
> ```bash
> chmod +x "/Applications/ZMK HID Runner.app/Contents/Resources/zmk-hid-host"
> ```

## Author and Contributions

The original `qmk-hid-host` project was created by **zzeneg**, and you can find the repository here: [https://github.com/zzeneg/qmk-hid-host](https://github.com/zzeneg/qmk-hid-host).

The **Spotify** and **Weather** functionalities described in this document were added in this pull request: [https://github.com/zzeneg/qmk-hid-host/pull/24/files](https://github.com/zzeneg/qmk-hid-host/pull/24/files).

The RAW HID functionality in this project is a custom implementation, ported from the great work of **@zzeneg** in the following repositories:
- [zmk-raw-hid](https://github.com/zzeneg/zmk-raw-hid)
- [zmk-nice-view-hid](https://github.com/zzeneg/zmk-nice-view-hid)

This implementation is 100% independent and is activated only with the following flags, without needing any other files:
```
CONFIG_NICE_OLED_WIDGET_RAW_HID=y
```

# Suggestions
If you have any implementation suggestion or something similar opens a
discussion

# Limitations
While `nice_oled` offers advanced features, there are hardware and software constraints to consider.

- Memory Usage (RAM) This shield uses the LVGL graphics library, which can be memory-intensive.
  *   **High Consumption**: Enabling multiple large widgets (WPM Graph, Bongo Cat, Raw HID) can exhaust the available RAM on the microcontroller (nRF52840).
  *   **Symptoms**: If memory is low, you may experience display artifacts, freezing, or Bluetooth instability.
  *   **Mitigation**: Use the "Balanced" or "Minimal" presets described in `OPTIMIZE.md` if you encounter issues.
- Battery Life OLED displays and continuous animations consume power.
  *   **Peripheral Impact**: Running complex animations (like Pokemon or Bongo Cat) on the peripheral half (which has a smaller battery in some builds) will significantly reduce battery life.
  *   **Refresh Rate**: Higher animation speeds (lower ms values) increase CPU usage, preventing the processor from sleeping.
  *   **Mitigation**: Use `CONFIG_NICE_OLED_WIDGET_STATIC_IMAGE_PERIPHERAL=y` for better battery life on split peripherals.
- Platform Compatibility (Raw HID) The Raw HID features rely on a host-side application to send data to the keyboard.
  *   **Universal Support (macOS, Linux, Windows)**: The `nice_oled` shield and its core Raw HID modules (**System Volume**, **Time**, **Layout**) are fully compatible with macOS, Linux, and Windows.
  *   **macOS Exclusive Features**: The specific integrations for **Spotify** track info and **Weather** are currently optimized and available **only for macOS**.
- Performance & Latency The display runs on shared hardware resources.
  *   **Typing Latency**: If the display thread priority is set too high (`1` or `2`), it might interrupt key scanning, causing perceived input lag during heavy animations.
  *   **Smart Battery**: The experimental "Smart Battery" animation uses significant compute resources and is **not recommended** for stability-focused builds.
  *   **Mitigation**: Keep `CONFIG_ZMK_DISPLAY_DEDICATED_THREAD_PRIORITY` at default (`5`) or lower (`10`).
- Image Rotation Strategy To maximize efficiency on the nRF52840, **real-time image rotation is avoided**.
  *   **Pre-processing**: All animated assets are pre-rotated on the host computer during the design phase.
  *   **Reasoning**: Performing geometric transformations (rotation) at runtime is computationally expensive and memory-intensive for this architecture.
  *   **Implementation**: By storing assets as ready-to-render, pre-rotated bitmaps, we offload the processing burden from the MCU, ensuring smooth playback with minimal CPU overhead.

# Acknowledgments

This project is built upon the foundation of numerous innovative contributions from the ZMK community. We extend our sincere gratitude to the following developers whose work has been a source of inspiration and whose drivers or methods have been ported or adapted for this module.

## Core Inspiration & Drivers

*   **@M165437**: For [nice-view-gem](https://github.com/M165437/nice-view-gem), which inspired the animation implementations.
*   **@zzeneg**: For the pioneering work on [zmk-raw-hid](https://github.com/zzeneg/zmk-raw-hid) and [zmk-nice-view-hid](https://github.com/zzeneg/zmk-nice-view-hid). The Raw HID functionality in this module is a custom implementation heavily inspired by and ported from his great work.
*   **@englmaxi**: For [zmk-dongle-display](https://github.com/englmaxi/zmk-dongle-display.git), a key reference for display management.
*   **@infely**: For [nice-view-battery](https://github.com/infely/nice-view-battery).
*   **@tobiasarndt**: For [zmk-config](https://github.com/tobiasarndt/zmk-config) and the sleep status implementation.
*   **@infused-kim**: For [Nice Peripheral View (tmp-npv)](https://github.com/infused-kim/tmp-npv).
*   **@codyd51**: For [Mighty-Mitts](https://github.com/codyd51/Mighty-Mitts).
*   **@badjeff**: For [zmk-hid-io](https://github.com/badjeff/zmk-hid-io).
*   **@carrefinho**: For [prospector-zmk-module](https://github.com/carrefinho/prospector-zmk-module).

Your contributions have been instrumental in making this modular shield system possible. Thank you for open-sourcing your work and driving the ZMK ecosystem forward.

# If you like my contributions, please don't forget the following:

I appreciate if you follow me on [<img align="center" alt="GitHub" width="22px" src="https://github.com/mctechnology17/mctechnology17/blob/main/src/github.png" />][github] and [<img align="center" alt="YouTube" width="22px" src="https://github.com/mctechnology17/mctechnology17/blob/main/src/youtube.png" />][youtube] !

![Star](./assets/star.GIF)

# My other projects:
- [zmk-dongle-display-view]: Use your [niceview] vertical with very beautiful widgets
- [zmk-oled-adapter]: This module is an adapter for 128x32, 128x64 and 128x128
  OLED screens, to be used on keyboards with ZMK without having to modify the
  shields of the keyboards.
- [qmk-config] will be updated soon
- [qmk_userspace] will be updated soon
- [vimtools] swiss army knife for vim (features and settings that will make your life easier)
- [gm] cross-platform git manager with friendly user interface
- [vim-better-header] the best automated template
- [vim-executor] multilanguage code executor

If you enjoy my contributions, feel free to donate.
- [paypal]
- [sponsor]

<p align="right">
  <a href="#top">⬆️  back to Table of Contents</a>
</p>

<p align="center">
        <img src="./assets/Bottom.svg" alt="Bottom" />
</p>

[zmk-config]: https://github.com/mctechnology17/zmk-config
[zmk-dongle-display-view]: https://github.com/mctechnology17/zmk-dongle-display-view
[zmk-oled-adapter]: https://github.com/mctechnology17/zmk-oled-adapter

[niceview]: https://nicekeyboards.com/nice-view/
[oled]: https://keycapsss.com/keyboard-parts/parts/80/0.91-oled-lcd-display-128x32-ssd1306-i2c
[nice-view]: https://nicekeyboards.com/nice-view
[puchi_ble_v1]: (https://keycapsss.com/keyboard-parts/mcu-controller/202/puchi-ble-wireless-microcontroller-pro-micro-replacement?number=KC10157_SWITCH&c=18)
[seeeduino_xiao_ble]: (https://keycapsss.com/keyboard-parts/mcu-controller/212/seeed-studio-xiao-nrf52840-rp2040-esp32c3?number=KC10167_NRF)
[nice_nano_v2]: (https://nicekeyboards.com/nice-nano)
[keymap-editor]: https://nickcoutsos.github.io/keymap-editor/
[ZMK firmware]: https://github.com/zmkfirmware/zmk/
[ZMK documentation]: https://zmk.dev/docs/user-setup
[ZMK keycodes]: https://zmk.dev/docs/codes
[ZMK Discord]: https://zmk.dev/community/discord/invite

[qmk-config]: https://github.com/mctechnology17/qmk-config
[qmk_userspace]: https://github.com/mctechnology17/qmk_userspace

[git]: (https://github.com/git-guides/install-git)

[github]: https://github.com/mctechnology17
[twitter]: https://twitter.com/mctechnology17
[youtube]: https://www.youtube.com/c/mctechnology17
[instagram]: https://www.instagram.com/mctechnology17/
[facebook]: https://m.facebook.com/mctechnology17/
[reddit]: https://www.reddit.com/user/mctechnology17

[vim-executor]: https://github.com/mctechnology17/vim-executor
[vim-better-header]: https://github.com/mctechnology17/vim-better-header
[gm]: https://github.com/mctechnology17/gm
[vimtools]: https://github.com/mctechnology17/vimtools

[jailbreakrepo]: https://mctechnology17.github.io/
[uiglitch]: https://repo.packix.com/package/com.mctechnology.uiglitch/
[uiswitches]: https://repo.packix.com/package/com.mctechnology.uiswitches/
[uibadge]: https://repo.packix.com/package/com.mctechnology.uibadge/

[youtuberepo]: https://github.com/mctechnology17/youtube_repo_mc_technology

[sponsor]: https://github.com/sponsors/mctechnology17
[paypal]: https://www.paypal.me/mctechnology17
