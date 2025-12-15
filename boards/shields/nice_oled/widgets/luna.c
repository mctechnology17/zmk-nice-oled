/*
 * Copyright (c) 2024 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/kernel.h>
#include <zephyr/bluetooth/services/bas.h>

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#include <zmk/display.h>
#include <zmk/event_manager.h>
#include <zmk/events/wpm_state_changed.h>
#include <zmk/wpm.h>

#include "luna.h"

#define SRC(array) (const void **)array, sizeof(array) / sizeof(lv_img_dsc_t *)

static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);

LV_IMG_DECLARE(dog_sit1_90);
LV_IMG_DECLARE(dog_sit2_90);
LV_IMG_DECLARE(dog_walk1_90);
LV_IMG_DECLARE(dog_walk2_90);
LV_IMG_DECLARE(dog_run1_90);
LV_IMG_DECLARE(dog_run2_90);
LV_IMG_DECLARE(dog_sneak1_90);
LV_IMG_DECLARE(dog_sneak2_90);

// #define ANIMATION_SPEED_IDLE 10000
#define ANIMATION_SPEED_IDLE 960
const lv_img_dsc_t *idle_imgs[] = {
    &dog_sit1_90,
    &dog_sit2_90,
};

// #define ANIMATION_SPEED_SLOW 2000
#define ANIMATION_SPEED_SLOW 200
const lv_img_dsc_t *slow_imgs[] = {
    &dog_walk1_90,
    &dog_walk2_90,
};

// #define ANIMATION_SPEED_MID 500
#define ANIMATION_SPEED_MID 200
const lv_img_dsc_t *mid_imgs[] = {
    &dog_walk1_90,
    &dog_walk2_90,
};

#define ANIMATION_SPEED_FAST 200
const lv_img_dsc_t *fast_imgs[] = {
    &dog_run1_90,
    &dog_run2_90,
};

struct luna_wpm_status_state {
    uint8_t wpm;
};

enum anim_state {
    anim_state_none,
    anim_state_idle,
    anim_state_slow,
    anim_state_mid,
    anim_state_fast
} current_anim_state;

static void set_animation(lv_obj_t *animing, struct luna_wpm_status_state state) {

    if (state.wpm < 15) { // def: 5
        if (current_anim_state != anim_state_idle) {
            lv_animimg_set_src(animing, SRC(idle_imgs));
            lv_animimg_set_duration(animing, ANIMATION_SPEED_IDLE);
            lv_animimg_set_repeat_count(animing, LV_ANIM_REPEAT_INFINITE);
            lv_animimg_start(animing);
            current_anim_state = anim_state_idle;
        }
    } else if (state.wpm < 30) {
        if (current_anim_state != anim_state_slow) {
            lv_animimg_set_src(animing, SRC(slow_imgs));
            lv_animimg_set_duration(animing, ANIMATION_SPEED_SLOW);
            lv_animimg_set_repeat_count(animing, LV_ANIM_REPEAT_INFINITE);
            lv_animimg_start(animing);
            current_anim_state = anim_state_slow;
        }
    } else if (state.wpm < 70) {
        if (current_anim_state != anim_state_mid) {
            lv_animimg_set_src(animing, SRC(mid_imgs));
            lv_animimg_set_duration(animing, ANIMATION_SPEED_MID);
            lv_animimg_set_repeat_count(animing, LV_ANIM_REPEAT_INFINITE);
            lv_animimg_start(animing);
            current_anim_state = anim_state_mid;
        }
    } else {
        if (current_anim_state != anim_state_fast) {
            lv_animimg_set_src(animing, SRC(fast_imgs));
            lv_animimg_set_duration(animing, ANIMATION_SPEED_FAST);
            lv_animimg_set_repeat_count(animing, LV_ANIM_REPEAT_INFINITE);
            lv_animimg_start(animing);
            current_anim_state = anim_state_fast;
        }
    }
}

struct luna_wpm_status_state luna_wpm_status_get_state(const zmk_event_t *eh) {
    struct zmk_wpm_state_changed *ev = as_zmk_wpm_state_changed(eh);
    return (struct luna_wpm_status_state){.wpm = ev->state};
};

void luna_wpm_status_update_cb(struct luna_wpm_status_state state) {
    struct zmk_widget_luna *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) { set_animation(widget->obj, state); }
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_luna, struct luna_wpm_status_state, luna_wpm_status_update_cb,
                            luna_wpm_status_get_state)

ZMK_SUBSCRIPTION(widget_luna, zmk_wpm_state_changed);

int zmk_widget_luna_init(struct zmk_widget_luna *widget, lv_obj_t *parent) {
    widget->obj = lv_animimg_create(parent);
    lv_obj_center(widget->obj);

    sys_slist_append(&widgets, &widget->node);

    widget_luna_init();

    return 0;
}

lv_obj_t *zmk_widget_luna_obj(struct zmk_widget_luna *widget) { return widget->obj; }
