/*
 * Copyright (c) 2021 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */
#include <zephyr/kernel.h>
#include <zmk/event_manager.h>
#include <zmk/events/keycode_state_changed.h>
#include <zephyr/logging/log.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#include <zmk/display.h>
#include <lvgl.h>
#include "responsive_bongo_cat.h"

static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);
static lv_anim_t idle_anim;
static lv_timer_t *idle_check_timer = NULL;

/*
LV_IMG_DECLARE(idle_img1);
LV_IMG_DECLARE(idle_img2);
LV_IMG_DECLARE(idle_img3);
LV_IMG_DECLARE(idle_img4);
LV_IMG_DECLARE(idle_img5);
LV_IMG_DECLARE(fast_img1);
LV_IMG_DECLARE(fast_img2);

static const void *idle_images[] = {&idle_img1, &idle_img2, &idle_img3, &idle_img4, &idle_img5};

static const void *tap_images[] = {&fast_img1, &fast_img2};
*/

LV_IMG_DECLARE(bongo_cat_both1_open_90);
LV_IMG_DECLARE(bongo_cat_both1_open_90);
LV_IMG_DECLARE(bongo_cat_both1_open_90);
LV_IMG_DECLARE(bongo_cat_both1_90);
LV_IMG_DECLARE(bongo_cat_both1_90);
LV_IMG_DECLARE(bongo_cat_right2_90);
LV_IMG_DECLARE(bongo_cat_left2_90);

static const void *idle_images[] = {&bongo_cat_both1_open_90, &bongo_cat_both1_open_90,
                                    &bongo_cat_both1_90, &bongo_cat_both1_90,
                                    &bongo_cat_both1_open_90};

static const void *tap_images[] = {&bongo_cat_right2_90, &bongo_cat_left2_90};

#define IDLE_FRAMES 5
#define TAP_FRAMES 2
#define IDLE_ANIM_TIME 1000   // 1 second for full idle cycle
#define IDLE_TIMEOUT_MS 500   // Return to idle after 500ms of no keypresses
#define IDLE_CHECK_PERIOD 100 // Check for idle every 100ms

struct responsive_bongo_cat_state {
    bool key_pressed;
    uint32_t last_tap;
    lv_obj_t *obj; // Store reference to the image object
    bool is_idle;  // Track if we're already in idle animation
};

static struct responsive_bongo_cat_state current_state = {
    .key_pressed = false, .last_tap = 0, .obj = NULL, .is_idle = true};

static void set_idle_frame(void *var, int32_t val) {
    LOG_DBG("BONGO: Idle animation frame: %d", val);
    lv_obj_t *img = (lv_obj_t *)var;
    int frame = val % IDLE_FRAMES;
    lv_img_set_src(img, idle_images[frame]);
}

static void start_idle_animation(lv_obj_t *obj) {
    if (current_state.is_idle) {
        return; // Don't restart if already in idle animation
    }

    LOG_DBG("BONGO: Starting idle animation");
    current_state.is_idle = true;

    lv_anim_init(&idle_anim);
    lv_anim_set_var(&idle_anim, obj);
    lv_anim_set_values(&idle_anim, 0, IDLE_FRAMES - 1);
    lv_anim_set_time(&idle_anim, IDLE_ANIM_TIME);
    lv_anim_set_exec_cb(&idle_anim, set_idle_frame);
    lv_anim_set_repeat_count(&idle_anim, LV_ANIM_REPEAT_INFINITE);
    lv_anim_start(&idle_anim);
}

static void check_idle_timeout(lv_timer_t *timer) {
    uint32_t now = k_uptime_get_32();
    uint32_t time_since_last_tap = now - current_state.last_tap;

    if (time_since_last_tap >= IDLE_TIMEOUT_MS && current_state.obj != NULL &&
        !current_state.is_idle) {
        LOG_DBG("BONGO: Idle timeout reached, starting idle animation");
        start_idle_animation(current_state.obj);
    }
}

static void play_tap_animation(lv_obj_t *obj) {
    LOG_DBG("BONGO: Playing tap animation");
    current_state.is_idle = false;
    lv_anim_del(obj, set_idle_frame); // Stop idle animation if running

    static uint8_t current_frame = 0;
    current_frame = (current_frame + 1) % TAP_FRAMES;
    lv_img_set_src(obj, tap_images[current_frame]);
}

static void update_responsive_bongo_cat_anim(struct zmk_widget_responsive_bongo_cat *widget,
                                             struct responsive_bongo_cat_state state) {
    if (!widget || !widget->obj) {
        LOG_ERR("BONGO: Widget or object is NULL!");
        return;
    }

    current_state.obj = widget->obj; // Update the global state

    if (state.key_pressed) {
        play_tap_animation(widget->obj);
    }
}

static struct responsive_bongo_cat_state responsive_bongo_cat_get_state(const zmk_event_t *eh) {
    const struct zmk_keycode_state_changed *ev = as_zmk_keycode_state_changed(eh);
    if (ev != NULL && ev->state) { // Only update on key press, not release
        current_state.key_pressed = true;
        current_state.last_tap = k_uptime_get_32();
    } else {
        current_state.key_pressed = false;
    }

    return current_state;
}

static void responsive_bongo_cat_update_cb(struct responsive_bongo_cat_state state) {
    struct zmk_widget_responsive_bongo_cat *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) {
        update_responsive_bongo_cat_anim(widget, state);
    }
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_responsive_bongo_cat, struct responsive_bongo_cat_state,
                            responsive_bongo_cat_update_cb, responsive_bongo_cat_get_state)
ZMK_SUBSCRIPTION(widget_responsive_bongo_cat, zmk_keycode_state_changed);

int zmk_widget_responsive_bongo_cat_init(struct zmk_widget_responsive_bongo_cat *widget,
                                         lv_obj_t *parent) {
    widget->obj = lv_img_create(parent);

    // Initialize idle check timer
    if (idle_check_timer == NULL) {
        idle_check_timer = lv_timer_create(check_idle_timeout, IDLE_CHECK_PERIOD, NULL);
    }

    // Start with idle animation
    current_state.obj = widget->obj;
    current_state.is_idle = false; // Set to false so initial animation will start
    start_idle_animation(widget->obj);

    sys_slist_append(&widgets, &widget->node);

    widget_responsive_bongo_cat_init();

    return 0;
}

lv_obj_t *zmk_widget_responsive_bongo_cat_obj(struct zmk_widget_responsive_bongo_cat *widget) {
    return widget->obj;
}
