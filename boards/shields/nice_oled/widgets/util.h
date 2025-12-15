#pragma once

#include <lvgl.h>
#include <zmk/endpoints.h>

// nice_epaper and nice_oled standard width = 68, height = 160
#define CANVAS_WIDTH CONFIG_NICE_OLED_CUSTOM_CANVAS_WIDTH
#define CANVAS_HEIGHT CONFIG_NICE_OLED_CUSTOM_CANVAS_HEIGHT

#define LVGL_BACKGROUND                                                                            \
    IS_ENABLED(CONFIG_NICE_OLED_WIDGET_INVERTED) ? lv_color_black() : lv_color_white()
#define LVGL_FOREGROUND                                                                            \
    IS_ENABLED(CONFIG_NICE_OLED_WIDGET_INVERTED) ? lv_color_white() : lv_color_black()

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_ALL) ||                     \
    IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_ONLY) ||                    \
    IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_AND_CENTRAL)
struct battery_info {
    uint8_t source;
    uint8_t level;
    bool usb_present;
};
#endif

struct status_state {
    uint8_t battery;
    bool charging;
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_ALL) ||                     \
    IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_ONLY) ||                    \
    IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_AND_CENTRAL)
// Calcula el número total de dispositivos (central + periféricos)
#define CONFIG_NICE_OLED_SPLIT_TOTAL_DEVICES (1 + CONFIG_ZMK_SPLIT_BLE_CENTRAL_PERIPHERALS)
    // Usa la macro para definir el tamaño del array
    struct battery_info batteries[CONFIG_NICE_OLED_SPLIT_TOTAL_DEVICES];
#endif

#if !IS_ENABLED(CONFIG_ZMK_SPLIT) || IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
    struct zmk_endpoint_instance selected_endpoint;
    int active_profile_index;
    bool active_profile_connected;
    bool active_profile_bonded;
    uint8_t layer_index;
    const char *layer_label;
    uint8_t wpm[10];
#ifdef CONFIG_NICE_OLED_WIDGET_RAW_HID
    bool is_connected;
    uint8_t hour;
    uint8_t minute;
    uint8_t volume;
    uint8_t layout;
    int8_t temperature;
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_SPOTIFY_MACOS)
    char media_player[11];
#endif
#endif
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED)
    uint8_t mod_state;
#endif

#else
    bool connected;
#endif
};

void to_uppercase(char *str);
void rotate_canvas(lv_obj_t *canvas, lv_color_t cbuf[]);
void draw_background(lv_obj_t *canvas);
void init_rect_dsc(lv_draw_rect_dsc_t *rect_dsc, lv_color_t bg_color);
void init_line_dsc(lv_draw_line_dsc_t *line_dsc, lv_color_t color, uint8_t width);
void init_label_dsc(lv_draw_label_dsc_t *label_dsc, lv_color_t color, const lv_font_t *font,
                    lv_text_align_t align);
