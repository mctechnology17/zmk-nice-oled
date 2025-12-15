/*
 * Copyright (c) 2020 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#include <zmk/display.h>
#include <zmk/event_manager.h>
#include <zmk/endpoints.h>
#include <zmk/keymap.h>
#include <zmk/events/activity_state_changed.h>
#include "sleep_status.h"

static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);

LV_IMG_DECLARE(vim_32x128);
LV_IMG_DECLARE(vim_68x160);
LV_IMG_DECLARE(sleep_oled);

// TODO: START bootloader
static void hide_and_restore_cb(lv_timer_t *timer) {
    lv_obj_t *art = (lv_obj_t *)timer->user_data;
    if (art) {
// Restaura la imagen original que estaba en el estado activo
#if IS_ENABLED(CONFIG_NICE_EPAPER_ON)
        lv_img_set_src(art, &vim_68x160);
#else
        lv_img_set_src(art, &vim_32x128);
#endif

        // Oculta el objeto, que es el comportamiento por defecto para el estado ACTIVO
        lv_obj_add_flag(art, LV_OBJ_FLAG_HIDDEN);
    }
}
// TODO: END bootloader

static void set_sleep_img(struct zmk_widget_sleep_status *widget,
                          struct zmk_activity_state_changed ev) {
    switch (ev.state) {
        // TODO: START bootloader
    case ZMK_ACTIVITY_ACTIVE:
        LOG_DBG("ACTIVITY EVENT ACTIVE");
        // Muestra temporalmente la imagen sleep_oled
        lv_img_set_src(widget->art, &sleep_oled);
        lv_obj_clear_flag(widget->art, LV_OBJ_FLAG_HIDDEN);
        // Crea un temporizador para ocultarla y restaurar la original después de 3s
        lv_timer_create(hide_and_restore_cb, 3000, widget->art);
        break;
        // TODO: END bootloader
    case ZMK_ACTIVITY_IDLE:
        LOG_DBG("ACTIVITY EVENT IDLE");
#if IS_ENABLED(CONFIG_NICE_PERI_VIEW_SHOW_SLEEP_ART_ON_IDLE)
        lv_obj_clear_flag(widget->art, LV_OBJ_FLAG_HIDDEN);
#else
        lv_obj_add_flag(widget->art, LV_OBJ_FLAG_HIDDEN);
#endif
        break;
    case ZMK_ACTIVITY_SLEEP:
        LOG_DBG("ACTIVITY EVENT SLEEP");
#if IS_ENABLED(CONFIG_NICE_PERI_VIEW_SHOW_SLEEP_ART_ON_SLEEP)
        lv_obj_clear_flag(widget->art, LV_OBJ_FLAG_HIDDEN);
#else
        lv_obj_add_flag(widget->art, LV_OBJ_FLAG_HIDDEN);
#endif
        break;
    default:
        LOG_WRN("Unhandled activity state: %d", ev.state);
    }
}

static void sleep_status_update_cb(struct zmk_activity_state_changed ev) {
    struct zmk_widget_sleep_status *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) { set_sleep_img(widget, ev); }
}

static struct zmk_activity_state_changed sleep_status_get_state(const zmk_event_t *eh) {
    struct zmk_activity_state_changed *ev = as_zmk_activity_state_changed(eh);
    return *ev;
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_sleep_status, struct zmk_activity_state_changed,
                            sleep_status_update_cb, sleep_status_get_state)

ZMK_SUBSCRIPTION(widget_sleep_status, zmk_activity_state_changed);

int zmk_widget_sleep_status_init(struct zmk_widget_sleep_status *widget, lv_obj_t *parent) {
    sys_slist_append(&widgets, &widget->node);
    widget->obj = lv_obj_create(parent);
    lv_obj_set_size(widget->obj, 160, 68);
    widget->art = lv_img_create(widget->obj);
#if IS_ENABLED(CONFIG_NICE_EPAPER_ON)
    lv_img_set_src(widget->art, &vim_68x160);
#else
    // lv_img_set_src(widget->art, &sleep_oled);
    lv_img_set_src(widget->art, &vim_32x128);
#endif
    lv_obj_align(widget->art, LV_ALIGN_TOP_LEFT, 0, 0);

    // lv_obj_add_flag(widget->art, LV_OBJ_FLAG_HIDDEN);

    widget_sleep_status_init();
    return 0;
}

lv_obj_t *zmk_widget_sleep_status_obj(struct zmk_widget_sleep_status *widget) {
    return widget->obj;
}
