#include <zephyr/kernel.h>
#include "profile.h"
#if !IS_ENABLED(CONFIG_NICE_EPAPER_ON)
// use custom_fonts.h only for the draw_active_profile_text function
#include <fonts.h>
#include <stdio.h>
#endif // !IS_ENABLED(CONFIG_NICE_EPAPER_ON)

LV_IMG_DECLARE(profiles);

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_PROFILE_BIG) ||                                            \
    IS_ENABLED(CONFIG_NICE_OLED_WIDGET_PROFILE_PERIPHERAL_BIG)
#define OFFSET_X 0
#define OFFSET_Y 129

LV_IMG_DECLARE(profile);
LV_IMG_DECLARE(profile_active);
#endif

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_STREET_STARS)
#include <zmk/ble.h>

LV_IMG_DECLARE(street_star);
LV_IMG_DECLARE(street_star_active);

/* One star per Bluetooth profile, at most 5 (the ZMK default), centred in the 68 pixels of the screen. */
#define STREET_STAR_COUNT MIN(ZMK_BLE_PROFILE_COUNT, 5)
#define STREET_STAR_PITCH 13
#endif

#if !IS_ENABLED(CONFIG_NICE_OLED_WIDGET_PROFILE_BIG) &&                                           \
    !IS_ENABLED(CONFIG_NICE_OLED_WIDGET_PROFILE_PERIPHERAL_BIG)
static void draw_inactive_profiles(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_image_dsc_t img_dsc;
    lv_draw_image_dsc_init(&img_dsc);

    canvas_draw_img(canvas, CONFIG_NICE_OLED_WIDGET_PROFILE_CUSTOM_X, CONFIG_NICE_OLED_WIDGET_PROFILE_CUSTOM_Y, &profiles, &img_dsc);
}

static void draw_active_profile(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_rect_dsc_t rect_white_dsc;
    init_rect_dsc(&rect_white_dsc, LVGL_FOREGROUND);

    int offset = state->active_profile_index * 7;

    canvas_draw_rect(canvas, CONFIG_NICE_OLED_WIDGET_PROFILE_CUSTOM_X + offset, CONFIG_NICE_OLED_WIDGET_PROFILE_CUSTOM_Y, 3, 3, &rect_white_dsc);
}
#endif // !IS_ENABLED(CONFIG_NICE_OLED_WIDGET_PROFILE_BIG) &&
       // !IS_ENABLED(CONFIG_NICE_OLED_WIDGET_PROFILE_PERIPHERAL_BIG)

#if !IS_ENABLED(CONFIG_NICE_EPAPER_ON)
static void draw_active_profile_text(lv_obj_t *canvas, const struct status_state *state) {
    // new label_dsc
    lv_draw_label_dsc_t label_dsc;
    init_label_dsc(&label_dsc, LVGL_FOREGROUND, &pixel_operator_mono_8, LV_TEXT_ALIGN_LEFT);

    char text[14] = {};
    snprintf(text, sizeof(text), "%d", state->active_profile_index + 1);

    canvas_draw_text(canvas, CONFIG_NICE_OLED_WIDGET_PROFILE_TEXT_CUSTOM_X, CONFIG_NICE_OLED_WIDGET_PROFILE_TEXT_CUSTOM_Y, 35, &label_dsc, text);
}
#endif // CONFIG_NICE_EPAPER_ON

void draw_profile_status(lv_obj_t *canvas, const struct status_state *state) {
#if !IS_ENABLED(CONFIG_NICE_EPAPER_ON)
    draw_active_profile_text(canvas, state);
#endif // CONFIG_NICE_EPAPER_ON

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_PROFILE_BIG) ||                                            \
    IS_ENABLED(CONFIG_NICE_OLED_WIDGET_PROFILE_PERIPHERAL_BIG)
    lv_draw_image_dsc_t img_dsc;
    lv_draw_image_dsc_init(&img_dsc);

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_STREET_STARS)
    /* Only the star of the active profile is filled. */
    const int start_x = (CANVAS_WIDTH - ((STREET_STAR_COUNT - 1) * STREET_STAR_PITCH + 10)) / 2;

    for (int i = 0; i < STREET_STAR_COUNT; i++) {
        canvas_draw_img(canvas, start_x + (i * STREET_STAR_PITCH), OFFSET_Y + 2,
                        i == state->active_profile_index ? &street_star_active : &street_star,
                        &img_dsc);
    }
#else
    for (int i = 0; i < 5; i++) {
        canvas_draw_img(canvas, OFFSET_X + (i * 14), OFFSET_Y,
                           i == state->active_profile_index ? &profile_active : &profile, &img_dsc);
    }
#endif
#else
    draw_inactive_profiles(canvas, state);
    draw_active_profile(canvas, state);
#endif
}
