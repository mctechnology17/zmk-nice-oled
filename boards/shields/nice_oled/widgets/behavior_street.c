/*
 * ========================================
 * FileName: behavior_street.c
 * Date: 19.November.2026
 * Author: Marcos Chow Castro
 * Email: mctechnology170318@gmail.com
 * GitHub: https://github.com/mctechnology17
 * Brief: behavior &street: triggers the animations of the street character from the keymap.
 * =========================================
 * SPDX-License-Identifier: MIT
 */

#define DT_DRV_COMPAT zmk_nice_oled_behavior_street

#include <zephyr/device.h>

#include <drivers/behavior.h>
#include <zmk/behavior.h>

#if DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT)

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_STREET)
#include "street.h"
#endif

/* Without the character the key does nothing, so one keymap serves every build. */

static int on_street_pressed(struct zmk_behavior_binding *binding,
                             struct zmk_behavior_binding_event event) {
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_STREET)
    zmk_widget_street_key(binding->param1, true);
#endif
    return ZMK_BEHAVIOR_OPAQUE;
}

static int on_street_released(struct zmk_behavior_binding *binding,
                              struct zmk_behavior_binding_event event) {
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_STREET)
    zmk_widget_street_key(binding->param1, false);
#endif
    return ZMK_BEHAVIOR_OPAQUE;
}

static const struct behavior_driver_api behavior_street_driver_api = {
    .binding_pressed = on_street_pressed,
    .binding_released = on_street_released,
    .locality = BEHAVIOR_LOCALITY_CENTRAL,
};

BEHAVIOR_DT_INST_DEFINE(0, NULL, NULL, NULL, NULL, POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,
                        &behavior_street_driver_api);

#endif /* DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT) */

/* vim: set ft=c fdm=marker: */
