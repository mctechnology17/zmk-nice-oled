#include <zephyr/kernel.h>

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#include <zmk/battery.h>
#include <zmk/ble.h>
#include <zmk/display.h>
#include <zmk/endpoints.h>
#include <zmk/event_manager.h>
#include <zmk/events/battery_state_changed.h>
#include <zmk/events/ble_active_profile_changed.h>
#include <zmk/events/endpoint_changed.h>
#include <zmk/events/layer_state_changed.h>
#include <zmk/events/usb_conn_state_changed.h>
#include <zmk/events/wpm_state_changed.h>
#include <zmk/keymap.h>
#include <zmk/usb.h>
#include <zmk/wpm.h>
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_ALL) ||                     \
    IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_ONLY) ||                    \
    IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_AND_CENTRAL)
#include <zmk/split/central.h>
#endif

#include <fonts.h>
#include "output.h"
#include "profile.h"
#include "screen.h"

#ifdef CONFIG_NICE_OLED_WIDGET_RAW_HID
#include <lvgl.h>
#include <raw_hid/hid.h>
#endif

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED)
#include <zmk/events/keycode_state_changed.h>
#include <zmk/hid.h>
#include <dt-bindings/zmk/modifiers.h>
#endif

#if !IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_ALL) &&                    \
    !IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_ONLY) &&                   \
    !IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_AND_CENTRAL)

#include "battery.h"
#endif

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_ALL) ||                     \
    IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_ONLY) ||                    \
    IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_AND_CENTRAL)
struct battery_state {
    uint8_t source;
    uint8_t level;
    bool usb_present;
};
/**
 * @brief Dibuja el estado de la batería como texto "CENTRAL - PERIPHERAL".
 *
 * Esta función toma el estado de la batería del central y del periférico
 * y lo muestra como una cadena de texto simple en las coordenadas especificadas.
 */
static void draw_battery_text(lv_obj_t *canvas, const struct status_state *state) {
    // Un buffer de texto más grande para manejar múltiples baterías
    char text[32] = "";
    lv_draw_label_dsc_t label_dsc;

    // Inicialización de la fuente y el estilo del texto
#if IS_ENABLED(CONFIG_NICE_EPAPER_ON)
    init_label_dsc(&label_dsc, LVGL_FOREGROUND, &pixel_operator_mono_16, LV_TEXT_ALIGN_LEFT);
#else
    init_label_dsc(&label_dsc, LVGL_FOREGROUND, &lv_font_unscii_8, LV_TEXT_ALIGN_LEFT);
#endif

    //  Lógica Parcelada
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_ALL)
    // MODO 1: Muestra TODAS las baterías (Central + Periféricos) en una sola linea
    char *p = text;
    char *end = text + sizeof(text);
    for (int i = 0; i < CONFIG_NICE_OLED_SPLIT_TOTAL_DEVICES; i++) {
        // Añade el nivel de la batería y un espacio, controlando el tamaño del buffer
        int written = snprintf(p, end - p, "%d ", state->batteries[i].level);
        if (written > 0) {
            p += written;
        }
    }
    // Elimina el último espacio si se escribió algo
    if (p > text) {
        *(p - 1) = '\0';
    }

#elif IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_ONLY)
    // MODO 2: Muestra SÓLO las baterías de los periféricos
    char *p = text;
    char *end = text + sizeof(text);
    // El bucle empieza en 1 para saltarse la batería central (índice 0)
    for (int i = 1; i < CONFIG_NICE_OLED_SPLIT_TOTAL_DEVICES; i++) {
        int written = snprintf(p, end - p, "%d  ", state->batteries[i].level);
        if (written > 0) {
            p += written;
        }
    }
    if (p > text) {
        *(p - 1) = '\0';
    }

#elif IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_AND_CENTRAL)
    // MODO 3: Muestra la batería central y la del PRIMER periférico
    if (CONFIG_NICE_OLED_SPLIT_TOTAL_DEVICES >= 2) {
        snprintf(text, sizeof(text), "%d  %d", state->batteries[0].level,
                 state->batteries[1].level);
    } else {
        // Si no hay periférico, muestra solo la central
        snprintf(text, sizeof(text), "%d", state->batteries[0].level);
    }
#endif

    // Dibuja la cadena de texto final en la pantalla
    lv_canvas_draw_text(canvas, 0, 19, lv_obj_get_width(canvas), &label_dsc, text);
}
/*
static void draw_battery_text(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_label_dsc_t label_dsc;

    // Inicialización de la fuente y el estilo del texto
#if IS_ENABLED(CONFIG_NICE_EPAPER_ON)
    init_label_dsc(&label_dsc, LVGL_FOREGROUND, &pixel_operator_mono_16, LV_TEXT_ALIGN_LEFT);
#else
    init_label_dsc(&label_dsc, LVGL_FOREGROUND, &lv_font_unscii_8, LV_TEXT_ALIGN_LEFT);
#endif

    //  Lógica Parcelada
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_ALL)
    // MODO 1: Muestra TODAS las baterías, en dos líneas separadas.

    //  Batería Central
    char central_text[8];
    memset(central_text, 0, sizeof(central_text)); // Limpia el búfer
    snprintf(central_text, sizeof(central_text), "%d", state->batteries[0].level);
    lv_canvas_draw_text(canvas, 0, 1, 25, &label_dsc, central_text);

    //  Baterías Periféricas
    char peripheral_text[32];
    memset(peripheral_text, 0, sizeof(peripheral_text)); // Limpia el búfer
    char *p = peripheral_text;
    char *end = peripheral_text + sizeof(peripheral_text);

    for (int i = 1; i < CONFIG_NICE_OLED_SPLIT_TOTAL_DEVICES; i++) {
        int written = snprintf(p, end - p, "%d ", state->batteries[i].level);
        if (written > 0) {
            p += written;
        }
    }
    if (p > peripheral_text) {
        *(p - 1) = '\0'; // Elimina el último espacio
    }
    lv_canvas_draw_text(canvas, 0, 19, lv_obj_get_width(canvas), &label_dsc, peripheral_text);

#else
    // MODO 2 y 3 (el resto de los casos)
    char text[32];
    memset(text, 0, sizeof(text)); // Limpia el búfer

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_ONLY)
    // Muestra SÓLO las baterías de los periféricos
    char *p = text;
    char *end = text + sizeof(text);
    for (int i = 1; i < CONFIG_NICE_OLED_SPLIT_TOTAL_DEVICES; i++) {
        int written = snprintf(p, end - p, "%d ", state->batteries[i].level);
        if (written > 0) {
            p += written;
        }
    }
    if (p > text) {
        *(p - 1) = '\0';
    }
#elif IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_AND_CENTRAL)
    // Muestra la batería central y la del PRIMER periférico
    if (CONFIG_NICE_OLED_SPLIT_TOTAL_DEVICES >= 2) {
        snprintf(text, sizeof(text), "%d %d", state->batteries[0].level, state->batteries[1].level);
    } else {
        snprintf(text, sizeof(text), "%d", state->batteries[0].level);
    }
#endif

    // Dibuja la cadena de texto final para los modos 2 y 3
    lv_canvas_draw_text(canvas, 0, 19, lv_obj_get_width(canvas), &label_dsc, text);
#endif
}
*/

//  FIN DE LA SECCIÓN REFACTORIZADA
#endif

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_LAYER)
#include "layer.h"
#endif
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM)
#include "wpm.h"
#endif // IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM)

static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);

//  Declaración adelantada (Forward Declaration) para draw_canvas
static void draw_canvas(lv_obj_t *widget, lv_color_t cbuf[], const struct status_state *state);
//  Fin Declaración adelantada

/**
 * sleep status
 **/

#if IS_ENABLED(CONFIG_NICE_OLED_SHOW_SLEEP_ART_ON_IDLE) ||                                         \
    IS_ENABLED(CONFIG_NICE_OLED_SHOW_SLEEP_ART_ON_SLEEP)
#include "sleep_status.h"
static struct zmk_widget_sleep_status sleep_status_widget;
#endif

/**
 * luna
 **/

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_LUNA)
#include "luna.h"
static struct zmk_widget_luna luna_widget;
#endif

/**
 * bongo cat
 **/

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_BONGO_CAT)
#include "bongo_cat.h"
static struct zmk_widget_wpm_bongo_cat wpm_bongo_cat_widget;
#endif

/**
 * responsive bongo cat
 **/

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RESPONSIVE_BONGO_CAT)
#include "responsive_bongo_cat.h"
static struct zmk_widget_responsive_bongo_cat responsive_bongo_cat_widget;
#endif

/**
 * modifiers
 **/
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_LUNA)
#include "modifiers.h"
static struct zmk_widget_modifiers modifiers_widget;
#endif

//  INICIO SECCIÓN MODIFICADORES (NUEVA INTEGRACIÓN)
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED)

struct mods_status_state {
    uint8_t mods;
};

// Declaraciones de imágenes de símbolos reales (de modifiers_270.c)
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED_SYMBOL)
LV_IMG_DECLARE(control_0);
LV_IMG_DECLARE(control_white_0);
LV_IMG_DECLARE(shift_0);
LV_IMG_DECLARE(shift_white_0);
LV_IMG_DECLARE(opt_0);
LV_IMG_DECLARE(opt_white_0);
LV_IMG_DECLARE(alt_0);
LV_IMG_DECLARE(alt_white_0);
LV_IMG_DECLARE(cmd_0);
LV_IMG_DECLARE(cmd_white_0);
LV_IMG_DECLARE(win_0);
LV_IMG_DECLARE(win_white_0);

// Arrays de imágenes: [0] = normal, [1] = activo (blanco/invertido)
// Orden: Control, Shift, Alt/Opt, Gui/Cmd/Win
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED_SYMBOL_WINDOWS)
// Windows: Control, Shift, Alt, Win
static const lv_img_dsc_t *mod_imgs_normal[4] = {&control_0, &shift_0, &alt_0, &win_0};
static const lv_img_dsc_t *mod_imgs_active[4] = {&control_white_0, &shift_white_0, &alt_white_0, &win_white_0};
#else
// macOS (default): Control, Shift, Option, Command
static const lv_img_dsc_t *mod_imgs_normal[4] = {&control_0, &shift_0, &opt_0, &cmd_0};
static const lv_img_dsc_t *mod_imgs_active[4] = {&control_white_0, &shift_white_0, &opt_white_0, &cmd_white_0};
#endif
#endif // CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED_SYMBOL

// Función de dibujo para los modificadores
static void draw_mods_status(lv_obj_t *canvas, const struct status_state *state) {
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED_SYMBOL)
    // --- MODO SÍMBOLOS (Imágenes reales) ---
    lv_draw_img_dsc_t img_dsc;
    lv_draw_img_dsc_init(&img_dsc);

    // Las imágenes son 14x14 píxeles
    const int img_size = 14;
    const int spacing = 2;

    // Posición Base según tipo de pantalla y layout
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED_VER)
    // --- VERTICAL (Apilado) ---
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED_VER_ALIGN_RIGHT)
#if IS_ENABLED(CONFIG_NICE_EPAPER_ON)
    const int base_x = 68 - img_size - 2; // buena posicion para X vertical en right
#else
    const int base_x = 128 - img_size - 2;
#endif
#elif IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED_VER_ALIGN_LEFT)
    const int base_x = CONFIG_NICE_OLED_WIDGET_MODIFIERS_CUSTOM_X;
#else
#if IS_ENABLED(CONFIG_NICE_EPAPER_ON)
    const int base_x = (68 - img_size) / 2; // 68
#else
    const int base_x = (128 - img_size) / 2;
#endif
#endif
    const int base_y = 62; // start base y = 38, test 62 like raw hid

    for (int i = 0; i < 4; i++) {
        bool selected = (state->mod_state >> i) & 1 || (state->mod_state >> (i + 4)) & 1;
        int current_x = base_x;
        int current_y = base_y + i * (img_size + spacing);
        const lv_img_dsc_t *img = selected ? mod_imgs_active[i] : mod_imgs_normal[i];
        lv_canvas_draw_img(canvas, current_x, current_y, img, &img_dsc);
    }

#elif IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED_HOR)
    // --- HORIZONTAL ---
    const int base_x = CONFIG_NICE_OLED_WIDGET_MODIFIERS_CUSTOM_X;
    const int base_y = CONFIG_NICE_OLED_WIDGET_MODIFIERS_CUSTOM_Y;

    for (int i = 0; i < 4; i++) {
        bool selected = (state->mod_state >> i) & 1 || (state->mod_state >> (i + 4)) & 1;
        int current_x = base_x + i * (img_size + spacing);
        int current_y = base_y;
        const lv_img_dsc_t *img = selected ? mod_imgs_active[i] : mod_imgs_normal[i];
        lv_canvas_draw_img(canvas, current_x, current_y, img, &img_dsc);
    }

#elif IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED_BOX)
    // --- BOX (2x2) ---
    const int base_x = CONFIG_NICE_OLED_WIDGET_MODIFIERS_CUSTOM_X;
    const int base_y = CONFIG_NICE_OLED_WIDGET_MODIFIERS_CUSTOM_Y;

    static const int offsets_box[4][2] = {
        {0, 0},                              // C (0,0)
        {img_size + spacing, 0},             // S (0,1)
        {0, img_size + spacing},             // A (1,0)
        {img_size + spacing, img_size + spacing} // G (1,1)
    };

    for (int i = 0; i < 4; i++) {
        bool selected = (state->mod_state >> i) & 1 || (state->mod_state >> (i + 4)) & 1;
        int current_x = base_x + offsets_box[i][0];
        int current_y = base_y + offsets_box[i][1];
        const lv_img_dsc_t *img = selected ? mod_imgs_active[i] : mod_imgs_normal[i];
        lv_canvas_draw_img(canvas, current_x, current_y, img, &img_dsc);
    }

#else
    // --- DEFAULT: BOX fallback ---
    const int base_x = CONFIG_NICE_OLED_WIDGET_MODIFIERS_CUSTOM_X;
    const int base_y = CONFIG_NICE_OLED_WIDGET_MODIFIERS_CUSTOM_Y;

    static const int offsets_default[4][2] = {
        {0, 0},
        {img_size + spacing, 0},
        {0, img_size + spacing},
        {img_size + spacing, img_size + spacing}
    };

    for (int i = 0; i < 4; i++) {
        bool selected = (state->mod_state >> i) & 1 || (state->mod_state >> (i + 4)) & 1;
        int current_x = base_x + offsets_default[i][0];
        int current_y = base_y + offsets_default[i][1];
        const lv_img_dsc_t *img = selected ? mod_imgs_active[i] : mod_imgs_normal[i];
        lv_canvas_draw_img(canvas, current_x, current_y, img, &img_dsc);
    }
#endif

#else
    // --- MODO LETRAS (Texto) ---
    const char *items[4] = {"C", "S", "A", "G"};

    // Descriptores de dibujo
    lv_draw_rect_dsc_t rect_black_dsc;
    init_rect_dsc(&rect_black_dsc, LVGL_BACKGROUND);
    lv_draw_rect_dsc_t rect_white_dsc;
    init_rect_dsc(&rect_white_dsc, LVGL_FOREGROUND);
    lv_draw_label_dsc_t mod_dsc;
    init_label_dsc(&mod_dsc, LVGL_FOREGROUND, &lv_font_unscii_8, LV_TEXT_ALIGN_CENTER);
    lv_draw_label_dsc_t mod_dsc_black;
    init_label_dsc(&mod_dsc_black, LVGL_BACKGROUND, &lv_font_unscii_8, LV_TEXT_ALIGN_CENTER);

    // Dimensiones de caja
    const int box_width = 12;
    const int box_height = 14;
    const int inner_box_offset = 2;
    const int text_offset_y = 4;
    const int inner_box_width = box_width - (2 * inner_box_offset);
    const int inner_box_height = box_height - (2 * inner_box_offset);

    // Posición Base según tipo de pantalla y layout
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED_VER)
    // --- VERTICAL (Apilado) ---
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED_VER_ALIGN_RIGHT)
#if IS_ENABLED(CONFIG_NICE_EPAPER_ON)
    const int base_x = 68 - box_width - 2;
#else
    const int base_x = 128 - box_width - 2;
#endif
#elif IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED_VER_ALIGN_LEFT)
    const int base_x = CONFIG_NICE_OLED_WIDGET_MODIFIERS_CUSTOM_X;
#else
#if IS_ENABLED(CONFIG_NICE_EPAPER_ON)
    const int base_x = (68 - box_width) / 2;
#else
    const int base_x = (128 - box_width) / 2;
#endif
#endif
    const int base_y = 38;

    for (int i = 0; i < 4; i++) {
        bool selected = (state->mod_state >> i) & 1 || (state->mod_state >> (i + 4)) & 1;
        int current_x = base_x;
        int current_y = base_y + i * (box_height + 2);

        lv_canvas_draw_rect(canvas, current_x, current_y, box_width, box_height, &rect_black_dsc);
        if (selected && inner_box_width > 0 && inner_box_height > 0) {
            lv_canvas_draw_rect(canvas, current_x + inner_box_offset,
                                current_y + inner_box_offset, inner_box_width, inner_box_height,
                                &rect_white_dsc);
        }
        lv_canvas_draw_text(canvas, current_x, current_y + text_offset_y, box_width,
                            (selected ? &mod_dsc_black : &mod_dsc), items[i]);
    }

#elif IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED_HOR)
    // --- HORIZONTAL ---
    const int base_x = CONFIG_NICE_OLED_WIDGET_MODIFIERS_CUSTOM_X;
    const int base_y = CONFIG_NICE_OLED_WIDGET_MODIFIERS_CUSTOM_Y;

    for (int i = 0; i < 4; i++) {
        bool selected = (state->mod_state >> i) & 1 || (state->mod_state >> (i + 4)) & 1;
        int current_x = base_x + i * (box_width + 2);
        int current_y = base_y;

        lv_canvas_draw_rect(canvas, current_x, current_y, box_width, box_height, &rect_black_dsc);
        if (selected && inner_box_width > 0 && inner_box_height > 0) {
            lv_canvas_draw_rect(canvas, current_x + inner_box_offset,
                                current_y + inner_box_offset, inner_box_width, inner_box_height,
                                &rect_white_dsc);
        }
        lv_canvas_draw_text(canvas, current_x, current_y + text_offset_y, box_width,
                            (selected ? &mod_dsc_black : &mod_dsc), items[i]);
    }

#elif IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED_BOX)
    // --- BOX (2x2) ---
    const int base_x = CONFIG_NICE_OLED_WIDGET_MODIFIERS_CUSTOM_X;
    const int base_y = CONFIG_NICE_OLED_WIDGET_MODIFIERS_CUSTOM_Y;

    static const int offsets_box[4][2] = {
        {0, 0}, {box_width + 2, 0}, {0, box_height + 2}, {box_width + 2, box_height + 2}
    };

    for (int i = 0; i < 4; i++) {
        bool selected = (state->mod_state >> i) & 1 || (state->mod_state >> (i + 4)) & 1;
        int current_x = base_x + offsets_box[i][0];
        int current_y = base_y + offsets_box[i][1];

        lv_canvas_draw_rect(canvas, current_x, current_y, box_width, box_height, &rect_black_dsc);
        if (selected && inner_box_width > 0 && inner_box_height > 0) {
            lv_canvas_draw_rect(canvas, current_x + inner_box_offset,
                                current_y + inner_box_offset, inner_box_width, inner_box_height,
                                &rect_white_dsc);
        }
        lv_canvas_draw_text(canvas, current_x, current_y + text_offset_y, box_width,
                            (selected ? &mod_dsc_black : &mod_dsc), items[i]);
    }

#else
    // --- DEFAULT: BOX fallback ---
    const int base_x = CONFIG_NICE_OLED_WIDGET_MODIFIERS_CUSTOM_X;
    const int base_y = CONFIG_NICE_OLED_WIDGET_MODIFIERS_CUSTOM_Y;

    static const int offsets_default[4][2] = {
        {0, 0}, {box_width + 2, 0}, {0, box_height + 2}, {box_width + 2, box_height + 2}
    };

    for (int i = 0; i < 4; i++) {
        bool selected = (state->mod_state >> i) & 1 || (state->mod_state >> (i + 4)) & 1;
        int current_x = base_x + offsets_default[i][0];
        int current_y = base_y + offsets_default[i][1];

        lv_canvas_draw_rect(canvas, current_x, current_y, box_width, box_height, &rect_black_dsc);
        if (selected && inner_box_width > 0 && inner_box_height > 0) {
            lv_canvas_draw_rect(canvas, current_x + inner_box_offset,
                                current_y + inner_box_offset, inner_box_width, inner_box_height,
                                &rect_white_dsc);
        }
        lv_canvas_draw_text(canvas, current_x, current_y + text_offset_y, box_width,
                            (selected ? &mod_dsc_black : &mod_dsc), items[i]);
    }
#endif
#endif // CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED_SYMBOL
}

#endif // IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED)
//  FIN SECCIÓN MODIFICADORES (NUEVA INTEGRACIÓN)

//  INICIO SECCIÓN LISTENER MODIFICADORES (NUEVA INTEGRACIÓN)
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED)

// Función para actualizar el estado del widget (adaptada al patrón existente)
static void set_mods_status(struct zmk_widget_screen *widget,
                            struct mods_status_state state /* No usada directamente */) {
#if !IS_ENABLED(CONFIG_ZMK_SPLIT) || IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
    // Obtiene el estado actual de los modificadores directamente
    widget->state.mod_state = zmk_hid_get_explicit_mods();
    // Vuelve a dibujar todo el canvas para reflejar el cambio
    draw_canvas(widget->obj, widget->cbuf, &widget->state);
#endif
}

// Callback que se llama cuando el estado necesita actualizarse
static void mods_status_update_cb(struct mods_status_state state) {
    struct zmk_widget_screen *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) { set_mods_status(widget, state); }
}

// Función para obtener el estado (requerida por el listener)
static struct mods_status_state mods_status_get_state(const zmk_event_t *eh) {
#if !IS_ENABLED(CONFIG_ZMK_SPLIT) || IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
    // No necesita devolver el estado real aquí porque set_mods_status lo obtiene
    // Pero podríamos devolverlo si quisiéramos coherencia
    return (struct mods_status_state){.mods = zmk_hid_get_explicit_mods()};
#else
    return (struct mods_status_state){.mods = 0}; // Estado vacío para periférico
#endif
};

// Registra el listener para el estado de los modificadores
ZMK_DISPLAY_WIDGET_LISTENER(widget_mods_status, struct mods_status_state, mods_status_update_cb,
                            mods_status_get_state)
// Se suscribe a los cambios de estado de las teclas (que pueden afectar a los modificadores)
ZMK_SUBSCRIPTION(widget_mods_status, zmk_keycode_state_changed);

#endif // IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED)
//  FIN SECCIÓN LISTENER MODIFICADORES (NUEVA INTEGRACIÓN)

/**
 * raw hid
 **/

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID)

// Función para dibujar el estado de Raw HID en el canvas principal

static void draw_hid_status(lv_obj_t *canvas, const struct status_state *state) {

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED_SYMBOL_VERTICAL) ||              \
    IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED_ONE_LINE_VERTICAL)

#define DRAW_HID_STATUS_TEXT_ALIGN LV_TEXT_ALIGN_LEFT

#else // IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED_SYMBOL_VERTICAL) ||
      // IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED_ONE_LINE_VERTICAL)
#define DRAW_HID_STATUS_TEXT_ALIGN LV_TEXT_ALIGN_LEFT

#endif // IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED_SYMBOL_VERTICAL) ||
       // IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED_ONE_LINE_VERTICAL)

    // lv_font_unscii_8
    // #define DRAW_HID_STATUS_FONTS \ (IS_ENABLED(CONFIG_NICE_EPAPER_ON) ? &lv_font_montserrat_14 :
    // &pixel_operator_mono_12)
#if IS_ENABLED(CONFIG_NICE_EPAPER_ON)
#define DRAW_HID_STATUS_FONTS &lv_font_montserrat_14
#else
#define DRAW_HID_STATUS_FONTS &pixel_operator_mono_12
#endif // IS_ENABLED(CONFIG_NICE_EPAPER_ON)

    lv_draw_rect_dsc_t rect_black_dsc;
    init_rect_dsc(&rect_black_dsc, LVGL_BACKGROUND);
    lv_draw_label_dsc_t label_time;
    // init_label_dsc(&label_time, LVGL_FOREGROUND, &lv_font_montserrat_22, LV_TEXT_ALIGN_CENTER);
    init_label_dsc(&label_time, LVGL_FOREGROUND, DRAW_HID_STATUS_FONTS, DRAW_HID_STATUS_TEXT_ALIGN);
    lv_draw_label_dsc_t label_layout;
    init_label_dsc(&label_layout, LVGL_FOREGROUND, DRAW_HID_STATUS_FONTS,
                   DRAW_HID_STATUS_TEXT_ALIGN);
    lv_draw_label_dsc_t label_volume;
    init_label_dsc(&label_volume, LVGL_FOREGROUND, DRAW_HID_STATUS_FONTS,
                   DRAW_HID_STATUS_TEXT_ALIGN);

    //  Área de dibujo - base position for fallback
    int hid_area_x = CONFIG_NICE_OLED_WIDGET_RAW_HID_CUSTOM_X;
    int hid_area_y = CONFIG_NICE_OLED_WIDGET_RAW_HID_CUSTOM_Y;
#if IS_ENABLED(CONFIG_NICE_EPAPER_ON)
    int hid_area_width = 68;
#else
    int hid_area_width = 32;
#endif // IS_ENABLED(CONFIG_NICE_EPAPER_ON)

    // Variable para rastrear la posición Y actual (para "HID not found")
    lv_coord_t current_y = hid_area_y;
    // Variable para almacenar el tamaño del texto calculado
    lv_point_t text_size;
    // Espacio vertical mínimo entre líneas (para "HID not found")
    const lv_coord_t line_gap = 0;

    if (state->is_connected) {
        char text_buffer[20]; // Buffer para formatear texto

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID_WEATHER)
        // Dibujar Temperatura
        sprintf(text_buffer, "%dC", state->temperature);
        lv_canvas_draw_text(canvas, CONFIG_NICE_OLED_WIDGET_RAW_HID_WEATHER_CUSTOM_X,
                            CONFIG_NICE_OLED_WIDGET_RAW_HID_WEATHER_CUSTOM_Y,
                            hid_area_width, &label_volume, text_buffer);
#endif

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID_TIME)
        //  Dibujar Hora
        sprintf(text_buffer, "%02i:%02i", state->hour, state->minute);
        lv_canvas_draw_text(canvas, CONFIG_NICE_OLED_WIDGET_RAW_HID_TIME_CUSTOM_X,
                            CONFIG_NICE_OLED_WIDGET_RAW_HID_TIME_CUSTOM_Y,
                            hid_area_width, &label_time, text_buffer);
#endif

        //  Dibujar Layout (condicional)
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID_LAYOUT)
        char layout_str[10] = {};
#ifdef CONFIG_NICE_OLED_WIDGET_RAW_HID_LAYOUT_LIST
        char layouts_config[sizeof(CONFIG_NICE_OLED_WIDGET_RAW_HID_LAYOUT_LIST)];
        strcpy(layouts_config, CONFIG_NICE_OLED_WIDGET_RAW_HID_LAYOUT_LIST);
        char *current_layout_token = strtok(layouts_config, ",");
        size_t i = 0;
        while (current_layout_token != NULL && i < state->layout) {
            i++;
            current_layout_token = strtok(NULL, ",");
        }
        if (current_layout_token != NULL) {
            snprintf(layout_str, sizeof(layout_str), "%s", current_layout_token);
        } else {
            snprintf(layout_str, sizeof(layout_str), "%i", state->layout);
        }
#else
        snprintf(layout_str, sizeof(layout_str), "L%i", state->layout);
#endif
        lv_canvas_draw_text(canvas, CONFIG_NICE_OLED_WIDGET_RAW_HID_LAYOUT_CUSTOM_X,
                            CONFIG_NICE_OLED_WIDGET_RAW_HID_LAYOUT_CUSTOM_Y,
                            hid_area_width, &label_layout, layout_str);
#endif // CONFIG_NICE_OLED_WIDGET_RAW_HID_LAYOUT

        //  Dibujar Volumen
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID_VOLUME)
#if IS_ENABLED(CONFIG_NICE_EPAPER_ON)
        sprintf(text_buffer, "Vol: %i", state->volume);
#else
        sprintf(text_buffer, "V:%i", state->volume);
#endif // IS_ENABLED(CONFIG_NICE_EPAPER_ON)
        lv_canvas_draw_text(canvas, CONFIG_NICE_OLED_WIDGET_RAW_HID_VOLUME_CUSTOM_X,
                            CONFIG_NICE_OLED_WIDGET_RAW_HID_VOLUME_CUSTOM_Y,
                            hid_area_width, &label_volume, text_buffer);
#endif

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_SPOTIFY_MACOS)
        // Dibujar Spotify/Media Player
        lv_canvas_draw_text(canvas, CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_CUSTOM_X,
                            CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_CUSTOM_Y,
                            hid_area_width, &label_volume, state->media_player);
#endif

    } else {
        //  Dibuja mensaje "HID not found"

        // Dibujar "HID"
        lv_txt_get_size(&text_size, "HID", label_time.font, label_time.letter_space,
                        label_time.line_space, LV_COORD_MAX, LV_TEXT_FLAG_NONE);
        lv_canvas_draw_text(canvas, hid_area_x, current_y, hid_area_width, &label_time, "HID");
        current_y += text_size.y + line_gap;

        // Dibujar "not"
        lv_txt_get_size(&text_size, "not", label_layout.font, label_layout.letter_space,
                        label_layout.line_space, LV_COORD_MAX, LV_TEXT_FLAG_NONE);
        lv_canvas_draw_text(canvas, hid_area_x, current_y, hid_area_width, &label_layout, "not");
        current_y += text_size.y + line_gap;

        // Dibujar "found"
        lv_txt_get_size(&text_size, "found", label_volume.font, label_volume.letter_space,
                        label_volume.line_space, LV_COORD_MAX, LV_TEXT_FLAG_NONE);
        lv_canvas_draw_text(canvas, hid_area_x, current_y, hid_area_width, &label_volume, "found");
    }
}

//  Listener para estado de conexión HID
static struct is_connected_notification get_is_hid_connected(const zmk_event_t *eh) {
    // Esta función asume que el evento is_connected_notification existe y se puede extraer así.
    // Verifica que as_is_connected_notification sea la forma correcta de obtener este evento.
    struct is_connected_notification *notification = as_is_connected_notification(eh);
    if (notification) {
        return *notification;
    }
    // Devuelve un estado desconectado por defecto si el evento no es del tipo esperado
    // o si el puntero es NULL (puede pasar durante la inicialización).
    return (struct is_connected_notification){.value = false};
}

static void hid_is_connected_update_cb(struct is_connected_notification is_connected) {
    // Actualiza el estado en *todos* los widgets de pantalla y redibuja
    struct zmk_widget_screen *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) {
        widget->state.is_connected = is_connected.value;
        // Llama a la función principal de dibujo para actualizar toda la pantalla
        draw_canvas(widget->obj, widget->cbuf, &widget->state);
    }
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_is_connected, struct is_connected_notification,
                            hid_is_connected_update_cb, get_is_hid_connected);
ZMK_SUBSCRIPTION(widget_is_connected, is_connected_notification);

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID_TIME)
//  Listener para la hora
static struct time_notification get_time(const zmk_event_t *eh) {
    struct time_notification *notification = as_time_notification(eh);
    if (notification) {
        return *notification;
    }
    return (struct time_notification){.hour = 0, .minute = 0}; // Hora por defecto
}

static void hid_time_update_cb(struct time_notification time) {
    struct zmk_widget_screen *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) {
        widget->state.hour = time.hour;
        widget->state.minute = time.minute;
        draw_canvas(widget->obj, widget->cbuf, &widget->state);
    }
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_time, struct time_notification, hid_time_update_cb, get_time);
ZMK_SUBSCRIPTION(widget_time, time_notification);
#endif

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID_VOLUME)
//  Listener para el volumen
static struct volume_notification get_volume(const zmk_event_t *eh) {
    struct volume_notification *notification = as_volume_notification(eh);
    if (notification) {
        return *notification;
    }
    return (struct volume_notification){.value = 0}; // Volumen por defecto
}

static void hid_volume_update_cb(struct volume_notification volume) {
    struct zmk_widget_screen *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) {
        widget->state.volume = volume.value;
        draw_canvas(widget->obj, widget->cbuf, &widget->state);
    }
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_volume, struct volume_notification, hid_volume_update_cb,
                            get_volume);
ZMK_SUBSCRIPTION(widget_volume, volume_notification);
#endif

#ifdef CONFIG_NICE_OLED_WIDGET_RAW_HID_LAYOUT // Reutiliza la Kconfig de status.c

static struct layout_notification get_layout(const zmk_event_t *eh) {
    struct layout_notification *notification = as_layout_notification(eh);
    if (notification) {
        return *notification;
    }
    return (struct layout_notification){.value = 0}; // Layout por defecto
}

static void hid_layout_update_cb(struct layout_notification layout) {
    struct zmk_widget_screen *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) {
        widget->state.layout = layout.value;
        draw_canvas(widget->obj, widget->cbuf, &widget->state);
    }
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_layout, struct layout_notification, hid_layout_update_cb,
                            get_layout);
ZMK_SUBSCRIPTION(widget_layout, layout_notification);

#endif // CONFIG_NICE_OLED_WIDGET_RAW_HID_LAYOUT

#endif // CONFIG_NICE_OLED_WIDGET_RAW_HID

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID_WEATHER)

static void weather_status_update_cb(struct weather_notification weather) {
    struct zmk_widget_screen *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) {
        widget->state.temperature = weather.temperature;
        draw_canvas(widget->obj, widget->cbuf, &widget->state);
    }
}

static struct weather_notification weather_status_get_state(const zmk_event_t *eh) {
    const struct weather_notification *ev = as_weather_notification(eh);
    if (ev == NULL) {
        return (struct weather_notification){.temperature = 127};
    }
    return *ev;
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_weather_status, struct weather_notification,
                            weather_status_update_cb, weather_status_get_state);
ZMK_SUBSCRIPTION(widget_weather_status, weather_notification);

#endif

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_SPOTIFY_MACOS)

static void spotify_status_update_cb(struct spotify_notification spotify) {
    struct zmk_widget_screen *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) {
        memcpy(widget->state.media_player, spotify.media_player,
               sizeof(widget->state.media_player));
        draw_canvas(widget->obj, widget->cbuf, &widget->state);
    }
}

static struct spotify_notification spotify_status_get_state(const zmk_event_t *eh) {
    const struct spotify_notification *ev = as_spotify_notification(eh);
    if (ev == NULL) {
        return (struct spotify_notification){.media_player = ""};
    }
    return *ev;
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_spotify_status, struct spotify_notification,
                            spotify_status_update_cb, spotify_status_get_state);
ZMK_SUBSCRIPTION(widget_spotify_status, spotify_notification);

#endif

/**
 * hid indicators
 **/

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_HID_INDICATORS)
#include "hid_indicators.h"
static struct zmk_widget_hid_indicators hid_indicators_widget;
#endif

/**
 * Draw canvas
 **/

static void draw_canvas(lv_obj_t *widget, lv_color_t cbuf[], const struct status_state *state) {
    lv_obj_t *canvas = lv_obj_get_child(widget, 0);

    // Draw widgets
    draw_background(canvas);
    draw_output_status(canvas, state);
#if !IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_ALL) &&                    \
    !IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_ONLY) &&                   \
    !IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_AND_CENTRAL)
    draw_battery_status(canvas, state);
#endif

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_ALL) ||                     \
    IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_ONLY) ||                    \
    IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_AND_CENTRAL)
    draw_battery_text(canvas, state);
#endif

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM)
    draw_wpm_status(canvas, state);
#endif // IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM)
    draw_profile_status(canvas, state);
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_LAYER)
    draw_layer_status(canvas, state);
#endif

#ifdef CONFIG_NICE_OLED_WIDGET_RAW_HID
    draw_hid_status(canvas, state);

#endif // CONFIG_NICE_OLED_WIDGET_RAW_HID

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED)
    // Dibuja los modificadores si la nueva Kconfig está habilitada
    draw_mods_status(canvas, state);
#endif // <-- NUEVO

    // Rotate for horizontal display
    rotate_canvas(canvas, cbuf);
}

/**
 * Battery status
 **/

#if !IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_ALL) &&                    \
    !IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_ONLY) &&                   \
    !IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_AND_CENTRAL)

static void set_battery_status(struct zmk_widget_screen *widget,
                               struct battery_status_state state) {
#if IS_ENABLED(CONFIG_USB_DEVICE_STACK)
    widget->state.charging = state.usb_present;
#endif /* IS_ENABLED(CONFIG_USB_DEVICE_STACK) */

    widget->state.battery = state.level;

    draw_canvas(widget->obj, widget->cbuf, &widget->state);
}

static void battery_status_update_cb(struct battery_status_state state) {
    struct zmk_widget_screen *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) { set_battery_status(widget, state); }
}

static struct battery_status_state battery_status_get_state(const zmk_event_t *eh) {
    const struct zmk_battery_state_changed *ev = as_zmk_battery_state_changed(eh);

    return (struct battery_status_state){
        .level = (ev != NULL) ? ev->state_of_charge : zmk_battery_state_of_charge(),
#if IS_ENABLED(CONFIG_USB_DEVICE_STACK)
        .usb_present = zmk_usb_is_powered(),
#endif /* IS_ENABLED(CONFIG_USB_DEVICE_STACK) */
    };
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_battery_status, struct battery_status_state,
                            battery_status_update_cb, battery_status_get_state);

ZMK_SUBSCRIPTION(widget_battery_status, zmk_battery_state_changed);
#if IS_ENABLED(CONFIG_USB_DEVICE_STACK)
ZMK_SUBSCRIPTION(widget_battery_status, zmk_usb_conn_state_changed);
#endif /* IS_ENABLED(CONFIG_USB_DEVICE_STACK) */

#endif // !IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_ALL)

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_ALL) ||                     \
    IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_ONLY) ||                    \
    IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_AND_CENTRAL)

static void set_battery_status(struct zmk_widget_screen *widget, struct battery_state state) {
    if (state.source >= CONFIG_NICE_OLED_SPLIT_TOTAL_DEVICES) {
        return;
    }
    LOG_DBG("Source: %d, level: %d, usb: %d", state.source, state.level, state.usb_present);
    widget->state.batteries[state.source].level = state.level;
    widget->state.batteries[state.source].usb_present = state.usb_present;

    draw_canvas(widget->obj, widget->cbuf, &widget->state);
}

void battery_status_update_cb(struct battery_state state) {
    struct zmk_widget_screen *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) { set_battery_status(widget, state); }
}

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_ALL) ||                     \
    IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_ONLY) ||                    \
    IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_AND_CENTRAL)
static struct battery_state peripheral_battery_status_get_state(const zmk_event_t *eh) {
    const struct zmk_peripheral_battery_state_changed *ev =
        as_zmk_peripheral_battery_state_changed(eh);
    return (struct battery_state){
        .source = ev->source + 1,
        .level = ev->state_of_charge,
    };
}
#endif

static struct battery_state central_battery_status_get_state(const zmk_event_t *eh) {
    const struct zmk_battery_state_changed *ev = as_zmk_battery_state_changed(eh);
    return (struct battery_state){
        .source = 0,
        .level = (ev != NULL) ? ev->state_of_charge : zmk_battery_state_of_charge(),
#if IS_ENABLED(CONFIG_USB_DEVICE_STACK)
        .usb_present = zmk_usb_is_powered(),
#endif /* IS_ENABLED(CONFIG_USB_DEVICE_STACK) */
    };
}

static struct battery_state battery_status_get_state(const zmk_event_t *eh) {
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_ALL) ||                     \
    IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_ONLY) ||                    \
    IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_AND_CENTRAL)
    if (as_zmk_peripheral_battery_state_changed(eh) != NULL) {
        return peripheral_battery_status_get_state(eh);
    } else {
        return central_battery_status_get_state(eh);
    }
#else
    return central_battery_status_get_state(eh);
#endif
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_battery_status, struct battery_state, battery_status_update_cb,
                            battery_status_get_state)

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_ALL) ||                     \
    IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_ONLY) ||                    \
    IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_AND_CENTRAL)
ZMK_SUBSCRIPTION(widget_battery_status, zmk_peripheral_battery_state_changed);
#endif
ZMK_SUBSCRIPTION(widget_battery_status, zmk_battery_state_changed);
ZMK_SUBSCRIPTION(widget_battery_status, zmk_usb_conn_state_changed);
#endif
// TODO: batt END

/**
 * Layer status
 **/

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_LAYER)
static void set_layer_status(struct zmk_widget_screen *widget, struct layer_status_state state) {
    widget->state.layer_index = state.index;
    widget->state.layer_label = state.label;

    draw_canvas(widget->obj, widget->cbuf, &widget->state);
}

static void layer_status_update_cb(struct layer_status_state state) {
    struct zmk_widget_screen *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) { set_layer_status(widget, state); }
}

static struct layer_status_state layer_status_get_state(const zmk_event_t *eh) {
    uint8_t index = zmk_keymap_highest_layer_active();
    return (struct layer_status_state){.index = index, .label = zmk_keymap_layer_name(index)};
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_layer_status, struct layer_status_state, layer_status_update_cb,
                            layer_status_get_state)

ZMK_SUBSCRIPTION(widget_layer_status, zmk_layer_state_changed);
#endif

/**
 * Output status
 **/

static void set_output_status(struct zmk_widget_screen *widget,
                              const struct output_status_state *state) {
    widget->state.selected_endpoint = state->selected_endpoint;
    widget->state.active_profile_index = state->active_profile_index;
    widget->state.active_profile_connected = state->active_profile_connected;
    widget->state.active_profile_bonded = state->active_profile_bonded;

    draw_canvas(widget->obj, widget->cbuf, &widget->state);
}

static void output_status_update_cb(struct output_status_state state) {
    struct zmk_widget_screen *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) { set_output_status(widget, &state); }
}

static struct output_status_state output_status_get_state(const zmk_event_t *_eh) {
    return (struct output_status_state){
        .selected_endpoint = zmk_endpoints_selected(),
        .active_profile_index = zmk_ble_active_profile_index(),
        .active_profile_connected = zmk_ble_active_profile_is_connected(),
        .active_profile_bonded = !zmk_ble_active_profile_is_open(),
    };
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_output_status, struct output_status_state,
                            output_status_update_cb, output_status_get_state)
ZMK_SUBSCRIPTION(widget_output_status, zmk_endpoint_changed);

#if IS_ENABLED(CONFIG_USB_DEVICE_STACK)
ZMK_SUBSCRIPTION(widget_output_status, zmk_usb_conn_state_changed);
#endif
#if defined(CONFIG_ZMK_BLE)
ZMK_SUBSCRIPTION(widget_output_status, zmk_ble_active_profile_changed);
#endif

/**
 * WPM status
 **/

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM)
static void set_wpm_status(struct zmk_widget_screen *widget, struct wpm_status_state state) {
    for (int i = 0; i < 9; i++) {
        widget->state.wpm[i] = widget->state.wpm[i + 1];
    }
    widget->state.wpm[9] = state.wpm;

    draw_canvas(widget->obj, widget->cbuf, &widget->state);
}

static void wpm_status_update_cb(struct wpm_status_state state) {
    struct zmk_widget_screen *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) { set_wpm_status(widget, state); }
}

struct wpm_status_state wpm_status_get_state(const zmk_event_t *eh) {
    return (struct wpm_status_state){.wpm = zmk_wpm_get_state()};
};

ZMK_DISPLAY_WIDGET_LISTENER(widget_wpm_status, struct wpm_status_state, wpm_status_update_cb,
                            wpm_status_get_state)
ZMK_SUBSCRIPTION(widget_wpm_status, zmk_wpm_state_changed);
#endif // IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM)

/**
 * Initialization
 **/

int zmk_widget_screen_init(struct zmk_widget_screen *widget, lv_obj_t *parent) {
    widget->obj = lv_obj_create(parent);
    lv_obj_set_size(widget->obj, CANVAS_HEIGHT, CANVAS_WIDTH);

    lv_obj_t *canvas = lv_canvas_create(widget->obj);
    lv_obj_align(canvas, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_canvas_set_buffer(canvas, widget->cbuf, CANVAS_HEIGHT, CANVAS_HEIGHT, LV_IMG_CF_TRUE_COLOR);

    sys_slist_append(&widgets, &widget->node);

    widget_battery_status_init();

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_LAYER)
    widget_layer_status_init();
#endif
    widget_output_status_init();
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM)
    widget_wpm_status_init();
#endif // IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM)

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM)

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_LUNA)
    zmk_widget_luna_init(&luna_widget, canvas);
    lv_obj_align(zmk_widget_luna_obj(&luna_widget), LV_ALIGN_TOP_LEFT, CONFIG_NICE_OLED_WIDGET_LUNA_CUSTOM_X, CONFIG_NICE_OLED_WIDGET_LUNA_CUSTOM_Y);
       // IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_LUNA)
#elif IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_BONGO_CAT)
    zmk_widget_wpm_bongo_cat_init(&wpm_bongo_cat_widget, canvas);
    lv_obj_align(zmk_widget_wpm_bongo_cat_obj(&wpm_bongo_cat_widget), LV_ALIGN_TOP_LEFT, CONFIG_NICE_OLED_WIDGET_BONGO_CAT_CUSTOM_X, CONFIG_NICE_OLED_WIDGET_BONGO_CAT_CUSTOM_Y);
       // IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_BONGO_CAT)
#endif

#endif // IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM)

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RESPONSIVE_BONGO_CAT)
    zmk_widget_responsive_bongo_cat_init(&responsive_bongo_cat_widget, canvas);
    lv_obj_align(zmk_widget_responsive_bongo_cat_obj(&responsive_bongo_cat_widget),
                 LV_ALIGN_TOP_LEFT, CONFIG_NICE_OLED_WIDGET_RESPONSIVE_BONGO_CAT_CUSTOM_X, CONFIG_NICE_OLED_WIDGET_RESPONSIVE_BONGO_CAT_CUSTOM_Y);
#endif

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_HID_INDICATORS)
    zmk_widget_hid_indicators_init(&hid_indicators_widget, canvas);
#endif

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_LUNA)
    zmk_widget_modifiers_init(&modifiers_widget, canvas); // Inicializar el widget de modifiers
#endif

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED) // <-- NUEVO
    widget_mods_status_init(); // <-- Inicializa el nuevo listener
#endif                         // <-- NUEVO

#ifdef CONFIG_NICE_OLED_WIDGET_RAW_HID
    widget_is_connected_init();
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID_TIME)
    widget_time_init();
#endif
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID_VOLUME)
    widget_volume_init();
#endif
#ifdef CONFIG_NICE_OLED_WIDGET_RAW_HID_LAYOUT
    widget_layout_init();
#endif
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID_WEATHER)
    widget_weather_status_init();
#endif
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_SPOTIFY_MACOS)
    widget_spotify_status_init();
#endif

    // Inicializa el estado HID a "desconectado" para el primer dibujo
    // Nota: Los valores iniciales (hora, vol, etc.) se obtendrán cuando lleguen los primeros
    // eventos.
    struct zmk_widget_screen *w;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, w, node) {
        w->state.is_connected = false; // Estado inicial por defecto
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID_WEATHER)
        w->state.temperature = 127; // Estado inicial para el clima (N/A)
#endif
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_SPOTIFY_MACOS)
        w->state.media_player[0] = '\0';
#endif
        // Otros campos HID se inicializarán a 0 o sus valores por defecto
    }
#endif // CONFIG_NICE_OLED_WIDGET_RAW_HID

    // tiene que estar siempre al final por la sobre exposicion!!
#if IS_ENABLED(CONFIG_NICE_OLED_SHOW_SLEEP_ART_ON_IDLE) ||                                         \
    IS_ENABLED(CONFIG_NICE_OLED_SHOW_SLEEP_ART_ON_SLEEP)
    zmk_widget_sleep_status_init(&sleep_status_widget, canvas);
    lv_obj_align(zmk_widget_sleep_status_obj(&sleep_status_widget), LV_ALIGN_TOP_LEFT, CONFIG_NICE_OLED_WIDGET_SLEEP_STATUS_CUSTOM_X, CONFIG_NICE_OLED_WIDGET_SLEEP_STATUS_CUSTOM_Y);
#endif

    return 0;
}

lv_obj_t *zmk_widget_screen_obj(struct zmk_widget_screen *widget) { return widget->obj; }
