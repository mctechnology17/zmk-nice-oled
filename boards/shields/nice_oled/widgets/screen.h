#pragma once

#ifndef WPM_STATUS_H
#define WPM_STATUS_H

#include <lvgl.h>
#include <zephyr/kernel.h>
#include "util.h"

struct zmk_widget_screen {
    sys_snode_t node;
    lv_obj_t *obj;
    lv_color_t cbuf[CANVAS_HEIGHT * CANVAS_HEIGHT];
    struct status_state state;
};

int zmk_widget_screen_init(struct zmk_widget_screen *widget, lv_obj_t *parent);
lv_obj_t *zmk_widget_screen_obj(struct zmk_widget_screen *widget);

extern struct wpm_status_state wpm_status_get_state(const zmk_event_t* eh);

#endif /* WPM_STATUS_H */