#include "battery.h"
#include <fonts.h>
#include <zephyr/kernel.h>

LV_IMG_DECLARE(bolt);

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_SMART_BATTERY)
LV_IMG_DECLARE(crystal_01);
LV_IMG_DECLARE(crystal_02);
LV_IMG_DECLARE(crystal_03);
LV_IMG_DECLARE(crystal_04);
LV_IMG_DECLARE(crystal_05);
LV_IMG_DECLARE(crystal_06);
LV_IMG_DECLARE(crystal_07);
LV_IMG_DECLARE(crystal_08);
LV_IMG_DECLARE(crystal_09);
LV_IMG_DECLARE(crystal_10);
LV_IMG_DECLARE(crystal_11);
LV_IMG_DECLARE(crystal_12);
LV_IMG_DECLARE(crystal_13);
LV_IMG_DECLARE(crystal_14);
LV_IMG_DECLARE(crystal_15);
LV_IMG_DECLARE(crystal_16);

#ifndef SET_ANIMATION_SMART_BATTERY_OFF
#define SET_ANIMATION_SMART_BATTERY_OFF &crystal_01
#endif

const lv_image_dsc_t *crystal_imgs_test[] = {
    &crystal_01, &crystal_02, &crystal_03, &crystal_04, &crystal_05, &crystal_06,
    &crystal_07, &crystal_08, &crystal_09, &crystal_10, &crystal_11, &crystal_12,
    &crystal_13, &crystal_14, &crystal_15, &crystal_16,
};

static lv_obj_t *art = NULL;
static lv_obj_t *art2 = NULL;

void animation_smart_battery_on(lv_obj_t *canvas) {
    if (art2) {
        lv_obj_del(art2);
        art2 = NULL;
    }
    art = lv_animimg_create(canvas);
    lv_obj_center(art);

    lv_animimg_set_src(art, (const void **)crystal_imgs_test, 16);
    lv_animimg_set_duration(art, CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_MS);
    lv_animimg_set_repeat_count(art, LV_ANIM_REPEAT_INFINITE);
    lv_animimg_start(art);
    lv_obj_align(art, LV_ALIGN_TOP_LEFT, CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_CUSTOM_X, CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_CUSTOM_Y);
}

void animation_smart_battery_off(lv_obj_t *canvas) {
    if (art) {
        lv_obj_del(art);
        art = NULL;
    }
    art2 = lv_image_create(canvas);
    lv_image_set_src(art2, SET_ANIMATION_SMART_BATTERY_OFF);
    lv_obj_align(art2, LV_ALIGN_TOP_LEFT, CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_CUSTOM_X, CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_CUSTOM_Y);
}
#endif

static lv_obj_t *battery_label;
static lv_obj_t *bolt_img;

void draw_battery_status(lv_obj_t *canvas, const struct status_state *state) {
    lv_obj_t *parent = lv_obj_get_parent(canvas);

    char text[10] = {};
    sprintf(text, "%i%%", state->battery);

    if (battery_label == NULL) {
        battery_label = lv_label_create(parent);
        lv_obj_set_style_text_font(battery_label, &pixel_operator_mono_8, 0);
        lv_obj_set_style_text_color(battery_label, lv_color_white(), 0);
    }
    lv_label_set_text(battery_label, text);
    lv_obj_set_pos(battery_label, CONFIG_NICE_OLED_WIDGET_BATTERY_CUSTOM_X, CONFIG_NICE_OLED_WIDGET_BATTERY_CUSTOM_Y);

    if (state->charging) {
        if (bolt_img == NULL) {
            bolt_img = lv_image_create(parent);
            lv_image_set_src(bolt_img, &bolt);
        }
        lv_obj_clear_flag(bolt_img, LV_OBJ_FLAG_HIDDEN);
        lv_obj_set_pos(bolt_img, CONFIG_NICE_OLED_WIDGET_BATTERY_CUSTOM_X - 12, CONFIG_NICE_OLED_WIDGET_BATTERY_CUSTOM_Y);
    } else if (bolt_img != NULL) {
        lv_obj_add_flag(bolt_img, LV_OBJ_FLAG_HIDDEN);
    }
}
