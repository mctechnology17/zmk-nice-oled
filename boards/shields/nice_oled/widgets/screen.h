#pragma once
#ifndef SCREEN_H_
#define SCREEN_H_

#include "util.h"
#include <lvgl.h>
#include <zephyr/kernel.h>

struct zmk_widget_screen {
    sys_snode_t node;
    lv_obj_t *obj;
    lv_color_t cbuf[CANVAS_HEIGHT * CANVAS_HEIGHT];
    struct status_state state;
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_SCROLL)
    lv_obj_t *media_canvas;
    lv_color_t media_cbuf[32 * 32];
    int16_t media_scroll_offset;
    uint8_t media_scroll_phase;
    uint16_t media_scroll_pause_ticks;
    lv_timer_t *media_scroll_timer;
    int64_t media_position_ts;
#endif
};

// TODO: batt
struct zmk_widget_battery_status {
    sys_snode_t node;
    lv_obj_t *obj;
};

int zmk_widget_battery_status_init(struct zmk_widget_battery_status *widget, lv_obj_t *parent);
lv_obj_t *zmk_widget_battery_status_obj(struct zmk_widget_battery_status *widget);
// TODO: batt END

int zmk_widget_screen_init(struct zmk_widget_screen *widget, lv_obj_t *parent);
lv_obj_t *zmk_widget_screen_obj(struct zmk_widget_screen *widget);
#endif
