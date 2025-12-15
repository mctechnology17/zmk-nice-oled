#include "output.h"
// #include "../assets/custom_fonts.h"
#include <fonts.h>
#include <zephyr/kernel.h>

LV_IMG_DECLARE(bt_no_signal);
LV_IMG_DECLARE(bt_unbonded);
LV_IMG_DECLARE(bt);
LV_IMG_DECLARE(usb);

#if !IS_ENABLED(CONFIG_ZMK_SPLIT) || IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
static void draw_usb_connected(lv_obj_t *canvas) {
    lv_draw_img_dsc_t img_dsc;
    lv_draw_img_dsc_init(&img_dsc);

    lv_canvas_draw_img(canvas, CONFIG_NICE_OLED_WIDGET_OUTPUT_USB_CUSTOM_X, CONFIG_NICE_OLED_WIDGET_OUTPUT_USB_CUSTOM_Y, &usb, &img_dsc);
}

static void draw_ble_unbonded(lv_obj_t *canvas) {
    lv_draw_img_dsc_t img_dsc;
    lv_draw_img_dsc_init(&img_dsc);

    lv_canvas_draw_img(canvas, CONFIG_NICE_OLED_WIDGET_OUTPUT_BT_UNBONDED_CUSTOM_X, CONFIG_NICE_OLED_WIDGET_OUTPUT_BT_UNBONDED_CUSTOM_Y, &bt_unbonded, &img_dsc);
}
#endif

static void draw_ble_disconnected(lv_obj_t *canvas) {
    lv_draw_img_dsc_t img_dsc;
    lv_draw_img_dsc_init(&img_dsc);

    lv_canvas_draw_img(canvas, CONFIG_NICE_OLED_WIDGET_OUTPUT_BT_CUSTOM_X, CONFIG_NICE_OLED_WIDGET_OUTPUT_BT_CUSTOM_Y, &bt_no_signal, &img_dsc);
}

static void draw_ble_connected(lv_obj_t *canvas) {
    lv_draw_img_dsc_t img_dsc;
    lv_draw_img_dsc_init(&img_dsc);

    lv_canvas_draw_img(canvas, CONFIG_NICE_OLED_WIDGET_OUTPUT_BT_CUSTOM_X, CONFIG_NICE_OLED_WIDGET_OUTPUT_BT_CUSTOM_Y, &bt, &img_dsc);
}

void draw_output_status(lv_obj_t *canvas, const struct status_state *state) {
#if IS_ENABLED(CONFIG_NICE_EPAPER_ON) &&                                                           \
    !IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_ALL)
    lv_draw_label_dsc_t label_dsc;
    init_label_dsc(&label_dsc, LVGL_FOREGROUND, &pixel_operator_mono_16, LV_TEXT_ALIGN_LEFT);
    lv_canvas_draw_text(canvas, 0, 1, 25, &label_dsc, "SIG");

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_OUTPUT_BACKGROUND)
    lv_draw_rect_dsc_t rect_white_dsc;
    init_rect_dsc(&rect_white_dsc, LVGL_FOREGROUND);
    lv_canvas_draw_rect(canvas, 43, 0, 24, 15, &rect_white_dsc);
#endif

#else

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_OUTPUT_BACKGROUND)
    lv_draw_rect_dsc_t rect_white_dsc;
    init_rect_dsc(&rect_white_dsc, LVGL_FOREGROUND);
    lv_canvas_draw_rect(canvas, -3, 32, 24, 15, &rect_white_dsc);
#endif

#endif // CONFIG_NICE_EPAPER_ON

#if !IS_ENABLED(CONFIG_ZMK_SPLIT) || IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
    switch (state->selected_endpoint.transport) {
    case ZMK_TRANSPORT_USB:
        draw_usb_connected(canvas);
        break;

    case ZMK_TRANSPORT_BLE:
        if (state->active_profile_bonded) {
            if (state->active_profile_connected) {
                draw_ble_connected(canvas);
            } else {
                draw_ble_disconnected(canvas);
            }
        } else {
            draw_ble_unbonded(canvas);
        }
        break;
    }
#else
    if (state->connected) {
        draw_ble_connected(canvas);
    } else {
        draw_ble_disconnected(canvas);
    }
#endif
}
