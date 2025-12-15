#pragma once

#include <lvgl.h>
#include "util.h"

struct battery_status_state {
    uint8_t level;
#if IS_ENABLED(CONFIG_USB_DEVICE_STACK)
    bool usb_present;
#endif
};
void draw_battery_status(lv_obj_t *canvas, const struct status_state *state);
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_SMART_BATTERY)
void animation_smart_battery_on(lv_obj_t *canvas);
void animation_smart_battery_off(lv_obj_t *canvas);
#endif
