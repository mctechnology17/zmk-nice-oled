# Optimization Guide

Advanced configuration guide for optimizing memory, CPU usage, and animations in the nice_oled shield.

## Table of Contents

- [Important Considerations](#important-considerations)
- [Memory Configuration](#memory-configuration)
- [Display Thread Configuration](#display-thread-configuration)
- [Animation Optimization](#animation-optimization)
- [Build Optimization](#build-optimization)
- [Presets](#presets)
- [ZMK Integration](#zmk-integration)

---

## Important Considerations

This shield merges with your ZMK keyboard configuration and shares resources with the main firmware. Poor configuration can affect:

- **Typing latency**: If the display consumes too many resources
- **Bluetooth stability**: If memory is insufficient
- **Battery life**: Frequent animations = higher consumption

> Always test your configuration by typing quickly while animations are active.

---

## Memory Configuration

### LVGL Memory Pool

The memory pool is used by LVGL for dynamic allocations (widgets, images, animations).

```ini
CONFIG_LV_Z_MEM_POOL_SIZE=8192
```

| Value | Use Case | Notes |
|-------|----------|-------|
| `4096` | Minimal setup, static images only | Default ZMK value |
| `8192` | Standard setup with animations | **Recommended** |
| `10000` | Multiple animations + RAW HID | For complex setups |
| `12288` | All features enabled | High memory usage |

**Symptoms of insufficient memory:**
- Display freezes or shows artifacts
- Animations stop working
- Keyboard becomes unresponsive

### VDB (Virtual Display Buffer) Size

Controls the percentage of the display buffer in RAM.

```ini
CONFIG_LV_Z_VDB_SIZE=64
```

| Value | Display Type | RAM Usage |
|-------|--------------|-----------|
| `64` | OLED 128x32 | ~256 bytes |
| `100` | ePaper/Custom | Full buffer |

> Lower values reduce RAM but increase CPU usage (more refresh cycles).

### Bits Per Pixel

```ini
CONFIG_LV_Z_BITS_PER_PIXEL=1
```

For monochrome displays, always use `1`. Higher values waste memory.

### Color Depth

```ini
CONFIG_LV_COLOR_DEPTH_1=y
```

For monochrome OLED/ePaper displays, use 1-bit color depth.

---

## Display Thread Configuration

### Stack Size

Controls the stack allocated for the display thread.

```ini
CONFIG_ZMK_DISPLAY_DEDICATED_THREAD_STACK_SIZE=2048
```

| Value | Use Case |
|-------|----------|
| `2048` | Default, basic widgets |
| `3072` | Standard animations |
| `4096` | Responsive widgets, complex animations |
| `5120` | All features + RAW HID |

**Increase if you see:**
- Stack overflow errors in logs
- Random crashes during animations
- Display not updating

### Thread Priority

```ini
CONFIG_ZMK_DISPLAY_DEDICATED_THREAD_PRIORITY=5
```

| Value | Behavior |
|-------|----------|
| `1` | Highest priority (smoother animations, may affect typing) |
| `5` | Balanced (default) |
| `10` | Lower priority (typing prioritized over display) |

### Work Queue

```ini
CONFIG_ZMK_DISPLAY_WORK_QUEUE_DEDICATED=y
```

Options:
- `ZMK_DISPLAY_WORK_QUEUE_DEDICATED` - Dedicated thread for display (recommended)
- `ZMK_DISPLAY_WORK_QUEUE_SYSTEM` - Shared with system (saves RAM, may lag)

---

## Animation Optimization

### Animation Duration

Longer durations = less CPU usage, smoother appearance.

```ini
# Luna/Bongo Cat animations
CONFIG_NICE_OLED_WIDGET_WPM_LUNA_ANIMATION_MS=300
CONFIG_NICE_OLED_WIDGET_WPM_BONGO_CAT_ANIMATION_MS=300
CONFIG_NICE_OLED_WIDGET_HID_INDICATORS_LUNA_ANIMATION_MS=300
CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_LUNA_ANIMATION_MS=300

# Peripheral animations
CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_MS=960
```

| Value | Effect |
|-------|--------|
| `200` | Fast, high CPU |
| `300` | Balanced |
| `500` | Smooth, low CPU |
| `1000+` | Very smooth, minimal CPU |

### Disable Unnecessary Animations

To save resources, disable animations you don't need:

```ini
# Disable WPM animations
CONFIG_NICE_OLED_WIDGET_WPM_LUNA=n
CONFIG_NICE_OLED_WIDGET_WPM_BONGO_CAT=n

# Disable HID indicator animations
CONFIG_NICE_OLED_WIDGET_HID_INDICATORS_LUNA=n
CONFIG_NICE_OLED_WIDGET_HID_INDICATORS_BONGO_CAT=n

# Disable modifier animations
CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_LUNA=n

# Disable peripheral animations
CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL=n
```

### Static Images vs Animations

Static images use significantly less resources:

```ini
# Use static image instead of animation on peripheral
CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL=n
CONFIG_NICE_OLED_WIDGET_STATIC_IMAGE_PERIPHERAL=y
CONFIG_NICE_OLED_WIDGET_STATIC_IMAGE_PERIPHERAL_VIM=y
```

---

## Build Optimization

### Compiler Optimization Level

In your `CMakeLists.txt` or build configuration:

```ini
CONFIG_SIZE_OPTIMIZATIONS=y      # Optimize for size (-Os)
# or
CONFIG_SPEED_OPTIMIZATIONS=y     # Optimize for speed (-O2)
```

For most keyboards, size optimization is preferred.

### Disable Unused Features

```ini
# Disable WPM if not needed
CONFIG_NICE_OLED_WIDGET_WPM=n
CONFIG_ZMK_WPM=n

# Disable RAW HID if not used
CONFIG_NICE_OLED_WIDGET_RAW_HID=n

# Disable responsive widgets
CONFIG_NICE_OLED_WIDGET_RESPONSIVE=n

# Disable sleep art
CONFIG_NICE_OLED_SHOW_SLEEP_ART_ON_IDLE=n
CONFIG_NICE_OLED_SHOW_SLEEP_ART_ON_SLEEP=n
```

### Font Optimization

Only include fonts you need:

```ini
# Disable extra fonts if not using RAW HID
CONFIG_LV_FONT_MONTSERRAT_14=n
CONFIG_LV_FONT_MONTSERRAT_22=n
```

---

## Presets

### Minimal (Low Memory)

For keyboards with limited resources:

```ini
CONFIG_LV_Z_MEM_POOL_SIZE=4096
CONFIG_LV_Z_VDB_SIZE=64
CONFIG_ZMK_DISPLAY_DEDICATED_THREAD_STACK_SIZE=2048
CONFIG_ZMK_DISPLAY_WORK_QUEUE_SYSTEM=y

CONFIG_NICE_OLED_WIDGET_WPM=n
CONFIG_NICE_OLED_WIDGET_HID_INDICATORS=n
CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_LUNA=n
CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL=n
CONFIG_NICE_OLED_WIDGET_STATIC_IMAGE_PERIPHERAL=y
```

### Balanced (Recommended)

Good balance between features and resources:

```ini
CONFIG_LV_Z_MEM_POOL_SIZE=8192
CONFIG_LV_Z_VDB_SIZE=64
CONFIG_ZMK_DISPLAY_DEDICATED_THREAD_STACK_SIZE=2048
CONFIG_ZMK_DISPLAY_WORK_QUEUE_DEDICATED=y

CONFIG_NICE_OLED_WIDGET_WPM=y
CONFIG_NICE_OLED_WIDGET_WPM_LUNA=y
CONFIG_NICE_OLED_WIDGET_WPM_LUNA_ANIMATION_MS=300
CONFIG_NICE_OLED_WIDGET_HID_INDICATORS=y
CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL=y
CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_MS=960
```

### Full Features (High Memory)

All features enabled:

```ini
CONFIG_LV_Z_MEM_POOL_SIZE=12288
CONFIG_LV_Z_VDB_SIZE=100
CONFIG_ZMK_DISPLAY_DEDICATED_THREAD_STACK_SIZE=4096
CONFIG_ZMK_DISPLAY_DEDICATED_THREAD_PRIORITY=1
CONFIG_ZMK_DISPLAY_WORK_QUEUE_DEDICATED=y

CONFIG_NICE_OLED_WIDGET_WPM=y
CONFIG_NICE_OLED_WIDGET_WPM_LUNA=y
CONFIG_NICE_OLED_WIDGET_WPM_BONGO_CAT=n
CONFIG_NICE_OLED_WIDGET_RESPONSIVE=y
CONFIG_NICE_OLED_WIDGET_RAW_HID=y
CONFIG_NICE_OLED_WIDGET_HID_INDICATORS=y
CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS=y
CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL=y
```

---

## Troubleshooting

| Problem | Possible Cause | Solution |
|---------|----------------|----------|
| Display freezes | Low memory pool | Increase `LV_Z_MEM_POOL_SIZE` |
| Animations stuttering | Low thread priority | Decrease priority value (e.g., `1`) |
| Stack overflow | Small stack size | Increase `DEDICATED_THREAD_STACK_SIZE` |
| Slow response | Too many features | Disable unused widgets |
| Build too large | Too many assets | Disable unused animations/images |
| Typing lag | Display consuming too much CPU | Increase priority (e.g., `10`), reduce animations |
| BLE disconnections | Insufficient memory | Reduce features, increase pool |
| RAW HID not working | Stack too small | Increase to `3072` or more |

---

## ZMK Integration

### How It Affects the Keyboard

This shield shares resources with the main ZMK firmware:

```
+------------------+     +------------------+
|   ZMK Firmware   |     |   nice_oled      |
|------------------|     |------------------|
| - Key scanning   |<--->| - LVGL rendering |
| - BLE stack      |     | - Animations     |
| - USB HID        |     | - RAW HID        |
| - Layer logic    |     | - WPM tracking   |
+------------------+     +------------------+
         |                       |
         +--------> RAM <--------+
         +--------> CPU <--------+
```

### RAW HID and Resources

When using RAW HID (time, volume, host layout):

```ini
# RAW HID requires additional memory
CONFIG_NICE_OLED_WIDGET_RAW_HID=y
CONFIG_LV_Z_MEM_POOL_SIZE=10000  # Increase pool
CONFIG_ZMK_DISPLAY_DEDICATED_THREAD_STACK_SIZE=3072  # Increase stack

# RAW HID configuration
CONFIG_USB_HID_DEVICE_COUNT=2
CONFIG_NICE_OLED_WIDGET_RAW_HID_REPORT_SIZE=32
```

### Split Keyboards

For split keyboards, the central has more load than the peripheral:

**Central (more resources):**
```ini
CONFIG_LV_Z_MEM_POOL_SIZE=8192
CONFIG_ZMK_DISPLAY_DEDICATED_THREAD_STACK_SIZE=3072
```

**Peripheral (less resources):**
```ini
CONFIG_LV_Z_MEM_POOL_SIZE=4096
CONFIG_ZMK_DISPLAY_DEDICATED_THREAD_STACK_SIZE=2048
CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_MS=1500  # Slower animations
```

---

## Module Impact Reference

Approximate resource usage when enabling/disabling each module:

### Central Modules

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

### Peripheral Modules

| Module | Config | RAM | Flash | Notes |
|--------|--------|-----|-------|-------|
| **Animation Peripheral** | `NICE_OLED_WIDGET_ANIMATION_PERIPHERAL=n` | -1KB | -8KB | All peripheral animations |
| **Cat Animation** | `NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_CAT=n` | -0.5KB | -4KB | Cat sprites (2 frames) |
| **Gem Animation** | `NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_GEM=n` | -0.5KB | -3KB | Gem sprites |
| **Pokemon Animation** | `NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_POKEMON=n` | -1KB | -12KB | Many frames |
| **Head Animation** | `NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_HEAD=n` | -0.5KB | -4KB | Head sprites |
| **Spaceman Animation** | `NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_SPACEMAN=n` | -0.5KB | -4KB | Spaceman sprites |
| **Static Image** | `NICE_OLED_WIDGET_STATIC_IMAGE_PERIPHERAL=y` | +0.1KB | -4KB | Static vs animated |
| **Sleep Art** | `NICE_OLED_SHOW_SLEEP_ART_ON_IDLE=n` | -0.5KB | -2KB | Sleep screen image |

### Optimization Examples

**Minimal Setup** (save ~8KB RAM, ~30KB Flash):
```ini
CONFIG_NICE_OLED_WIDGET_WPM=n
CONFIG_NICE_OLED_WIDGET_HID_INDICATORS=n
CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_LUNA=n
CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL=n
CONFIG_NICE_OLED_WIDGET_STATIC_IMAGE_PERIPHERAL=y
```

**Keep WPM Luna only** (save ~4KB RAM, ~15KB Flash):
```ini
CONFIG_NICE_OLED_WIDGET_WPM_LUNA=y
CONFIG_NICE_OLED_WIDGET_HID_INDICATORS_LUNA=n
CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_LUNA=n
```

**Disable all Luna** (save ~3KB RAM, ~12KB Flash):
```ini
CONFIG_NICE_OLED_WIDGET_WPM_LUNA=n
CONFIG_NICE_OLED_WIDGET_HID_INDICATORS_LUNA=n
CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_LUNA=n
```

> These are estimates based on typical builds. Actual savings vary by compiler optimization and configuration.

---

## Final Tips

1. **Start with the balanced preset** and adjust as needed
2. **Monitor keyboard behavior** while typing quickly
3. **If using RAW HID**, make sure you have enough memory
4. **On split keyboards**, optimize the peripheral more aggressively
5. **Always test** after changing memory configuration
