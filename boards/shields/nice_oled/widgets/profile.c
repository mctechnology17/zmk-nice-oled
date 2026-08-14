#include <zephyr/kernel.h>
#include "profile.h"
#include <fonts.h>
#include <stdio.h>

LV_IMG_DECLARE(profiles);

static lv_obj_t *profile_text;
static lv_obj_t *profile_img;
static lv_obj_t *active_rect;

void draw_profile_status(lv_obj_t *canvas, const struct status_state *state) {
    lv_obj_t *parent = lv_obj_get_parent(canvas);

    char text[14] = {};
    snprintf(text, sizeof(text), "%d", state->active_profile_index + 1);

    if (profile_text == NULL) {
        profile_text = lv_label_create(parent);
        lv_obj_set_style_text_font(profile_text, &pixel_operator_mono_8, 0);
    }
    lv_label_set_text(profile_text, text);
    lv_obj_set_pos(profile_text, CONFIG_NICE_OLED_WIDGET_PROFILE_TEXT_CUSTOM_X, CONFIG_NICE_OLED_WIDGET_PROFILE_TEXT_CUSTOM_Y);

    if (profile_img == NULL) {
        profile_img = lv_image_create(parent);
        lv_image_set_src(profile_img, &profiles);
    }
    lv_obj_set_pos(profile_img, CONFIG_NICE_OLED_WIDGET_PROFILE_CUSTOM_X, CONFIG_NICE_OLED_WIDGET_PROFILE_CUSTOM_Y);

    if (active_rect == NULL) {
        active_rect = lv_obj_create(parent);
        lv_obj_set_style_bg_color(active_rect, LVGL_FOREGROUND, 0);
        lv_obj_set_style_bg_opa(active_rect, LV_OPA_COVER, 0);
        lv_obj_set_style_border_width(active_rect, 0, 0);
        lv_obj_set_size(active_rect, 3, 3);
    }
    lv_obj_set_pos(active_rect, CONFIG_NICE_OLED_WIDGET_PROFILE_CUSTOM_X + state->active_profile_index * 7,
                   CONFIG_NICE_OLED_WIDGET_PROFILE_CUSTOM_Y);
}
