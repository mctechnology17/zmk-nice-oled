#include <zephyr/kernel.h>
#include "profile.h"
#include <fonts.h>
#include <stdio.h>

static lv_obj_t *profile_text;

void draw_profile_status(lv_obj_t *canvas, const struct status_state *state) {
    lv_obj_t *parent = canvas;

    char text[14] = {};
    snprintf(text, sizeof(text), "%d", state->active_profile_index + 1);

    if (profile_text == NULL) {
        profile_text = lv_label_create(parent);
        lv_obj_set_style_text_font(profile_text, &pixel_operator_mono_8, 0);
        lv_obj_set_style_text_color(profile_text, lv_color_white(), 0);
    }
    lv_label_set_text(profile_text, text);
    lv_obj_set_pos(profile_text, CONFIG_NICE_OLED_WIDGET_PROFILE_TEXT_CUSTOM_X, CONFIG_NICE_OLED_WIDGET_PROFILE_TEXT_CUSTOM_Y);
}
