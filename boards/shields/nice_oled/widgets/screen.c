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
static void draw_canvas(struct zmk_widget_screen *w);
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
    draw_canvas(widget);
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

#define DRAW_HID_STATUS_TEXT_ALIGN LV_TEXT_ALIGN_LEFT

#if IS_ENABLED(CONFIG_NICE_EPAPER_ON)
#define DRAW_HID_STATUS_FONTS &lv_font_montserrat_14
#elif CONFIG_NICE_OLED_WIDGET_RAW_HID_FONT_SIZE == 5
#define DRAW_HID_STATUS_FONTS &hid_font_5
#elif CONFIG_NICE_OLED_WIDGET_RAW_HID_FONT_SIZE == 6
#define DRAW_HID_STATUS_FONTS &hid_font_6
#elif CONFIG_NICE_OLED_WIDGET_RAW_HID_FONT_SIZE == 7
#define DRAW_HID_STATUS_FONTS &hid_font_7
#elif CONFIG_NICE_OLED_WIDGET_RAW_HID_FONT_SIZE == 8
#define DRAW_HID_STATUS_FONTS &hid_font_8
#else
#define DRAW_HID_STATUS_FONTS &pixel_operator_mono_12
#endif

#if IS_ENABLED(CONFIG_NICE_EPAPER_ON)
#define DRAW_HID_MEDIA_FONTS &lv_font_montserrat_14
#elif CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_FONT_SIZE == 5
#define DRAW_HID_MEDIA_FONTS &hid_font_5
#elif CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_FONT_SIZE == 6
#define DRAW_HID_MEDIA_FONTS &hid_font_6
#elif CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_FONT_SIZE == 7
#define DRAW_HID_MEDIA_FONTS &hid_font_7
#elif CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_FONT_SIZE == 8
#define DRAW_HID_MEDIA_FONTS &hid_font_8
#else
#define DRAW_HID_MEDIA_FONTS &pixel_operator_mono_12
#endif

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_SCROLL)
#define MEDIA_SCROLL_START_PAUSE 0
#define MEDIA_SCROLL_SCROLLING   1
#define MEDIA_SCROLL_END_PAUSE   2

/* Pause durations expressed as tick counts at SCROLL_SPEED_MS per tick. */
#define MEDIA_SCROLL_START_PAUSE_TICKS \
    (CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_SCROLL_START_PAUSE_MS / \
     CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_SCROLL_SPEED_MS)
#define MEDIA_SCROLL_END_PAUSE_TICKS \
    (CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_SCROLL_END_PAUSE_MS / \
     CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_SCROLL_SPEED_MS)

/* Temp buffer for the 32×32 media canvas rotation — same pattern as rotate_canvas. */
static lv_color_t media_cbuf_tmp[32 * 32];

/* Draw the current media text at scroll offset into the small 32×32 portrait canvas,
 * then rotate it to landscape — same as rotate_canvas but only for this small region.
 * LVGL marks only the 32×32 canvas dirty, so only that column range is flushed to the
 * SSD1306 (~128 bytes vs 512 for a full redraw). */
static void draw_media_canvas(struct zmk_widget_screen *w) {
    if (w->media_canvas == NULL) {
        return;
    }
    lv_obj_t *canvas = w->media_canvas;

    lv_draw_rect_dsc_t bg_dsc;
    init_rect_dsc(&bg_dsc, LVGL_BACKGROUND);
    lv_canvas_draw_rect(canvas, 0, 0, 32, 32, &bg_dsc);

    lv_draw_label_dsc_t label_dsc;
    init_label_dsc(&label_dsc, LVGL_FOREGROUND, DRAW_HID_MEDIA_FONTS, LV_TEXT_ALIGN_LEFT);

    lv_draw_line_dsc_t line_dsc;
    init_line_dsc(&line_dsc, LVGL_FOREGROUND, 1);

    // Top border
    lv_point_t top_pts[] = {{0, 0}, {31, 0}};
    lv_canvas_draw_line(canvas, top_pts, 2, &line_dsc);

    // Note icon: stem at (1,2)-(1,6), flag at (2,3), notehead at (0-1,5-6)
    lv_draw_rect_dsc_t note_dsc;
    init_rect_dsc(&note_dsc, LVGL_FOREGROUND);
    lv_canvas_draw_rect(canvas, 1, 2, 1, 5, &note_dsc);
    lv_canvas_draw_rect(canvas, 2, 3, 1, 1, &note_dsc);
    lv_canvas_draw_rect(canvas, 0, 5, 2, 2, &note_dsc);

    // "Media" header (x=6, y=2, max_w=21 leaves room for icon at x=28)
    lv_canvas_draw_text(canvas, 6, 2, 21, &label_dsc, "Media");

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_LINUX)
    // Title (y=8)
    {
        lv_point_t tsz;
        lv_txt_get_size(&tsz, w->state.media_player, DRAW_HID_MEDIA_FONTS, 0, 0,
                        LV_COORD_MAX, LV_TEXT_FLAG_NONE);
        int16_t t_scroll = (tsz.x > 32) ? MIN(w->media_scroll_offset, tsz.x - 32) : 0;
        lv_canvas_draw_text(canvas, -t_scroll, 8, LV_COORD_MAX, &label_dsc,
                            w->state.media_player);
    }

    // Artist (y=14)
    {
        lv_point_t asz;
        lv_txt_get_size(&asz, w->state.media_artist, DRAW_HID_MEDIA_FONTS, 0, 0,
                        LV_COORD_MAX, LV_TEXT_FLAG_NONE);
        int16_t a_scroll = (asz.x > 32) ? MIN(w->media_scroll_offset, asz.x - 32) : 0;
        lv_canvas_draw_text(canvas, -a_scroll, 14, LV_COORD_MAX, &label_dsc,
                            w->state.media_artist);
    }

    // "/" separator pixels (x=14-16, y=21-25)
    lv_draw_rect_dsc_t px_dsc;
    init_rect_dsc(&px_dsc, LVGL_FOREGROUND);
    lv_canvas_draw_rect(canvas, 16, 21, 1, 1, &px_dsc);
    lv_canvas_draw_rect(canvas, 16, 22, 1, 1, &px_dsc);
    lv_canvas_draw_rect(canvas, 15, 23, 1, 1, &px_dsc);
    lv_canvas_draw_rect(canvas, 14, 24, 1, 1, &px_dsc);
    lv_canvas_draw_rect(canvas, 14, 25, 1, 1, &px_dsc);

    // Current time (y=21) — LV_COORD_MAX prevents colon from being a word-break
    char cur_buf[8];
    uint16_t cur_sec = w->state.media_position_snap;
    if (w->state.media_play_status == 1 && w->media_position_ts > 0) {
        int64_t elapsed = (k_uptime_get() - w->media_position_ts) / 1000;
        uint32_t advanced = (uint32_t)cur_sec + (uint32_t)elapsed;
        cur_sec = (uint16_t)MIN(advanced, (uint32_t)w->state.media_total_time);
    }
    snprintf(cur_buf, sizeof(cur_buf), "%u:%02u", cur_sec / 60u, cur_sec % 60u);
    lv_canvas_draw_text(canvas, 0, 21, LV_COORD_MAX, &label_dsc, cur_buf);

    // Total time (y=21, x=17)
    char tot_buf[8];
    snprintf(tot_buf, sizeof(tot_buf), "%u:%02u",
             w->state.media_total_time / 60u, w->state.media_total_time % 60u);
    lv_canvas_draw_text(canvas, 18, 21, LV_COORD_MAX, &label_dsc, tot_buf);

    // Play/pause icon (x=28-30, y=2-6)
    lv_draw_rect_dsc_t icon_dsc;
    init_rect_dsc(&icon_dsc, LVGL_FOREGROUND);
    if (w->state.media_play_status == 2) {
        // Pause: two 1×5 bars
        lv_canvas_draw_rect(canvas, 28, 2, 1, 5, &icon_dsc);
        lv_canvas_draw_rect(canvas, 30, 2, 1, 5, &icon_dsc);
    } else if (w->state.media_play_status == 1) {
        // Play: right-pointing triangle
        lv_canvas_draw_rect(canvas, 28, 2, 1, 1, &icon_dsc);
        lv_canvas_draw_rect(canvas, 28, 3, 2, 1, &icon_dsc);
        lv_canvas_draw_rect(canvas, 28, 4, 3, 1, &icon_dsc);
        lv_canvas_draw_rect(canvas, 28, 5, 2, 1, &icon_dsc);
        lv_canvas_draw_rect(canvas, 28, 6, 1, 1, &icon_dsc);
    }

    // Progress bar at y=29,30 (between separator y=28 and bottom border y=31)
    if (w->state.media_total_time > 0) {
        int16_t fill_w = (int16_t)((uint32_t)cur_sec * 32u / w->state.media_total_time);
        if (fill_w > 32) fill_w = 32;
        uint8_t bar_phase = (w->state.media_play_status == 1)
                            ? (uint8_t)((k_uptime_get() / 500) % 2) : 0;
        lv_draw_rect_dsc_t bar_dsc;
        init_rect_dsc(&bar_dsc, LVGL_FOREGROUND);
        // Solid 2px end cap at leading edge (both rows)
        int16_t cap_start = fill_w - 2;
        if (cap_start < 0) cap_start = 0;
        if (fill_w > 0) {
            lv_canvas_draw_rect(canvas, cap_start, 29, fill_w - cap_start, 2, &bar_dsc);
        }
        // Dithered checkerboard: rows y=29 and y=30 are complementary
        for (int16_t x = 0; x < cap_start; x++) {
            if ((x + bar_phase) % 2 == 1) {
                lv_canvas_draw_rect(canvas, x, 29, 1, 1, &bar_dsc);
            } else {
                lv_canvas_draw_rect(canvas, x, 30, 1, 1, &bar_dsc);
            }
        }
    }
#endif /* CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_LINUX */

    // Separator line
    lv_point_t sep_pts[] = {{0, 28}, {31, 28}};
    lv_canvas_draw_line(canvas, sep_pts, 2, &line_dsc);

    // Bottom border
    lv_point_t bot_pts[] = {{0, 31}, {31, 31}};
    lv_canvas_draw_line(canvas, bot_pts, 2, &line_dsc);

    memcpy(media_cbuf_tmp, w->media_cbuf, sizeof(media_cbuf_tmp));

    lv_img_dsc_t img = {0};
    img.data = (void *)media_cbuf_tmp;
    img.header.cf = LV_IMG_CF_TRUE_COLOR;
    img.header.w = 32;
    img.header.h = 32;

    lv_canvas_fill_bg(canvas, LVGL_BACKGROUND, LV_OPA_COVER);
    lv_canvas_transform(canvas, &img, 900, LV_IMG_ZOOM_NONE, -1, 0, 16, 16, false);
}

static void media_scroll_timer_cb(lv_timer_t *timer) {
    struct zmk_widget_screen *w = (struct zmk_widget_screen *)timer->user_data;
    lv_point_t sz;
    lv_txt_get_size(&sz, w->state.media_player, DRAW_HID_MEDIA_FONTS, 0, 0,
                    LV_COORD_MAX, LV_TEXT_FLAG_NONE);
    int16_t max_offset = sz.x - 32;
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_LINUX)
    lv_txt_get_size(&sz, w->state.media_artist, DRAW_HID_MEDIA_FONTS, 0, 0,
                    LV_COORD_MAX, LV_TEXT_FLAG_NONE);
    int16_t artist_max = sz.x - 32;
    if (artist_max > max_offset) max_offset = artist_max;
#endif
    if (max_offset > 0) {
        switch (w->media_scroll_phase) {
        case MEDIA_SCROLL_START_PAUSE:
            if (++w->media_scroll_pause_ticks >= MEDIA_SCROLL_START_PAUSE_TICKS) {
                w->media_scroll_pause_ticks = 0;
                w->media_scroll_phase = MEDIA_SCROLL_SCROLLING;
            }
            break;
        case MEDIA_SCROLL_SCROLLING:
            if (++w->media_scroll_offset >= max_offset) {
                w->media_scroll_offset = max_offset;
                w->media_scroll_pause_ticks = 0;
                w->media_scroll_phase = MEDIA_SCROLL_END_PAUSE;
            }
            break;
        case MEDIA_SCROLL_END_PAUSE:
            if (++w->media_scroll_pause_ticks >= MEDIA_SCROLL_END_PAUSE_TICKS) {
                w->media_scroll_offset = 0;
                w->media_scroll_pause_ticks = 0;
                w->media_scroll_phase = MEDIA_SCROLL_START_PAUSE;
            }
            break;
        }
    } else {
        w->media_scroll_offset = 0;
    }
    draw_media_canvas(w);
}

static void media_active_start(struct zmk_widget_screen *w) {
    if (w->media_scroll_timer == NULL) {
        w->media_scroll_timer = lv_timer_create(media_scroll_timer_cb,
            CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_SCROLL_SPEED_MS, w);
    }
}

static void media_scroll_reset(struct zmk_widget_screen *w) {
    w->media_scroll_offset = 0;
    w->media_scroll_phase = MEDIA_SCROLL_START_PAUSE;
    w->media_scroll_pause_ticks = 0;
}

static void media_scroll_stop(struct zmk_widget_screen *w) {
    if (w->media_scroll_timer != NULL) {
        lv_timer_del(w->media_scroll_timer);
        w->media_scroll_timer = NULL;
    }
    w->media_scroll_offset = 0;
    if (w->media_canvas != NULL) {
        lv_canvas_fill_bg(w->media_canvas, LVGL_BACKGROUND, LV_OPA_COVER);
    }
}
#endif /* CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_SCROLL */

static void draw_hid_status(lv_obj_t *canvas, const struct status_state *state,
                            int16_t media_scroll_offset) {

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
#if (IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_SPOTIFY_MACOS) || \
     IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_LINUX)) && \
    !IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_SCROLL)
    lv_draw_label_dsc_t label_media;
    init_label_dsc(&label_media, LVGL_FOREGROUND, DRAW_HID_MEDIA_FONTS,
                   DRAW_HID_STATUS_TEXT_ALIGN);
#endif

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

#if (IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_SPOTIFY_MACOS) || \
     IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_LINUX)) && \
    !IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_SCROLL)
        lv_canvas_draw_text(canvas,
                            CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_CUSTOM_X - media_scroll_offset,
                            CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_CUSTOM_Y,
                            LV_COORD_MAX, &label_media, state->media_player);
#endif

    }
#if !IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MEDIA_CANVAS_FALLBACK)
    else {
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
#endif
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
    struct zmk_widget_screen *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) {
        widget->state.is_connected = is_connected.value;
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_SCROLL)
        if (!is_connected.value) {
            media_scroll_stop(widget);
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MEDIA_CANVAS_FALLBACK)
            if (widget->media_canvas != NULL) {
                lv_obj_add_flag(widget->media_canvas, LV_OBJ_FLAG_HIDDEN);
            }
#endif
        }
#endif
        // Hide WPM animation widgets when HID is connected; show when disconnected.
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_LUNA)
        if (is_connected.value) {
            lv_obj_add_flag(zmk_widget_luna_obj(&luna_widget), LV_OBJ_FLAG_HIDDEN);
        } else {
            lv_obj_clear_flag(zmk_widget_luna_obj(&luna_widget), LV_OBJ_FLAG_HIDDEN);
        }
#elif IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_BONGO_CAT)
        if (is_connected.value) {
            lv_obj_add_flag(zmk_widget_wpm_bongo_cat_obj(&wpm_bongo_cat_widget), LV_OBJ_FLAG_HIDDEN);
        } else {
            lv_obj_clear_flag(zmk_widget_wpm_bongo_cat_obj(&wpm_bongo_cat_widget), LV_OBJ_FLAG_HIDDEN);
        }
#endif
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RESPONSIVE_BONGO_CAT)
        if (is_connected.value) {
            lv_obj_add_flag(zmk_widget_responsive_bongo_cat_obj(&responsive_bongo_cat_widget), LV_OBJ_FLAG_HIDDEN);
        } else {
            lv_obj_clear_flag(zmk_widget_responsive_bongo_cat_obj(&responsive_bongo_cat_widget), LV_OBJ_FLAG_HIDDEN);
        }
#endif
        draw_canvas(widget);
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
        draw_canvas(widget);
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
        draw_canvas(widget);
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
        draw_canvas(widget);
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
        draw_canvas(widget);
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
        strncpy(widget->state.media_player, spotify.media_player,
                sizeof(widget->state.media_player) - 1);
        widget->state.media_player[sizeof(widget->state.media_player) - 1] = '\0';
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_SCROLL)
        media_scroll_reset(widget);
#endif
        draw_canvas(widget);
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

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_LINUX)

static void media_player_linux_update_cb(struct media_player_linux_notification notif) {
    struct zmk_widget_screen *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) {
        strncpy(widget->state.media_player, notif.media_player,
                sizeof(widget->state.media_player) - 1);
        widget->state.media_player[sizeof(widget->state.media_player) - 1] = '\0';
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_SCROLL)
        media_scroll_reset(widget);
        draw_media_canvas(widget);
#endif
        draw_canvas(widget);
    }
}

static struct media_player_linux_notification media_player_linux_get_state(const zmk_event_t *eh) {
    const struct media_player_linux_notification *ev = as_media_player_linux_notification(eh);
    if (ev == NULL) {
        return (struct media_player_linux_notification){.media_player = ""};
    }
    return *ev;
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_media_player_linux, struct media_player_linux_notification,
                            media_player_linux_update_cb, media_player_linux_get_state);
ZMK_SUBSCRIPTION(widget_media_player_linux, media_player_linux_notification);

static void media_extended_update_cb(struct media_extended_notification notif) {
    struct zmk_widget_screen *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) {
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_SCROLL)
        bool artist_changed = strncmp(widget->state.media_artist, notif.artist,
                                      sizeof(widget->state.media_artist)) != 0;
#endif
        widget->state.media_total_time    = notif.total_time;
        widget->state.media_position_snap = notif.position;
        widget->state.media_play_status   = notif.play_status;
        strncpy(widget->state.media_artist, notif.artist,
                sizeof(widget->state.media_artist) - 1);
        widget->state.media_artist[sizeof(widget->state.media_artist) - 1] = '\0';
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_SCROLL)
        widget->media_position_ts = k_uptime_get();
        if (notif.play_status == 1) {
            // Playing: show canvas and start scroll/update timer
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MEDIA_CANVAS_FALLBACK)
            if (widget->media_canvas != NULL) {
                lv_obj_clear_flag(widget->media_canvas, LV_OBJ_FLAG_HIDDEN);
            }
#endif
            media_active_start(widget);
        } else {
            // Paused or stopped: stop timer
            if (widget->media_scroll_timer != NULL) {
                lv_timer_del(widget->media_scroll_timer);
                widget->media_scroll_timer = NULL;
            }
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MEDIA_CANVAS_FALLBACK)
            if (notif.play_status == 0) {
                if (widget->media_canvas != NULL) {
                    lv_obj_add_flag(widget->media_canvas, LV_OBJ_FLAG_HIDDEN);
                }
            } else {
                // Paused: keep visible but frozen
                if (widget->media_canvas != NULL) {
                    lv_obj_clear_flag(widget->media_canvas, LV_OBJ_FLAG_HIDDEN);
                }
            }
#endif
        }
        if (artist_changed) {
            media_scroll_reset(widget);
        }
        draw_media_canvas(widget);
#endif
    }
}

static struct media_extended_notification media_extended_get_state(const zmk_event_t *eh) {
    const struct media_extended_notification *ev = as_media_extended_notification(eh);
    if (ev == NULL) {
        return (struct media_extended_notification){0};
    }
    return *ev;
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_media_extended, struct media_extended_notification,
                            media_extended_update_cb, media_extended_get_state);
ZMK_SUBSCRIPTION(widget_media_extended, media_extended_notification);

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

static void draw_canvas(struct zmk_widget_screen *w) {
    lv_obj_t *canvas = lv_obj_get_child(w->obj, 0);
    const struct status_state *state = &w->state;

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
    draw_hid_status(canvas, state,
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_SCROLL)
        w->media_scroll_offset
#else
        0
#endif
    );
#endif // CONFIG_NICE_OLED_WIDGET_RAW_HID

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED)
    // Dibuja los modificadores si la nueva Kconfig está habilitada
    draw_mods_status(canvas, state);
#endif // <-- NUEVO

    // Rotate for horizontal display
    rotate_canvas(canvas, w->cbuf);
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

    draw_canvas(widget);
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

    draw_canvas(widget);
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

    draw_canvas(widget);
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

    draw_canvas(widget);
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

    draw_canvas(widget);
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
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_LINUX)
    widget_media_player_linux_init();
    widget_media_extended_init();
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
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_SPOTIFY_MACOS) || \
    IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_LINUX)
        w->state.media_player[0] = '\0';
#endif
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_LINUX)
        w->state.media_artist[0] = '\0';
        w->state.media_play_status = 0;
        w->state.media_total_time = 0;
        w->state.media_position_snap = 0;
#endif
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_SCROLL)
        w->media_scroll_offset = 0;
        w->media_scroll_phase = MEDIA_SCROLL_START_PAUSE;
        w->media_scroll_pause_ticks = 0;
        w->media_scroll_timer = NULL;
        w->media_position_ts = 0;

        /* Small 32×32 canvas for the media text row. Positioned in LVGL landscape so that
         * portrait y=CUSTOM_Y aligns with its right edge after the same 90° CCW rotation
         * used by rotate_canvas. Only this canvas is dirty on scroll ticks, so LVGL flushes
         * ~128 bytes to the SSD1306 instead of the full 512 bytes. */
        lv_obj_t *media_canvas = lv_canvas_create(w->obj);
        lv_canvas_set_buffer(media_canvas, w->media_cbuf, 32, 32, LV_IMG_CF_TRUE_COLOR);
        lv_obj_set_pos(media_canvas,
            CANVAS_HEIGHT - 32 - CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_CUSTOM_Y,
            0);
        lv_canvas_fill_bg(media_canvas, LVGL_BACKGROUND, LV_OPA_COVER);
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MEDIA_CANVAS_FALLBACK)
        lv_obj_add_flag(media_canvas, LV_OBJ_FLAG_HIDDEN);
#endif
        w->media_canvas = media_canvas;
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
