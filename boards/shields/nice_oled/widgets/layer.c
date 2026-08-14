#include "layer.h"
#include <fonts.h>
#include <zephyr/kernel.h>

static lv_obj_t *layer_label;

void draw_layer_status(lv_obj_t *canvas, const struct status_state *state) {
    lv_obj_t *parent = lv_obj_get_parent(canvas);

    char text[10] = {};

    if (state->layer_label == NULL) {
        sprintf(text, "Layer %i", state->layer_index);
    } else {
        strncpy(text, state->layer_label, 9);
        to_uppercase(text);
    }

    if (layer_label == NULL) {
        layer_label = lv_label_create(parent);
        lv_obj_set_style_text_font(layer_label, &pixel_operator_mono_8, 0);
        lv_obj_set_style_text_color(layer_label, lv_color_white(), 0);
    }
    lv_label_set_text(layer_label, text);
    lv_obj_set_pos(layer_label, CONFIG_NICE_OLED_WIDGET_LAYER_CUSTOM_X, CONFIG_NICE_OLED_WIDGET_LAYER_CUSTOM_Y);
}
