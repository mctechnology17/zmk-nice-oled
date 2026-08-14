#include "output.h"
#include <fonts.h>
#include <zephyr/kernel.h>

LV_IMG_DECLARE(bt_no_signal);
LV_IMG_DECLARE(bt_unbonded);
LV_IMG_DECLARE(bt);
LV_IMG_DECLARE(usb);

static lv_obj_t *output_img;

void draw_output_status(lv_obj_t *canvas, const struct status_state *state) {
    lv_obj_t *parent = canvas;

    const lv_image_dsc_t *src;
    lv_coord_t x;
    lv_coord_t y;

#if !IS_ENABLED(CONFIG_ZMK_SPLIT) || IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
    switch (state->selected_endpoint.transport) {
    case ZMK_TRANSPORT_USB:
        src = &usb;
        x = CONFIG_NICE_OLED_WIDGET_OUTPUT_USB_CUSTOM_X;
        y = CONFIG_NICE_OLED_WIDGET_OUTPUT_USB_CUSTOM_Y;
        break;

    case ZMK_TRANSPORT_BLE:
    default:
        if (state->active_profile_bonded) {
            if (state->active_profile_connected) {
                src = &bt;
            } else {
                src = &bt_no_signal;
            }
            x = CONFIG_NICE_OLED_WIDGET_OUTPUT_BT_CUSTOM_X;
            y = CONFIG_NICE_OLED_WIDGET_OUTPUT_BT_CUSTOM_Y;
        } else {
            src = &bt_unbonded;
            x = CONFIG_NICE_OLED_WIDGET_OUTPUT_BT_UNBONDED_CUSTOM_X;
            y = CONFIG_NICE_OLED_WIDGET_OUTPUT_BT_UNBONDED_CUSTOM_Y;
        }
        break;
    }
#else
    if (state->connected) {
        src = &bt;
    } else {
        src = &bt_no_signal;
    }
    x = CONFIG_NICE_OLED_WIDGET_OUTPUT_BT_CUSTOM_X;
    y = CONFIG_NICE_OLED_WIDGET_OUTPUT_BT_CUSTOM_Y;
#endif

    if (output_img == NULL) {
        output_img = lv_image_create(parent);
    }
    lv_image_set_src(output_img, src);
    lv_obj_set_pos(output_img, x, y);
}
