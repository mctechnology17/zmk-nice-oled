/*
 * ========================================
 * FileName: street.h
 * Date: 19.November.2026
 * Author: Marcos Chow Castro
 * Email: mctechnology170318@gmail.com
 * GitHub: https://github.com/mctechnology17
 * Brief: street character widget of zmk-nice-oled.
 * =========================================
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <lvgl.h>
#include <zephyr/kernel.h>

struct zmk_widget_street {
    sys_snode_t node;
    lv_obj_t *obj;
};

int zmk_widget_street_init(struct zmk_widget_street *widget, lv_obj_t *parent);
lv_obj_t *zmk_widget_street_obj(struct zmk_widget_street *widget);

/*
 * For the behavior &street. param is one of the STREET_* values of
 * <behaviors/street.dtsi>; pressed tells a key press from its release.
 */
void zmk_widget_street_key(uint8_t param, bool pressed);

/* vim: set ft=c fdm=marker: */
