/*
 * ========================================
 * FileName: caps_word.c
 * Date: 19.November.2026
 * Author: Marcos Chow Castro
 * Email: mctechnology170318@gmail.com
 * GitHub: https://github.com/mctechnology17
 * Brief: caps word state for the widgets of zmk-nice-oled.
 * =========================================
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/device.h>
#include <zephyr/devicetree.h>

#include "caps_word.h"

/*
 * ZMK does not offer the state of caps word. It is the first field of the data of the behavior
 * (struct behavior_caps_word_data { bool active; } in app/src/behaviors/behavior_caps_word.c of
 * ZMK), so it is read from there. If ZMK changes that struct, turn CONFIG_NICE_OLED_CAPS_WORD off.
 */
#define CAPS_WORD_DEV(node) DEVICE_DT_GET(node),
static const struct device *const caps_word_devs[] = {
    DT_FOREACH_STATUS_OKAY(zmk_behavior_caps_word, CAPS_WORD_DEV)};

bool nice_oled_caps_word_active(void) {
    for (int i = 0; i < ARRAY_SIZE(caps_word_devs); i++) {
        const bool *active = caps_word_devs[i]->data;
        if (active != NULL && *active) {
            return true;
        }
    }
    return false;
}
