/*
 * Copyright (c) 2021 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */
#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#include <drivers/behavior.h>
#include <zmk/display.h>
#include <zmk/behavior.h>
#include <zmk/event_manager.h>
#include <zmk/events/position_state_changed.h>
#include <zmk/split/central.h>
#include <lvgl.h>
#include "responsive_bongo_cat.h"

#define RESPONSIVE_BONGO_CAT_SYNC_BEHAVIOR_NAME "rbcsync"
#define RESPONSIVE_BONGO_CAT_IDLE_CHECK_PERIOD_MS 250

struct zmk_responsive_bongo_cat_activity {
    int64_t timestamp;
};

ZMK_EVENT_DECLARE(zmk_responsive_bongo_cat_activity);
ZMK_EVENT_IMPL(zmk_responsive_bongo_cat_activity);

static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);
static lv_timer_t *idle_check_timer = NULL;

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RESPONSIVE_BONGO_CAT_VERTICAL)
LV_IMG_DECLARE(bongo_cat_both1_open_90);
LV_IMG_DECLARE(bongo_cat_both1_90);
LV_IMG_DECLARE(bongo_cat_right2_90);
LV_IMG_DECLARE(bongo_cat_left2_90);

static const void *idle_image = &bongo_cat_both1_open_90;
static const void *tap_images[] = {&bongo_cat_right2_90, &bongo_cat_left2_90};
#else
LV_IMG_DECLARE(bongo_cat_double_tap1_06);
LV_IMG_DECLARE(bongo_cat_tap1_03);
LV_IMG_DECLARE(bongo_cat_tap2_03);

static const void *idle_image = &bongo_cat_double_tap1_06;
static const void *tap_images[] = {&bongo_cat_tap1_03, &bongo_cat_tap2_03};
#endif

#define TAP_FRAMES 2

struct responsive_bongo_cat_state {
    bool activity;
    uint32_t last_tap;
    uint32_t activity_count;
    bool is_idle;
    uint8_t current_frame;
};

static struct responsive_bongo_cat_state current_state = {
    .activity = false,
    .last_tap = 0,
    .activity_count = 0,
    .is_idle = true,
    .current_frame = 0,
};

static void check_idle_timeout(lv_timer_t *timer) {
    uint32_t now = k_uptime_get_32();
    uint32_t time_since_last_tap = now - current_state.last_tap;

    if (time_since_last_tap < CONFIG_NICE_OLED_WIDGET_RESPONSIVE_BONGO_CAT_IDLE_TIMEOUT_MS ||
        current_state.is_idle) {
        return;
    }

    current_state.is_idle = true;
    current_state.current_frame = 0;

    struct zmk_widget_responsive_bongo_cat *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) {
        if (widget->obj != NULL) {
            lv_img_set_src(widget->obj, idle_image);
        }
    }
}

static void play_tap_animation(lv_obj_t *obj) {
    current_state.is_idle = false;
    lv_img_set_src(obj, tap_images[current_state.current_frame]);
    current_state.current_frame = (current_state.current_frame + 1) % TAP_FRAMES;
}

static void update_responsive_bongo_cat_anim(struct zmk_widget_responsive_bongo_cat *widget,
                                             struct responsive_bongo_cat_state state) {
    if (!widget || !widget->obj) {
        return;
    }

    if (state.activity) {
        play_tap_animation(widget->obj);
    }
}

static struct responsive_bongo_cat_state responsive_bongo_cat_get_state(const zmk_event_t *eh) {
    const struct zmk_position_state_changed *position_ev = as_zmk_position_state_changed(eh);
    const struct zmk_responsive_bongo_cat_activity *activity_ev =
        as_zmk_responsive_bongo_cat_activity(eh);

    if ((position_ev != NULL && position_ev->state) || activity_ev != NULL) {
        current_state.activity = true;
        current_state.last_tap = k_uptime_get_32();
        current_state.activity_count++;
    } else {
        current_state.activity = false;
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
ZMK_SUBSCRIPTION(widget_responsive_bongo_cat, zmk_position_state_changed);
ZMK_SUBSCRIPTION(widget_responsive_bongo_cat, zmk_responsive_bongo_cat_activity);

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RESPONSIVE_BONGO_CAT_SYNC_CENTRAL)
static int64_t last_synced_activity_at;

static int responsive_bongo_cat_sync_central_listener(const zmk_event_t *eh) {
    const struct zmk_position_state_changed *ev = as_zmk_position_state_changed(eh);

    if (ev == NULL || !ev->state) {
        return ZMK_EV_EVENT_BUBBLE;
    }

    int64_t now = k_uptime_get();
    if ((now - last_synced_activity_at) <
        CONFIG_NICE_OLED_WIDGET_RESPONSIVE_BONGO_CAT_SYNC_INTERVAL_MS) {
        return ZMK_EV_EVENT_BUBBLE;
    }
    last_synced_activity_at = now;

    struct zmk_behavior_binding binding = {
        .behavior_dev = RESPONSIVE_BONGO_CAT_SYNC_BEHAVIOR_NAME,
        .param1 = 1,
        .param2 = 0,
    };
    struct zmk_behavior_binding_event event = {
        .position = 0,
        .timestamp = now,
        .source = ZMK_POSITION_STATE_CHANGE_SOURCE_LOCAL,
    };

    for (uint8_t source = 0; source < ZMK_SPLIT_CENTRAL_PERIPHERAL_COUNT; source++) {
        int err = zmk_split_central_invoke_behavior(source, &binding, event, true);
        if (err) {
            LOG_DBG("Failed to sync responsive bongo cat activity to peripheral %d: %d", source,
                    err);
        }
    }

    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(responsive_bongo_cat_sync_central, responsive_bongo_cat_sync_central_listener);
ZMK_SUBSCRIPTION(responsive_bongo_cat_sync_central, zmk_position_state_changed);
#endif

#if IS_ENABLED(CONFIG_ZMK_SPLIT) && !IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
static int behavior_responsive_bongo_cat_sync_init(const struct device *dev) { return 0; }

static int on_responsive_bongo_cat_sync_pressed(struct zmk_behavior_binding *binding,
                                                struct zmk_behavior_binding_event event) {
    raise_zmk_responsive_bongo_cat_activity(
        (struct zmk_responsive_bongo_cat_activity){.timestamp = k_uptime_get()});
    return ZMK_BEHAVIOR_OPAQUE;
}

static int on_responsive_bongo_cat_sync_released(struct zmk_behavior_binding *binding,
                                                 struct zmk_behavior_binding_event event) {
    return ZMK_BEHAVIOR_OPAQUE;
}

static const struct behavior_driver_api behavior_responsive_bongo_cat_sync_driver_api = {
    .binding_pressed = on_responsive_bongo_cat_sync_pressed,
    .binding_released = on_responsive_bongo_cat_sync_released,
};

DEVICE_DEFINE(behavior_responsive_bongo_cat_sync, RESPONSIVE_BONGO_CAT_SYNC_BEHAVIOR_NAME,
              behavior_responsive_bongo_cat_sync_init, NULL, NULL, NULL, POST_KERNEL,
              CONFIG_KERNEL_INIT_PRIORITY_DEFAULT, &behavior_responsive_bongo_cat_sync_driver_api);

static const STRUCT_SECTION_ITERABLE(zmk_behavior_ref, zmk_behavior_responsive_bongo_cat_sync_ref) =
    {
        .device = DEVICE_GET(behavior_responsive_bongo_cat_sync),
        .metadata = {},
};
#endif

int zmk_widget_responsive_bongo_cat_init(struct zmk_widget_responsive_bongo_cat *widget,
                                         lv_obj_t *parent) {
    widget->obj = lv_img_create(parent);
    lv_img_set_src(widget->obj, idle_image);

    if (idle_check_timer == NULL) {
        idle_check_timer =
            lv_timer_create(check_idle_timeout, RESPONSIVE_BONGO_CAT_IDLE_CHECK_PERIOD_MS, NULL);
    }

    sys_slist_append(&widgets, &widget->node);

    widget_responsive_bongo_cat_init();

    return 0;
}

lv_obj_t *zmk_widget_responsive_bongo_cat_obj(struct zmk_widget_responsive_bongo_cat *widget) {
    return widget->obj;
}
