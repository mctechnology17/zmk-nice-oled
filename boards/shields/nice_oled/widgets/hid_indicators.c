/*
 * Copyright (c) 2024 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/bluetooth/services/bas.h>
#include <zephyr/kernel.h>

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#include <zmk/display.h>
#include <zmk/event_manager.h>
#include <zmk/events/hid_indicators_changed.h>
#include "hid_indicators.h"

static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);

#define LED_NLCK 0x01
#define LED_CLCK 0x02
#define LED_SLCK 0x04

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_HID_INDICATORS_BONGO_CAT)
LV_IMG_DECLARE(bongo_cat_double_tap2_03);
LV_IMG_DECLARE(bongo_cat_double_tap1_04);
const lv_img_dsc_t *hid_anim_imgs[] = {&bongo_cat_double_tap2_03, &bongo_cat_double_tap1_04};
#define HID_HAS_ANIMATION 1

#elif IS_ENABLED(CONFIG_NICE_OLED_WIDGET_HID_INDICATORS_LUNA)
LV_IMG_DECLARE(dog_bark1_90);
LV_IMG_DECLARE(dog_bark2_90);
const lv_img_dsc_t *hid_anim_imgs[] = {&dog_bark1_90, &dog_bark2_90};
#define HID_HAS_ANIMATION 1

#else
#define HID_HAS_ANIMATION 0
#endif

#if HID_HAS_ANIMATION
static lv_obj_t *hid_anim = NULL; // Variable estática para almacenar el objeto animado
#endif

struct hid_indicators_state {
    uint8_t hid_indicators;
};

static void set_hid_indicators(lv_obj_t *label, struct hid_indicators_state state) {
#if HID_HAS_ANIMATION

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_HID_INDICATORS_LUNA_ANIMATION_MS) ||                        \
    IS_ENABLED(CONFIG_NICE_OLED_WIDGET_HID_INDICATORS_BONGO_CAT_ANIMATION_MS)
#define SET_HID_INDICATORS_MS 300
#else
#define SET_HID_INDICATORS_MS 300
#endif

    // Construir el texto según los indicadores activos.
    // Se agregan las letras correspondientes si cada indicador está activo.

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_HID_INDICATORS_LUNA_ONLY_CAPSLOCK) ||                       \
    IS_ENABLED(CONFIG_NICE_OLED_WIDGET_HID_INDICATORS_BONGO_CAT_ONLY_CAPSLOCK)
    if (state.hid_indicators & LED_CLCK)
#else
    if (state.hid_indicators & (LED_CLCK | LED_NLCK | LED_SLCK))
#endif
    {
        lv_label_set_text(label, "");
        if (!hid_anim) { // Si no existe aún, creamos la animación

            hid_anim = lv_animimg_create(label);
            lv_obj_center(hid_anim);

            lv_animimg_set_src(hid_anim, (const void **)hid_anim_imgs, 2);
            lv_animimg_set_duration(hid_anim, SET_HID_INDICATORS_MS);
            lv_animimg_set_repeat_count(hid_anim, LV_ANIM_REPEAT_INFINITE);
            lv_animimg_start(hid_anim);
            lv_obj_align(hid_anim, LV_ALIGN_TOP_LEFT,
                         CONFIG_NICE_OLED_WIDGET_HID_INDICATORS_CUSTOM_X,
                         CONFIG_NICE_OLED_WIDGET_HID_INDICATORS_CUSTOM_Y);
        }
    } else {
        // Si LED_CLCK no está activo, y se había creado la animación, la eliminamos
        if (hid_anim) {
            lv_obj_del(hid_anim);
            hid_anim = NULL;
        }
        lv_label_set_text(label, "");
    }

#else
    // HID_HAS_ANIMATION = 0: No animation enabled
    // Widget remains functional but displays nothing visually
    lv_label_set_text(label, "");
#endif
}

void hid_indicators_update_cb(struct hid_indicators_state state) {
    struct zmk_widget_hid_indicators *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) { set_hid_indicators(widget->obj, state); }
}

static struct hid_indicators_state hid_indicators_get_state(const zmk_event_t *eh) {
    struct zmk_hid_indicators_changed *ev = as_zmk_hid_indicators_changed(eh);
    return (struct hid_indicators_state){
        .hid_indicators = ev->indicators,
    };
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_hid_indicators, struct hid_indicators_state,
                            hid_indicators_update_cb, hid_indicators_get_state)

ZMK_SUBSCRIPTION(widget_hid_indicators, zmk_hid_indicators_changed);

int zmk_widget_hid_indicators_init(struct zmk_widget_hid_indicators *widget, lv_obj_t *parent) {
    widget->obj = lv_label_create(parent);

    sys_slist_append(&widgets, &widget->node);

    widget_hid_indicators_init();

    return 0;
}

lv_obj_t *zmk_widget_hid_indicators_obj(struct zmk_widget_hid_indicators *widget) {
    return widget->obj;
}
