# Optimization Guide

How to set memory, the display thread and the animations of the nice_oled shield.

> [!IMPORTANT]
> This guide is for the branch `zmk-v0.4` (ZMK v0.4, Zephyr 4.1, **LVGL 9**).
> LVGL 9 needs about twice the stack and twice the memory pool of LVGL 8.
> The values that older versions of this guide recommended (stack `2048` to
> `5120`, pool `4096`) are too small now: with them the display thread
> overflows its stack and **the whole keyboard freezes a few seconds after it
> starts** (no keys, no display updates).
> If you copied one of the old presets into your `.conf`, remove those lines.
> For ZMK v0.3 use the guide of the branch `zmk-v0.3`.

## Table of Contents

- [The short version](#the-short-version)
- [Which value wins](#which-value-wins)
- [Check what your build really uses](#check-what-your-build-really-uses)
- [Display thread](#display-thread)
- [Memory](#memory)
- [Animations](#animations)
- [Battery](#battery)
- [Disable what you do not use](#disable-what-you-do-not-use)
- [Presets](#presets)
- [The keyboard freezes: how to find out why](#the-keyboard-freezes-how-to-find-out-why)
- [Troubleshooting](#troubleshooting)
- [Module impact reference](#module-impact-reference)

---

## The short version

The shield already sets what LVGL 9 needs. You do not have to add anything:

| Option | Default of this branch | Where it is set |
|---|---|---|
| `CONFIG_ZMK_DISPLAY_WORK_QUEUE_DEDICATED` | `y` | `Kconfig.defconfig` |
| `CONFIG_ZMK_DISPLAY_DEDICATED_THREAD_STACK_SIZE` | `8192` | `nice_oled.conf`, `nice_epaper.conf`, `nice_custom.conf` |
| `CONFIG_ZMK_DISPLAY_DEDICATED_THREAD_PRIORITY` | `5` (`3` with `NICE_OLED_WIDGET_RESPONSIVE`) | `Kconfig.defconfig` |
| `CONFIG_LV_Z_MEM_POOL_SIZE` | `16384` central or non-split, `8192` peripheral | `Kconfig.defconfig` |
| `CONFIG_LV_Z_VDB_SIZE` | `64` OLED, `100` nice!view | `Kconfig.defconfig` |

Rules:

1. Do not set the stack below `8192` or the pool below the defaults.
2. Do not lower them to save RAM: the nRF52840 has 256 KB and a full central
   build uses about 168 KB.
3. If the keyboard freezes, first [check what your build really
   uses](#check-what-your-build-really-uses).

---

## Which value wins

The same option can be set in several places. From weakest to strongest:

1. `Kconfig.defconfig` of the shields and modules (defaults).
2. `.conf` of every shield in your `shield:` line (this is where the stack
   of this shield is set).
3. Your own `config/<keyboard>.conf`.
4. `cmake-args` in `build.yaml` (`-DCONFIG_...=...`).

So a line in your own `.conf` or in `cmake-args` beats the value of this
shield. That is how an old preset lowers the stack without any warning.

Two more things to know:

- ZMK defines `CONFIG_ZMK_DISPLAY_DEDICATED_THREAD_STACK_SIZE` before the
  shield defaults are read, so a `default` in a `Kconfig.defconfig` never
  applies to it. Only a `.conf` or `cmake-args` can change it.
- The shield of your keyboard can set display defaults too (for example
  `LV_Z_VDB_SIZE` or `LV_DPI_DEF`). When two shields set a default for the
  same option, the first one read wins. For those two options that is fine.

---

## Check what your build really uses

Do not guess: look at the merged configuration of the build.

Local build:

```sh
grep -E "ZMK_DISPLAY_WORK_QUEUE_DEDICATED|ZMK_DISPLAY_DEDICATED_THREAD|LV_Z_MEM_POOL_SIZE|LV_Z_VDB_SIZE" build/zephyr/.config
```

GitHub Actions: open the build job, step "Kconfig file" (it prints the
merged `.config`), and search for the same names.

Expected on this branch:

```ini
CONFIG_ZMK_DISPLAY_WORK_QUEUE_DEDICATED=y
CONFIG_ZMK_DISPLAY_DEDICATED_THREAD_STACK_SIZE=8192
CONFIG_ZMK_DISPLAY_DEDICATED_THREAD_PRIORITY=5
CONFIG_LV_Z_MEM_POOL_SIZE=16384   # 8192 on a peripheral
```

---

## Display thread

### Stack size

```ini
CONFIG_ZMK_DISPLAY_DEDICATED_THREAD_STACK_SIZE=8192
```

Measured with the thread analyzer on a Corne central with a nice!view, the
default widgets and RAW HID: 3784 bytes on one board and 3728 on another.
ZMK alone gives 3072, which overflows. `8192` leaves room for the widgets
that were not measured (Luna, Bongo Cat, responsive, the peripheral
animations).

| Value | Result |
|---|---|
| `2048` to `3584` | Overflows: the keyboard freezes a few seconds after it starts |
| `4096` to `6144` | Works with the default widgets, with little or no margin |
| `8192` | **Default. Keep it** |

A stack overflow in the display thread stops the whole firmware, not only
the display: the keys stop too.

### Work queue

```ini
CONFIG_ZMK_DISPLAY_WORK_QUEUE_DEDICATED=y
```

Keep the dedicated thread. With `ZMK_DISPLAY_WORK_QUEUE_SYSTEM` the display
is drawn on the system work queue, whose stack (`3072` in ZMK) is too small
for LVGL 9 and is shared with the rest of the firmware.

### Priority

```ini
CONFIG_ZMK_DISPLAY_DEDICATED_THREAD_PRIORITY=5
```

A lower number is a higher priority.

| Value | Behavior |
|---|---|
| `1` to `2` | Display first. Can delay key scanning during heavy animations |
| `3` | Used by `NICE_OLED_WIDGET_RESPONSIVE` |
| `5` | Default |
| `10` | Typing first, the display waits |

---

## Memory

### LVGL memory pool

```ini
CONFIG_LV_Z_MEM_POOL_SIZE=16384
```

Used by LVGL for objects, animations and drawing.

| Value | Use |
|---|---|
| `4096` | Too small for LVGL 9 (it was the LVGL 8 value) |
| `8192` | Peripheral half (default there) |
| `16384` | Central or non-split keyboard (default there) |

When the pool is too small the display stops updating or shows garbage. If
you see that on a peripheral with a long animation, try `12288`.

### Canvas

Since LVGL 9 the canvas is a square of 160x160 pixels with one byte per
pixel, plus a copy of the same size that is used to rotate it: about 51 KB
of RAM that are not part of the pool. This is fixed and it is the main
reason why a build with this shield uses more RAM than one with the
`nice_view` shield of ZMK.

### VDB size

```ini
CONFIG_LV_Z_VDB_SIZE=64
```

Percentage of the screen that LVGL renders in one pass. `64` for OLED and
`100` for nice!view are the defaults; a lower value saves a little RAM and
costs more refresh passes. `CONFIG_LV_Z_BITS_PER_PIXEL=1` and
`CONFIG_LV_COLOR_DEPTH_1=y` are set by the shield: do not change them.

---

## Animations

Longer durations mean fewer redraws and less CPU.

```ini
# Luna/Bongo Cat animations
CONFIG_NICE_OLED_WIDGET_WPM_LUNA_ANIMATION_MS=300
CONFIG_NICE_OLED_WIDGET_WPM_BONGO_CAT_ANIMATION_MS=300
CONFIG_NICE_OLED_WIDGET_HID_INDICATORS_LUNA_ANIMATION_MS=300
CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_LUNA_ANIMATION_MS=300

# Peripheral animations (length of the whole loop)
CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_MS=960
```

A static image instead of an animation on the peripheral:

```ini
CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL=n
CONFIG_NICE_OLED_WIDGET_STATIC_IMAGE_PERIPHERAL=y
CONFIG_NICE_OLED_WIDGET_STATIC_IMAGE_PERIPHERAL_VIM=y
```

---

## Battery

What costs battery is how often the screen is redrawn, not the stack or the
pool sizes (RAM that is reserved costs nothing).

- Every change of a widget redraws and rotates the whole canvas. Widgets that
  change all the time (WPM, modifiers, responsive Bongo Cat) keep the CPU
  awake while you type.
- On the peripheral, an animation redraws many times per second, all day. A
  static image is the cheapest choice
  (`CONFIG_NICE_OLED_WIDGET_STATIC_IMAGE_PERIPHERAL=y`), and
  `CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_SMART_BATTERY=y` plays the
  animation only while charging.
- A nice!view keeps its image without power; an OLED uses power for every
  lit pixel.

---

## Disable what you do not use

```ini
# WPM
CONFIG_NICE_OLED_WIDGET_WPM=n
CONFIG_ZMK_WPM=n

# RAW HID
CONFIG_NICE_OLED_WIDGET_RAW_HID=n

# Responsive widgets
CONFIG_NICE_OLED_WIDGET_RESPONSIVE=n

# Sleep art
CONFIG_NICE_OLED_SHOW_SLEEP_ART_ON_IDLE=n
CONFIG_NICE_OLED_SHOW_SLEEP_ART_ON_SLEEP=n

# Animations
CONFIG_NICE_OLED_WIDGET_WPM_LUNA=n
CONFIG_NICE_OLED_WIDGET_WPM_BONGO_CAT=n
CONFIG_NICE_OLED_WIDGET_HID_INDICATORS_LUNA=n
CONFIG_NICE_OLED_WIDGET_HID_INDICATORS_BONGO_CAT=n
CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_LUNA=n
```

---

## Presets

None of them touches the stack, the work queue or the pool: the defaults of
the shield are right for all three.

### Minimal (longest battery life)

```ini
CONFIG_NICE_OLED_WIDGET_WPM=n
CONFIG_NICE_OLED_WIDGET_HID_INDICATORS=n
CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_LUNA=n
CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL=n
CONFIG_NICE_OLED_WIDGET_STATIC_IMAGE_PERIPHERAL=y
```

### Balanced (recommended)

```ini
CONFIG_NICE_OLED_WIDGET_WPM=y
CONFIG_NICE_OLED_WIDGET_WPM_LUNA=y
CONFIG_NICE_OLED_WIDGET_WPM_LUNA_ANIMATION_MS=300
CONFIG_NICE_OLED_WIDGET_HID_INDICATORS=y
CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL=y
CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_MS=960
```

### Full features

```ini
CONFIG_NICE_OLED_WIDGET_WPM=y
CONFIG_NICE_OLED_WIDGET_WPM_LUNA=y
CONFIG_NICE_OLED_WIDGET_RESPONSIVE=y
CONFIG_NICE_OLED_WIDGET_RAW_HID=y
CONFIG_NICE_OLED_WIDGET_HID_INDICATORS=y
CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS=y
CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL=y
```

---

## The keyboard freezes: how to find out why

1. [Check the merged configuration](#check-what-your-build-really-uses). If
   the stack is not `8192`, something in your config lowers it.
2. Build with only `zmk` and `zmk-nice-oled` in `west.yml`, then add your
   other modules back one at a time. A display module that is not ported to
   LVGL 9 can break the build or the firmware.
3. Read the log. Add the snippet `zmk-usb-logging` to the build of the half
   that freezes, with these options:

   ```ini
   CONFIG_THREAD_ANALYZER=y
   CONFIG_THREAD_ANALYZER_AUTO=y
   CONFIG_THREAD_ANALYZER_USE_LOG=y
   CONFIG_THREAD_ANALYZER_AUTO_INTERVAL=30
   CONFIG_MPU_STACK_GUARD=y
   ```

   Connect that half by USB and read its serial port (for example
   `cat /dev/ttyACM0` on Linux). Every 30 seconds the log lists each thread
   with its stack use. A thread close to 100 % is the one to enlarge, and a
   line with `FATAL` names the thread that failed.

   On a nice!nano do not combine `zmk-usb-logging` with the snippet
   `studio-rpc-usb-uart`: the board runs out of USB endpoints and does not
   start. Leave ZMK Studio out of the debug build.

---

## Troubleshooting

| Problem | Likely cause | What to do |
|---|---|---|
| Keyboard freezes a few seconds after it starts (keys too) | Display thread stack too small | Remove old stack values from your config; check that the merged value is `8192` |
| Display stops updating, keys still work | LVGL pool too small | Check `LV_Z_MEM_POOL_SIZE` (central `16384`, peripheral `8192`) |
| Build fails in another display module | That module is not ported to LVGL 9 | Remove it from `west.yml` or use its Zephyr 4.1 branch |
| Typing feels slow during animations | Display priority too high | Keep priority `5`, or `10` |
| Battery drains fast | Screen redrawn all the time | Static image or smart battery on the peripheral, slower animations |

---

## Module impact reference

RAM and flash that each option saves when it is turned off. These numbers
are estimates from ZMK v0.3 (LVGL 8) and were not measured again on this
branch: use them only to compare options with each other.

### Central modules

| Module | Config | RAM | Flash | Notes |
|--------|--------|-----|-------|-------|
| **WPM Widget** | `NICE_OLED_WIDGET_WPM=n` | -0.5KB | -2KB | Disables WPM tracking entirely |
| **WPM Luna** | `NICE_OLED_WIDGET_WPM_LUNA=n` | -1KB | -4KB | Saves Luna sprites |
| **WPM Bongo Cat** | `NICE_OLED_WIDGET_WPM_BONGO_CAT=n` | -1.5KB | -6KB | Larger sprites than Luna |
| **WPM Speedometer** | `NICE_OLED_WIDGET_WPM_SPEEDOMETER=n` | -0.3KB | -1KB | Gauge drawing code |
| **WPM Graph** | `NICE_OLED_WIDGET_WPM_GRAPH=n` | -0.3KB | -1KB | Graph drawing code |
| **HID Indicators** | `NICE_OLED_WIDGET_HID_INDICATORS=n` | -0.5KB | -1.5KB | CapsLock/NumLock status |
| **HID Luna** | `NICE_OLED_WIDGET_HID_INDICATORS_LUNA=n` | -1KB | -4KB | Luna on CapsLock |
| **HID Bongo Cat** | `NICE_OLED_WIDGET_HID_INDICATORS_BONGO_CAT=n` | -1.5KB | -6KB | Bongo on CapsLock |
| **Modifiers Indicators** | `NICE_OLED_WIDGET_MODIFIERS_INDICATORS=n` | -0.5KB | -2KB | Ctrl/Shift/Alt/Gui display |
| **Modifiers Luna** | `NICE_OLED_WIDGET_MODIFIERS_INDICATORS_LUNA=n` | -1KB | -4KB | Luna on modifiers |
| **Modifiers Symbols** | `NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED_SYMBOL=n` | -0.2KB | -3KB | Symbol icons |
| **Layer Widget** | `NICE_OLED_WIDGET_LAYER=n` | -0.2KB | -0.5KB | Layer name display |
| **Profile Widget** | `NICE_OLED_WIDGET_PROFILE_BIG=n` | -0.3KB | -2KB | Big profile icons |
| **RAW HID** | `NICE_OLED_WIDGET_RAW_HID=n` | -1KB | -4KB | Time/Volume/Layout from host |
| **RAW HID + Font** | (includes Montserrat 14) | -3KB | -8KB | RAW HID with font |
| **Responsive** | `NICE_OLED_WIDGET_RESPONSIVE=n` | -0.5KB | -2KB | Keystroke animations |

### Peripheral modules

| Module | Config | RAM | Flash | Notes |
|--------|--------|-----|-------|-------|
| **Animation Peripheral** | `NICE_OLED_WIDGET_ANIMATION_PERIPHERAL=n` | -1KB | -8KB | All peripheral animations |
| **Cat Animation** | `NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_CAT=n` | -0.5KB | -4KB | Cat sprites |
| **Gem Animation** | `NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_GEM=n` | -0.5KB | -3KB | Gem sprites |
| **Pokemon Animation** | `NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_POKEMON=n` | -1KB | -12KB | Many frames |
| **Head Animation** | `NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_HEAD=n` | -0.5KB | -4KB | Head sprites |
| **Spaceman Animation** | `NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_SPACEMAN=n` | -0.5KB | -4KB | Spaceman sprites |
| **Static Image** | `NICE_OLED_WIDGET_STATIC_IMAGE_PERIPHERAL=y` | +0.1KB | -4KB | Static vs animated |
| **Sleep Art** | `NICE_OLED_SHOW_SLEEP_ART_ON_IDLE=n` | -0.5KB | -2KB | Sleep screen image |

---

## Final tips

1. Start without any memory or stack option of your own.
2. Type fast for a while with the animations on, and with every Bluetooth
   host you use connected.
3. On a split keyboard, save battery on the peripheral first.
4. After any change in memory options, check the merged configuration.
