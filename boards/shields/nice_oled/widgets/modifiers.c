/*
 * Copyright (c) 2024 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/kernel.h>
#include <zephyr/bluetooth/services/bas.h>
#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#include <zmk/display.h>
#include <zmk/event_manager.h>
#include <zmk/events/keycode_state_changed.h>
#include <zmk/hid.h>
#include <dt-bindings/zmk/modifiers.h>

#include "modifiers.h"

struct modifiers_state {
    uint8_t modifiers;
};

static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);

/**
 * Construye el string que representa los modificadores activos.
 * Se utiliza el siguiente mapeo:
 *   - Windows/Command (MOD_LGUI | MOD_RGUI) → "M"
 *   - Alt (MOD_LALT | MOD_RALT)              → "A"
 *   - Control (MOD_LCTL | MOD_RCTL)           → "C"
 *   - Shift (MOD_LSFT | MOD_RSFT)             → "S"
 *
 * @param label Objeto label de LVGL donde se mostrará el string.
 * @param state Estado de los modificadores.
 */
/* DEBUG
static void set_modifiers_text(lv_obj_t *label, struct modifiers_state state) {
    char text[16] = {0};

    if (state.modifiers & (MOD_LGUI | MOD_RGUI)) {
        strcat(text, "M");
    }
    if (state.modifiers & (MOD_LALT | MOD_RALT)) {
        strcat(text, "A");
    }
    if (state.modifiers & (MOD_LCTL | MOD_RCTL)) {
        strcat(text, "C");
    }
    if (state.modifiers & (MOD_LSFT | MOD_RSFT)) {
        strcat(text, "S");
    }

    lv_label_set_text(label, text);
}
*/

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED_SYMBOL_VERTICAL)

LV_IMG_DECLARE(alt_0);
LV_IMG_DECLARE(alt_white_0);
LV_IMG_DECLARE(cmd_0);
LV_IMG_DECLARE(cmd_white_0);
LV_IMG_DECLARE(control_0);
LV_IMG_DECLARE(control_white_0);
LV_IMG_DECLARE(opt_0);
LV_IMG_DECLARE(opt_white_0);
LV_IMG_DECLARE(shift_0);
LV_IMG_DECLARE(shift_white_0);
LV_IMG_DECLARE(win_0);
LV_IMG_DECLARE(win_white_0);

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED_SYMBOL_WINDOWS)

const lv_img_dsc_t *symbol_imgs_alt[] = {&alt_0, &alt_white_0};
const lv_img_dsc_t *symbol_imgs_win[] = {&win_0, &win_white_0};
const lv_img_dsc_t *symbol_imgs_control[] = {&control_0, &control_white_0};
const lv_img_dsc_t *symbol_imgs_shift[] = {&shift_0, &shift_white_0};

#else

const lv_img_dsc_t *symbol_imgs_alt[] = {&opt_0, &opt_white_0};
const lv_img_dsc_t *symbol_imgs_win[] = {&cmd_0, &cmd_white_0};
const lv_img_dsc_t *symbol_imgs_control[] = {&control_0, &control_white_0};
const lv_img_dsc_t *symbol_imgs_shift[] = {&shift_0, &shift_white_0};

#endif
#define MODIFIERS_USE_SYMBOLS 1

#elif IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_BONGO_CAT)

LV_IMG_DECLARE(bongo_cat_double_tap1_06);
LV_IMG_DECLARE(bongo_cat_tap1_03);
LV_IMG_DECLARE(bongo_cat_tap2_03);
LV_IMG_DECLARE(bongo_cat_double_tap2_02);
LV_IMG_DECLARE(bongo_cat_double_tap1_03);

// Idle: sitting bongo cat
const lv_img_dsc_t *bongo_imgs_idle[] = {&bongo_cat_double_tap1_06};
// GUI/Cmd: tap left
const lv_img_dsc_t *bongo_imgs_gui[] = {&bongo_cat_tap1_03, &bongo_cat_tap2_03};
// Alt: tap right
const lv_img_dsc_t *bongo_imgs_alt[] = {&bongo_cat_tap2_03, &bongo_cat_tap1_03};
// Ctrl: fast double tap
const lv_img_dsc_t *bongo_imgs_ctrl[] = {&bongo_cat_double_tap2_02, &bongo_cat_double_tap1_03};
// Shift: alternating taps
const lv_img_dsc_t *bongo_imgs_shift[] = {&bongo_cat_double_tap1_03, &bongo_cat_double_tap2_02};

static lv_obj_t *bongo_imgs = NULL; // Variable estática para almacenar el objeto animado
#define MODIFIERS_USE_BONGO_CAT 1

#elif IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_LUNA)

LV_IMG_DECLARE(dog_sit1_90);
LV_IMG_DECLARE(dog_sit2_90);
LV_IMG_DECLARE(dog_walk1_90);
LV_IMG_DECLARE(dog_walk2_90);
LV_IMG_DECLARE(dog_run1_90);
LV_IMG_DECLARE(dog_run2_90);
LV_IMG_DECLARE(dog_sneak1_90);
LV_IMG_DECLARE(dog_sneak2_90);

const lv_img_dsc_t *luna_imgs_sit_90[] = {&dog_sit1_90, &dog_sit2_90};
const lv_img_dsc_t *luna_imgs_walk_90[] = {&dog_walk1_90, &dog_walk2_90};
const lv_img_dsc_t *luna_imgs_run_90[] = {&dog_run1_90, &dog_run2_90};
const lv_img_dsc_t *luna_imgs_sneak_90[] = {&dog_sneak1_90, &dog_sneak2_90};

static lv_obj_t *luna_imgs = NULL; // Variable estática para almacenar el objeto animado
#define MODIFIERS_USE_LUNA 1

#else
// No animation, no symbols - modifiers widget will show nothing
#define MODIFIERS_USE_NONE 1

#endif

static void set_modifiers_text(lv_obj_t *label, struct modifiers_state ignored) {
    uint8_t mods = zmk_hid_get_explicit_mods();
    /* Limpiamos el texto del label, ya que se usarán imágenes fijas */
    lv_label_set_text(label, "");

#if defined(MODIFIERS_USE_SYMBOLS)
    //     lv_canvas_draw_img(canvas, 45, 2, &usb, &img_dsc);

    /* Definición de variables estáticas para cada imagen fija */
    static lv_obj_t *fixed_win = NULL;
    static lv_obj_t *fixed_alt = NULL;
    static lv_obj_t *fixed_ctl = NULL;
    static lv_obj_t *fixed_shf = NULL;

    /* Creación de los objetos de imagen (si aún no existen) */
    // Offset base para posicionamiento vertical de símbolos
    const int base_x = CONFIG_NICE_OLED_WIDGET_MODIFIERS_CUSTOM_X;
    const int base_y = CONFIG_NICE_OLED_WIDGET_MODIFIERS_CUSTOM_Y;

    if (!fixed_ctl) {
        fixed_ctl = lv_img_create(label);
        lv_obj_align(fixed_ctl, LV_ALIGN_TOP_LEFT, base_x + 78, base_y - 44);
        lv_img_set_src(fixed_ctl, symbol_imgs_control[0]);
    }
    if (!fixed_shf) {
        fixed_shf = lv_img_create(label);
        lv_obj_align(fixed_shf, LV_ALIGN_TOP_LEFT, base_x + 65, base_y - 44);
        lv_img_set_src(fixed_shf, symbol_imgs_shift[0]);
    }
    if (!fixed_alt) {
        fixed_alt = lv_img_create(label);
        lv_obj_align(fixed_alt, LV_ALIGN_TOP_LEFT, base_x + 51, base_y - 44);
        lv_img_set_src(fixed_alt, symbol_imgs_alt[0]);
    }
    if (!fixed_win) {
        fixed_win = lv_img_create(label);
        lv_obj_align(fixed_win, LV_ALIGN_TOP_LEFT, base_x + 37, base_y - 44);
        lv_img_set_src(fixed_win, symbol_imgs_win[0]);
    }

    /* Actualizar la fuente de cada imagen según el estado de cada modificador */
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED_SYMBOL_WINDOWS)
    /* Para Windows/Command: si alguno de los mods está activo, usar la imagen blanca */
    lv_img_set_src(fixed_win,
                   (mods & (MOD_LGUI | MOD_RGUI)) ? symbol_imgs_win[1] : symbol_imgs_win[0]);
#else
    /* En caso de no usar la opción WINDOWS, se aplicaría la lógica correspondiente (por ejemplo con
     * symbol_imgs_win definidas como cmd) */
    lv_img_set_src(fixed_win,
                   (mods & (MOD_LGUI | MOD_RGUI)) ? symbol_imgs_win[1] : symbol_imgs_win[0]);
#endif

    lv_img_set_src(fixed_alt,
                   (mods & (MOD_LALT | MOD_RALT)) ? symbol_imgs_alt[1] : symbol_imgs_alt[0]);
    lv_img_set_src(fixed_ctl, (mods & (MOD_LCTL | MOD_RCTL)) ? symbol_imgs_control[1]
                                                             : symbol_imgs_control[0]);
    lv_img_set_src(fixed_shf,
                   (mods & (MOD_LSFT | MOD_RSFT)) ? symbol_imgs_shift[1] : symbol_imgs_shift[0]);

#elif defined(MODIFIERS_USE_BONGO_CAT)
    /* En modo "bongo cat" se utiliza la lógica de animación */
    if (mods & (MOD_LGUI | MOD_RGUI)) {
        if (!bongo_imgs) {
            bongo_imgs = lv_animimg_create(label);
            lv_obj_center(bongo_imgs);
            lv_animimg_set_src(bongo_imgs, (const void **)bongo_imgs_gui, 2);
            lv_animimg_set_duration(bongo_imgs,
                                    CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_BONGO_CAT_ANIMATION_MS);
            lv_animimg_set_repeat_count(bongo_imgs, LV_ANIM_REPEAT_INFINITE);
            lv_animimg_start(bongo_imgs);
            lv_obj_align(bongo_imgs, LV_ALIGN_TOP_LEFT, CONFIG_NICE_OLED_WIDGET_BONGO_CAT_CUSTOM_X, CONFIG_NICE_OLED_WIDGET_BONGO_CAT_CUSTOM_Y);
        }
    } else if (mods & (MOD_LALT | MOD_RALT)) {
        if (!bongo_imgs) {
            bongo_imgs = lv_animimg_create(label);
            lv_obj_center(bongo_imgs);
            lv_animimg_set_src(bongo_imgs, (const void **)bongo_imgs_alt, 2);
            lv_animimg_set_duration(bongo_imgs,
                                    CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_BONGO_CAT_ANIMATION_MS);
            lv_animimg_set_repeat_count(bongo_imgs, LV_ANIM_REPEAT_INFINITE);
            lv_animimg_start(bongo_imgs);
            lv_obj_align(bongo_imgs, LV_ALIGN_TOP_LEFT, CONFIG_NICE_OLED_WIDGET_BONGO_CAT_CUSTOM_X, CONFIG_NICE_OLED_WIDGET_BONGO_CAT_CUSTOM_Y);
        }
    } else if (mods & (MOD_LCTL | MOD_RCTL)) {
        if (!bongo_imgs) {
            bongo_imgs = lv_animimg_create(label);
            lv_obj_center(bongo_imgs);
            lv_animimg_set_src(bongo_imgs, (const void **)bongo_imgs_ctrl, 2);
            lv_animimg_set_duration(bongo_imgs,
                                    CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_BONGO_CAT_ANIMATION_MS);
            lv_animimg_set_repeat_count(bongo_imgs, LV_ANIM_REPEAT_INFINITE);
            lv_animimg_start(bongo_imgs);
            lv_obj_align(bongo_imgs, LV_ALIGN_TOP_LEFT, CONFIG_NICE_OLED_WIDGET_BONGO_CAT_CUSTOM_X, CONFIG_NICE_OLED_WIDGET_BONGO_CAT_CUSTOM_Y);
        }
    } else if (mods & (MOD_LSFT | MOD_RSFT)) {
        if (!bongo_imgs) {
            bongo_imgs = lv_animimg_create(label);
            lv_obj_center(bongo_imgs);
            lv_animimg_set_src(bongo_imgs, (const void **)bongo_imgs_shift, 2);
            lv_animimg_set_duration(bongo_imgs,
                                    CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_BONGO_CAT_ANIMATION_MS);
            lv_animimg_set_repeat_count(bongo_imgs, LV_ANIM_REPEAT_INFINITE);
            lv_animimg_start(bongo_imgs);
            lv_obj_align(bongo_imgs, LV_ALIGN_TOP_LEFT, CONFIG_NICE_OLED_WIDGET_BONGO_CAT_CUSTOM_X, CONFIG_NICE_OLED_WIDGET_BONGO_CAT_CUSTOM_Y);
        }
    } else {
        if (bongo_imgs) {
            lv_obj_del(bongo_imgs);
            bongo_imgs = NULL;
        }
    }

#elif defined(MODIFIERS_USE_LUNA)
    /* En modo "luna" se utiliza la lógica de animación ya existente */
    if (mods & (MOD_LGUI | MOD_RGUI)) {
        if (!luna_imgs) {
            luna_imgs = lv_animimg_create(label);
            lv_obj_center(luna_imgs);
            lv_animimg_set_src(luna_imgs, (const void **)luna_imgs_sit_90, 2);
            lv_animimg_set_duration(luna_imgs,
                                    CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_LUNA_ANIMATION_MS);
            lv_animimg_set_repeat_count(luna_imgs, LV_ANIM_REPEAT_INFINITE);
            lv_animimg_start(luna_imgs);
            lv_obj_align(luna_imgs, LV_ALIGN_TOP_LEFT, CONFIG_NICE_OLED_WIDGET_LUNA_CUSTOM_X, CONFIG_NICE_OLED_WIDGET_LUNA_CUSTOM_Y);
        }
    } else if (mods & (MOD_LALT | MOD_RALT)) {
        if (!luna_imgs) {
            luna_imgs = lv_animimg_create(label);
            lv_obj_center(luna_imgs);
            lv_animimg_set_src(luna_imgs, (const void **)luna_imgs_walk_90, 2);
            lv_animimg_set_duration(luna_imgs,
                                    CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_LUNA_ANIMATION_MS);
            lv_animimg_set_repeat_count(luna_imgs, LV_ANIM_REPEAT_INFINITE);
            lv_animimg_start(luna_imgs);
            lv_obj_align(luna_imgs, LV_ALIGN_TOP_LEFT, CONFIG_NICE_OLED_WIDGET_LUNA_CUSTOM_X, CONFIG_NICE_OLED_WIDGET_LUNA_CUSTOM_Y);
        }
    } else if (mods & (MOD_LCTL | MOD_RCTL)) {
        if (!luna_imgs) {
            luna_imgs = lv_animimg_create(label);
            lv_obj_center(luna_imgs);
            lv_animimg_set_src(luna_imgs, (const void **)luna_imgs_run_90, 2);
            lv_animimg_set_duration(luna_imgs,
                                    CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_LUNA_ANIMATION_MS);
            lv_animimg_set_repeat_count(luna_imgs, LV_ANIM_REPEAT_INFINITE);
            lv_animimg_start(luna_imgs);
            lv_obj_align(luna_imgs, LV_ALIGN_TOP_LEFT, CONFIG_NICE_OLED_WIDGET_LUNA_CUSTOM_X, CONFIG_NICE_OLED_WIDGET_LUNA_CUSTOM_Y);
        }
    } else if (mods & (MOD_LSFT | MOD_RSFT)) {
        if (!luna_imgs) {
            luna_imgs = lv_animimg_create(label);
            lv_obj_center(luna_imgs);
            lv_animimg_set_src(luna_imgs, (const void **)luna_imgs_sneak_90, 2);
            lv_animimg_set_duration(luna_imgs,
                                    CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_LUNA_ANIMATION_MS);
            lv_animimg_set_repeat_count(luna_imgs, LV_ANIM_REPEAT_INFINITE);
            lv_animimg_start(luna_imgs);
            lv_obj_align(luna_imgs, LV_ALIGN_TOP_LEFT, CONFIG_NICE_OLED_WIDGET_LUNA_CUSTOM_X, CONFIG_NICE_OLED_WIDGET_LUNA_CUSTOM_Y);
        }
    } else {
        if (luna_imgs) {
            lv_obj_del(luna_imgs);
            luna_imgs = NULL;
        }
    }

#else
    // MODIFIERS_USE_NONE: No animation, no symbols
    // Widget remains functional but displays nothing visually
    (void)mods; // Suppress unused variable warning
#endif
}

/**
 * Callback de actualización del widget, se invoca al cambiar el estado de los modificadores.
 */
static void modifiers_update_cb(struct modifiers_state state) {
    struct zmk_widget_modifiers *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) { set_modifiers_text(widget->obj, state); }
}

/**
 * Obtiene el estado actual de los modificadores.
 *
 * @param eh Evento de ZMK.
 * @return Estructura con el estado de los modificadores.
 */
static struct modifiers_state modifiers_get_state(const zmk_event_t *eh) {
    return (struct modifiers_state){.modifiers = zmk_hid_get_explicit_mods()};
}

/* Registra el listener para actualizar el widget cuando cambie el estado de los modificadores */
ZMK_DISPLAY_WIDGET_LISTENER(widget_modifiers, struct modifiers_state, modifiers_update_cb,
                            modifiers_get_state)
ZMK_SUBSCRIPTION(widget_modifiers, zmk_keycode_state_changed);

/**
 * Inicializa el widget de modificadores.
 * Se crea un label que mostrará el texto con los modificadores activos.
 *
 * @param widget Puntero a la estructura del widget.
 * @param parent Objeto padre de LVGL en el que se creará el label.
 * @return 0 si la inicialización fue exitosa.
 */
int zmk_widget_modifiers_init(struct zmk_widget_modifiers *widget, lv_obj_t *parent) {
    widget->obj = lv_label_create(parent);
    sys_slist_append(&widgets, &widget->node);
    widget_modifiers_init();
    return 0;
}
