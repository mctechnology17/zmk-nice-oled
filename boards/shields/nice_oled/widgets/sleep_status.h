/*
 * Copyright (c) 2020 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <lvgl.h>
#include <zephyr/kernel.h>

struct zmk_widget_sleep_status {
    sys_snode_t node;
    lv_obj_t *obj;
    lv_obj_t *art;
};

int zmk_widget_sleep_status_init(struct zmk_widget_sleep_status *widget, lv_obj_t *parent);
lv_obj_t *zmk_widget_sleep_status_obj(struct zmk_widget_sleep_status *widget);