#include "battery.h"
#include <fonts.h>
#include <string.h>
#include <zephyr/kernel.h>

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_BATTERY_PERIPHERAL)
#include <zmk/peripheral_status_sync.h>
#endif

LV_IMG_DECLARE(bolt);

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_SMART_BATTERY)

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_SMART_BATTERY_POKEMON)
LV_IMG_DECLARE(pokemon_00);
LV_IMG_DECLARE(pokemon_01);
LV_IMG_DECLARE(pokemon_02);
LV_IMG_DECLARE(pokemon_03);
LV_IMG_DECLARE(pokemon_04);
LV_IMG_DECLARE(pokemon_05);
LV_IMG_DECLARE(pokemon_06);
LV_IMG_DECLARE(pokemon_07);
LV_IMG_DECLARE(pokemon_08);
LV_IMG_DECLARE(pokemon_09);
LV_IMG_DECLARE(pokemon_10);
LV_IMG_DECLARE(pokemon_11);
LV_IMG_DECLARE(pokemon_12);
LV_IMG_DECLARE(pokemon_13);
LV_IMG_DECLARE(pokemon_14);
LV_IMG_DECLARE(pokemon_15);
LV_IMG_DECLARE(pokemon_16);
LV_IMG_DECLARE(pokemon_17);
LV_IMG_DECLARE(pokemon_18);
LV_IMG_DECLARE(pokemon_19);
LV_IMG_DECLARE(pokemon_20);
LV_IMG_DECLARE(pokemon_21);
LV_IMG_DECLARE(pokemon_22);
LV_IMG_DECLARE(pokemon_23);
LV_IMG_DECLARE(pokemon_24);
LV_IMG_DECLARE(pokemon_25);
LV_IMG_DECLARE(pokemon_26);
LV_IMG_DECLARE(pokemon_27);
LV_IMG_DECLARE(pokemon_28);
LV_IMG_DECLARE(pokemon_29);
LV_IMG_DECLARE(pokemon_30);
LV_IMG_DECLARE(pokemon_31);
LV_IMG_DECLARE(pokemon_32);
LV_IMG_DECLARE(pokemon_33);
LV_IMG_DECLARE(pokemon_34);
LV_IMG_DECLARE(pokemon_35);
LV_IMG_DECLARE(pokemon_36);
LV_IMG_DECLARE(pokemon_37);
LV_IMG_DECLARE(pokemon_38);
LV_IMG_DECLARE(pokemon_39);
LV_IMG_DECLARE(pokemon_40);
LV_IMG_DECLARE(pokemon_41);
LV_IMG_DECLARE(pokemon_42);
LV_IMG_DECLARE(pokemon_43);
LV_IMG_DECLARE(pokemon_44);
LV_IMG_DECLARE(pokemon_45);
LV_IMG_DECLARE(pokemon_46);
LV_IMG_DECLARE(pokemon_47);

static const lv_img_dsc_t *smart_battery_anim_imgs[] = {
    &pokemon_00, &pokemon_01, &pokemon_02, &pokemon_03, &pokemon_04, &pokemon_05,
    &pokemon_06, &pokemon_07, &pokemon_08, &pokemon_09, &pokemon_10, &pokemon_11,
    &pokemon_12, &pokemon_13, &pokemon_14, &pokemon_15, &pokemon_16, &pokemon_17,
    &pokemon_18, &pokemon_19, &pokemon_20, &pokemon_21, &pokemon_22, &pokemon_23,
    &pokemon_24, &pokemon_25, &pokemon_26, &pokemon_27, &pokemon_28, &pokemon_29,
    &pokemon_30, &pokemon_31, &pokemon_32, &pokemon_33, &pokemon_34, &pokemon_35,
    &pokemon_36, &pokemon_37, &pokemon_38, &pokemon_39, &pokemon_40, &pokemon_41,
    &pokemon_42, &pokemon_43, &pokemon_44, &pokemon_45, &pokemon_46, &pokemon_47};
#define SMART_BATTERY_ANIM_FRAME_COUNT 48
#define SMART_BATTERY_OFF_IMAGE &pokemon_00

#elif IS_ENABLED(CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_SMART_BATTERY_HEAD)
LV_IMG_DECLARE(head_00);
LV_IMG_DECLARE(head_01);
LV_IMG_DECLARE(head_02);
LV_IMG_DECLARE(head_03);
LV_IMG_DECLARE(head_04);
LV_IMG_DECLARE(head_05);
LV_IMG_DECLARE(head_06);
LV_IMG_DECLARE(head_07);
LV_IMG_DECLARE(head_08);
LV_IMG_DECLARE(head_09);
LV_IMG_DECLARE(head_10);
LV_IMG_DECLARE(head_11);
LV_IMG_DECLARE(head_12);
LV_IMG_DECLARE(head_13);
LV_IMG_DECLARE(head_14);
LV_IMG_DECLARE(head_15);

static const lv_img_dsc_t *smart_battery_anim_imgs[] = {
    &head_00, &head_01, &head_02, &head_03, &head_04, &head_05, &head_06, &head_07,
    &head_08, &head_09, &head_10, &head_11, &head_12, &head_13, &head_14, &head_15};
#define SMART_BATTERY_ANIM_FRAME_COUNT 16
#define SMART_BATTERY_OFF_IMAGE &head_00

#elif IS_ENABLED(CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_SMART_BATTERY_SPACEMAN)
LV_IMG_DECLARE(spaceman_00);
LV_IMG_DECLARE(spaceman_01);
LV_IMG_DECLARE(spaceman_02);
LV_IMG_DECLARE(spaceman_03);
LV_IMG_DECLARE(spaceman_04);
LV_IMG_DECLARE(spaceman_05);
LV_IMG_DECLARE(spaceman_06);
LV_IMG_DECLARE(spaceman_07);
LV_IMG_DECLARE(spaceman_08);
LV_IMG_DECLARE(spaceman_09);
LV_IMG_DECLARE(spaceman_10);
LV_IMG_DECLARE(spaceman_11);
LV_IMG_DECLARE(spaceman_12);
LV_IMG_DECLARE(spaceman_13);
LV_IMG_DECLARE(spaceman_14);
LV_IMG_DECLARE(spaceman_15);
LV_IMG_DECLARE(spaceman_16);
LV_IMG_DECLARE(spaceman_17);
LV_IMG_DECLARE(spaceman_18);
LV_IMG_DECLARE(spaceman_19);

static const lv_img_dsc_t *smart_battery_anim_imgs[] = {
    &spaceman_00, &spaceman_01, &spaceman_02, &spaceman_03, &spaceman_04,
    &spaceman_05, &spaceman_06, &spaceman_07, &spaceman_08, &spaceman_09,
    &spaceman_10, &spaceman_11, &spaceman_12, &spaceman_13, &spaceman_14,
    &spaceman_15, &spaceman_16, &spaceman_17, &spaceman_18, &spaceman_19};
#define SMART_BATTERY_ANIM_FRAME_COUNT 20
#define SMART_BATTERY_OFF_IMAGE &spaceman_00

#elif IS_ENABLED(CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_SMART_BATTERY_CAT)
LV_IMG_DECLARE(cat_0);
LV_IMG_DECLARE(cat_1);
LV_IMG_DECLARE(cat_2);
LV_IMG_DECLARE(cat_3);
LV_IMG_DECLARE(cat_4);
LV_IMG_DECLARE(cat_5);
LV_IMG_DECLARE(cat_6);
LV_IMG_DECLARE(cat_7);

static const lv_img_dsc_t *smart_battery_anim_imgs[] = {&cat_0, &cat_1, &cat_2, &cat_3,
                                                        &cat_4, &cat_5, &cat_6, &cat_7};
#define SMART_BATTERY_ANIM_FRAME_COUNT 8
#define SMART_BATTERY_OFF_IMAGE &cat_0

#else
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

static const lv_img_dsc_t *smart_battery_anim_imgs[] = {
    &crystal_01, &crystal_02, &crystal_03, &crystal_04, &crystal_05, &crystal_06,
    &crystal_07, &crystal_08, &crystal_09, &crystal_10, &crystal_11, &crystal_12,
    &crystal_13, &crystal_14, &crystal_15, &crystal_16,
};
#define SMART_BATTERY_ANIM_FRAME_COUNT 16
#define SMART_BATTERY_OFF_IMAGE &crystal_01
#endif

static lv_obj_t *art = NULL;
static lv_obj_t *art2 = NULL;
static bool smart_battery_anim_running = false;

static void animation_smart_battery_create_on(lv_obj_t *canvas) {
    art = lv_animimg_create(canvas);
    lv_obj_center(art);

    lv_animimg_set_src(art, (const void **)smart_battery_anim_imgs,
                       SMART_BATTERY_ANIM_FRAME_COUNT);
    lv_animimg_set_duration(art, CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_MS);
    lv_animimg_set_repeat_count(art, LV_ANIM_REPEAT_INFINITE);
    lv_obj_align(art, LV_ALIGN_TOP_LEFT, CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_CUSTOM_X,
                 CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_CUSTOM_Y);
}

static void animation_smart_battery_create_off(lv_obj_t *canvas) {
    art2 = lv_img_create(canvas);
    lv_img_set_src(art2, SMART_BATTERY_OFF_IMAGE);
    lv_obj_align(art2, LV_ALIGN_TOP_LEFT, CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_CUSTOM_X,
                 CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_CUSTOM_Y);
}

void animation_smart_battery_on(lv_obj_t *canvas) {
    if (art == NULL) {
        animation_smart_battery_create_on(canvas);
    }
    if (art2 == NULL) {
        animation_smart_battery_create_off(canvas);
    }

    lv_obj_add_flag(art2, LV_OBJ_FLAG_HIDDEN);
    lv_obj_remove_flag(art, LV_OBJ_FLAG_HIDDEN);

    if (!smart_battery_anim_running) {
        lv_animimg_start(art);
        smart_battery_anim_running = true;
    }
}

void animation_smart_battery_off(lv_obj_t *canvas) {
    if (art == NULL) {
        animation_smart_battery_create_on(canvas);
    }
    if (art2 == NULL) {
        animation_smart_battery_create_off(canvas);
    }

    if (smart_battery_anim_running) {
        lv_anim_delete(art, NULL);
        smart_battery_anim_running = false;
    }

    lv_obj_add_flag(art, LV_OBJ_FLAG_HIDDEN);
    lv_obj_remove_flag(art2, LV_OBJ_FLAG_HIDDEN);
}
#endif

static void draw_level(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_label_dsc_t label_right_dsc;
#if IS_ENABLED(CONFIG_NICE_EPAPER_ON)
    init_label_dsc(&label_right_dsc, LVGL_FOREGROUND, &pixel_operator_mono_16, LV_TEXT_ALIGN_RIGHT);
#else
    init_label_dsc(&label_right_dsc, LVGL_FOREGROUND, &pixel_operator_mono_16, LV_TEXT_ALIGN_LEFT);
#endif // CONFIG_NICE_EPAPER_ON

    char text[10] = {};

    sprintf(text, "%i%%", state->battery);
    // x, y, width, dsc, text
    canvas_draw_text(canvas, CONFIG_NICE_OLED_WIDGET_BATTERY_CUSTOM_X, CONFIG_NICE_OLED_WIDGET_BATTERY_CUSTOM_Y, 42, &label_right_dsc, text);
}

static void draw_charging_level(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_image_dsc_t img_dsc;
    lv_draw_image_dsc_init(&img_dsc);
    lv_draw_label_dsc_t label_right_dsc;
#if IS_ENABLED(CONFIG_NICE_EPAPER_ON)
    init_label_dsc(&label_right_dsc, LVGL_FOREGROUND, &pixel_operator_mono_16, LV_TEXT_ALIGN_RIGHT);
#else
    init_label_dsc(&label_right_dsc, LVGL_FOREGROUND, &pixel_operator_mono_16, LV_TEXT_ALIGN_LEFT);
#endif // CONFIG_NICE_EPAPER_ON

    char text[10] = {};

    sprintf(text, "%i", state->battery);
    canvas_draw_text(canvas, CONFIG_NICE_OLED_WIDGET_BATTERY_CUSTOM_X, CONFIG_NICE_OLED_WIDGET_BATTERY_CUSTOM_Y, 35, &label_right_dsc, text);
#if IS_ENABLED(CONFIG_NICE_EPAPER_ON)
    canvas_draw_img(canvas, CONFIG_NICE_OLED_WIDGET_BATTERY_CUSTOM_X + 36, CONFIG_NICE_OLED_WIDGET_BATTERY_CUSTOM_Y + 2, &bolt, &img_dsc);
#else
    canvas_draw_img(canvas, CONFIG_NICE_OLED_WIDGET_BATTERY_CUSTOM_X + 25, CONFIG_NICE_OLED_WIDGET_BATTERY_CUSTOM_Y, &bolt, &img_dsc);
#endif // CONFIG_NICE_EPAPER_ON
}

#if IS_ENABLED(CONFIG_NICE_EPAPER_ON) &&                                                         \
    IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_BATTERY_PERIPHERAL)
static void draw_central_battery_peripheral(lv_obj_t *canvas, const struct status_state *state,
                                            lv_draw_label_dsc_t *label_dsc,
                                            lv_draw_image_dsc_t *img_dsc) {
    char level_text[8] = {};

    if (state->central_battery_level == ZMK_PERIPHERAL_STATUS_SYNC_BATTERY_UNKNOWN) {
        snprintf(level_text, sizeof(level_text), "--");
    } else {
        snprintf(level_text, sizeof(level_text), "%u", state->central_battery_level);
    }

    canvas_draw_text(canvas, 0, 19, 42, label_dsc, level_text);

    uint8_t suffix_x = (strlen(level_text) * 7) + 3;
    if (state->central_battery_charging) {
        canvas_draw_img(canvas, suffix_x, 21, &bolt, img_dsc);
    } else {
        canvas_draw_text(canvas, suffix_x, 19, 8, label_dsc, "%");
    }
}
#endif

void draw_battery_status(lv_obj_t *canvas, const struct status_state *state) {
#if IS_ENABLED(CONFIG_NICE_EPAPER_ON)
    lv_draw_label_dsc_t label_left_dsc;
    init_label_dsc(&label_left_dsc, LVGL_FOREGROUND, &pixel_operator_mono_16, LV_TEXT_ALIGN_LEFT);
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_BATTERY_PERIPHERAL)
    lv_draw_image_dsc_t img_dsc;
    lv_draw_image_dsc_init(&img_dsc);
    draw_central_battery_peripheral(canvas, state, &label_left_dsc, &img_dsc);
#else
    canvas_draw_text(canvas, 0, 19, 25, &label_left_dsc, "BAT");
#endif
#endif // CONFIG_NICE_EPAPER_ON
    if (state->charging) {
        draw_charging_level(canvas, state);
    } else {
        draw_level(canvas, state);
    }
}
