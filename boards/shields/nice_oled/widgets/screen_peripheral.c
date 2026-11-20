#include <zephyr/kernel.h>

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#include <zmk/battery.h>
#include <zmk/ble.h>
#include <zmk/display.h>
#include <zmk/event_manager.h>
#include <zmk/events/battery_state_changed.h>
#include <zmk/events/split_peripheral_status_changed.h>
#include <zmk/events/usb_conn_state_changed.h>
#include <zmk/split/bluetooth/peripheral.h>
#include <zmk/usb.h>
#include <dt-bindings/zmk/modifiers.h>

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_PERIPHERAL_INDICATORS)
#include <zmk/events/modifiers_peripheral_state_changed.h>
#include <zmk/modifiers_peripheral.h>
#endif

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_PROFILE_PERIPHERAL)
#include <zmk/events/profile_peripheral_state_changed.h>
#include <zmk/profile_peripheral.h>
#endif

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_LAYER_PERIPHERAL)
#include <zmk/events/layer_peripheral_state_changed.h>
#include <zmk/layer_peripheral.h>
#endif

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_BATTERY_PERIPHERAL)
#include <zmk/events/central_battery_peripheral_state_changed.h>
#include <zmk/peripheral_status_sync.h>
#endif

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_POINTER_STATUS_PERIPHERAL)
#include <zmk/events/pointer_status_changed.h>
#include <zmk/pointer_status.h>
#endif

#define NICE_OLED_WIDGET_MODIFIERS_PERIPHERAL_FIXED_ACTIVE                                           \
    (IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_PERIPHERAL_INDICATORS_FIXED) &&                    \
     !IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_PERIPHERAL_INDICATORS_LUNA) &&                    \
     !IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_PERIPHERAL_INDICATORS_BONGO_CAT))

#include "animation.h"
#include "battery.h"
#include "output.h"
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_PROFILE_PERIPHERAL)
#include "profile.h"
#endif
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_LAYER_PERIPHERAL)
#include "layer.h"
#endif
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RESPONSIVE_BONGO_CAT) ||                                   \
    IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RESPONSIVE_BONGO_CAT_PERIPHERAL)
#include "responsive_bongo_cat.h"
#endif
#include "screen_peripheral.h"

static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);

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

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_PERIPHERAL) &&                                          \
    IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_PERIPHERAL_LUNA)
#include "luna.h"
static struct zmk_widget_luna luna_widget;
#endif

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_PERIPHERAL) &&                                          \
    IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_PERIPHERAL_NUMBER)
#include <zmk/display/widgets/wpm_peripheral_status.h>
static struct zmk_widget_wpm_peripheral_status wpm_peripheral_status_widget;
#endif

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_PERIPHERAL) &&                                          \
    IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_PERIPHERAL_BONGO_CAT)
#include "bongo_cat.h"
static struct zmk_widget_wpm_bongo_cat wpm_bongo_cat_widget;
#endif

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_PERIPHERAL_INDICATORS_LUNA) ||                    \
    IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_PERIPHERAL_INDICATORS_BONGO_CAT)
#include "modifiers.h"
static struct zmk_widget_modifiers modifiers_widget;
#endif

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_HID_INDICATORS)
#include "hid_indicators.h"
static struct zmk_widget_hid_indicators hid_indicators_widget;
#endif

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RESPONSIVE_BONGO_CAT) ||                                   \
    IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RESPONSIVE_BONGO_CAT_PERIPHERAL)
static struct zmk_widget_responsive_bongo_cat responsive_bongo_cat_widget;
#endif

#if NICE_OLED_WIDGET_MODIFIERS_PERIPHERAL_FIXED_ACTIVE
struct modifiers_peripheral_status_state {
    uint8_t mods;
};

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_PERIPHERAL_INDICATORS_FIXED_SYMBOL)
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

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_PERIPHERAL_INDICATORS_FIXED_SYMBOL_WINDOWS)
static const lv_img_dsc_t *mod_imgs_normal[4] = {&control_0, &shift_0, &alt_0, &win_0};
static const lv_img_dsc_t *mod_imgs_active[4] = {&control_white_0, &shift_white_0, &alt_white_0,
                                                 &win_white_0};
#else
static const lv_img_dsc_t *mod_imgs_normal[4] = {&control_0, &shift_0, &opt_0, &cmd_0};
static const lv_img_dsc_t *mod_imgs_active[4] = {&control_white_0, &shift_white_0, &opt_white_0,
                                                 &cmd_white_0};
#endif
#endif

static void draw_modifiers_peripheral_status(lv_obj_t *canvas, const struct status_state *state) {
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_PERIPHERAL_INDICATORS_FIXED_SYMBOL)
    lv_draw_image_dsc_t img_dsc;
    lv_draw_image_dsc_init(&img_dsc);

    const int img_size = 14;
    const int spacing = 2;

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_PERIPHERAL_INDICATORS_FIXED_VER)
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_PERIPHERAL_INDICATORS_FIXED_VER_ALIGN_RIGHT)
#if IS_ENABLED(CONFIG_NICE_EPAPER_ON)
    const int base_x = 68 - img_size - 2;
#else
    const int base_x = 128 - img_size - 2;
#endif
#elif IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_PERIPHERAL_INDICATORS_FIXED_VER_ALIGN_LEFT)
    const int base_x = CONFIG_NICE_OLED_WIDGET_MODIFIERS_CUSTOM_X;
#else
#if IS_ENABLED(CONFIG_NICE_EPAPER_ON)
    const int base_x = (68 - img_size) / 2;
#else
    const int base_x = (128 - img_size) / 2;
#endif
#endif
    const int base_y = 62;

    for (int i = 0; i < 4; i++) {
        bool selected = (state->mod_state >> i) & 1 || (state->mod_state >> (i + 4)) & 1;
        int current_x = base_x;
        int current_y = base_y + i * (img_size + spacing);
        const lv_img_dsc_t *img = selected ? mod_imgs_active[i] : mod_imgs_normal[i];
        canvas_draw_img(canvas, current_x, current_y, img, &img_dsc);
    }

#elif IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_PERIPHERAL_INDICATORS_FIXED_HOR)
    const int base_x = CONFIG_NICE_OLED_WIDGET_MODIFIERS_CUSTOM_X;
    const int base_y = CONFIG_NICE_OLED_WIDGET_MODIFIERS_CUSTOM_Y;

    for (int i = 0; i < 4; i++) {
        bool selected = (state->mod_state >> i) & 1 || (state->mod_state >> (i + 4)) & 1;
        int current_x = base_x + i * (img_size + spacing);
        int current_y = base_y;
        const lv_img_dsc_t *img = selected ? mod_imgs_active[i] : mod_imgs_normal[i];
        canvas_draw_img(canvas, current_x, current_y, img, &img_dsc);
    }

#else
    const int base_x = CONFIG_NICE_OLED_WIDGET_MODIFIERS_CUSTOM_X;
    const int base_y = CONFIG_NICE_OLED_WIDGET_MODIFIERS_CUSTOM_Y;

    static const int offsets_box[4][2] = {
        {0, 0},
        {img_size + spacing, 0},
        {0, img_size + spacing},
        {img_size + spacing, img_size + spacing},
    };

    for (int i = 0; i < 4; i++) {
        bool selected = (state->mod_state >> i) & 1 || (state->mod_state >> (i + 4)) & 1;
        int current_x = base_x + offsets_box[i][0];
        int current_y = base_y + offsets_box[i][1];
        const lv_img_dsc_t *img = selected ? mod_imgs_active[i] : mod_imgs_normal[i];
        canvas_draw_img(canvas, current_x, current_y, img, &img_dsc);
    }
#endif

#else
    const char *items[4] = {"C", "S", "A", "G"};

    lv_draw_rect_dsc_t rect_black_dsc;
    init_rect_dsc(&rect_black_dsc, LVGL_BACKGROUND);
    lv_draw_rect_dsc_t rect_white_dsc;
    init_rect_dsc(&rect_white_dsc, LVGL_FOREGROUND);
    lv_draw_label_dsc_t mod_dsc;
    init_label_dsc(&mod_dsc, LVGL_FOREGROUND, &lv_font_unscii_8, LV_TEXT_ALIGN_CENTER);
    lv_draw_label_dsc_t mod_dsc_black;
    init_label_dsc(&mod_dsc_black, LVGL_BACKGROUND, &lv_font_unscii_8, LV_TEXT_ALIGN_CENTER);

    const int box_width = 12;
    const int box_height = 14;
    const int inner_box_offset = 2;
    const int text_offset_y = 4;
    const int inner_box_width = box_width - (2 * inner_box_offset);
    const int inner_box_height = box_height - (2 * inner_box_offset);

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_PERIPHERAL_INDICATORS_FIXED_VER)
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_PERIPHERAL_INDICATORS_FIXED_VER_ALIGN_RIGHT)
#if IS_ENABLED(CONFIG_NICE_EPAPER_ON)
    const int base_x = 68 - box_width - 2;
#else
    const int base_x = 128 - box_width - 2;
#endif
#elif IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_PERIPHERAL_INDICATORS_FIXED_VER_ALIGN_LEFT)
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

        canvas_draw_rect(canvas, current_x, current_y, box_width, box_height, &rect_black_dsc);
        if (selected && inner_box_width > 0 && inner_box_height > 0) {
            canvas_draw_rect(canvas, current_x + inner_box_offset,
                                current_y + inner_box_offset, inner_box_width, inner_box_height,
                                &rect_white_dsc);
        }
        canvas_draw_text(canvas, current_x, current_y + text_offset_y, box_width,
                            selected ? &mod_dsc_black : &mod_dsc, items[i]);
    }

#elif IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_PERIPHERAL_INDICATORS_FIXED_HOR)
    const int base_x = CONFIG_NICE_OLED_WIDGET_MODIFIERS_CUSTOM_X;
    const int base_y = CONFIG_NICE_OLED_WIDGET_MODIFIERS_CUSTOM_Y;

    for (int i = 0; i < 4; i++) {
        bool selected = (state->mod_state >> i) & 1 || (state->mod_state >> (i + 4)) & 1;
        int current_x = base_x + i * (box_width + 2);
        int current_y = base_y;

        canvas_draw_rect(canvas, current_x, current_y, box_width, box_height, &rect_black_dsc);
        if (selected && inner_box_width > 0 && inner_box_height > 0) {
            canvas_draw_rect(canvas, current_x + inner_box_offset,
                                current_y + inner_box_offset, inner_box_width, inner_box_height,
                                &rect_white_dsc);
        }
        canvas_draw_text(canvas, current_x, current_y + text_offset_y, box_width,
                            selected ? &mod_dsc_black : &mod_dsc, items[i]);
    }

#else
    const int base_x = CONFIG_NICE_OLED_WIDGET_MODIFIERS_CUSTOM_X;
    const int base_y = CONFIG_NICE_OLED_WIDGET_MODIFIERS_CUSTOM_Y;

    static const int offsets_box[4][2] = {
        {0, 0},
        {box_width + 2, 0},
        {0, box_height + 2},
        {box_width + 2, box_height + 2},
    };

    for (int i = 0; i < 4; i++) {
        bool selected = (state->mod_state >> i) & 1 || (state->mod_state >> (i + 4)) & 1;
        int current_x = base_x + offsets_box[i][0];
        int current_y = base_y + offsets_box[i][1];

        canvas_draw_rect(canvas, current_x, current_y, box_width, box_height, &rect_black_dsc);
        if (selected && inner_box_width > 0 && inner_box_height > 0) {
            canvas_draw_rect(canvas, current_x + inner_box_offset,
                                current_y + inner_box_offset, inner_box_width, inner_box_height,
                                &rect_white_dsc);
        }
        canvas_draw_text(canvas, current_x, current_y + text_offset_y, box_width,
                            selected ? &mod_dsc_black : &mod_dsc, items[i]);
    }
#endif
#endif
}
#endif

/**
 * Draw canvas
 **/

static void draw_canvas(lv_obj_t *widget, uint8_t cbuf[], const struct status_state *state) {
    lv_obj_t *canvas = lv_obj_get_child(widget, 0);

    // Draw widgets
    draw_background(canvas);
    draw_output_status(canvas, state);
    draw_battery_status(canvas, state);
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_PROFILE_PERIPHERAL)
    draw_profile_status(canvas, state);
#endif
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_LAYER_PERIPHERAL)
    draw_layer_status(canvas, state);
#endif
#if NICE_OLED_WIDGET_MODIFIERS_PERIPHERAL_FIXED_ACTIVE
    draw_modifiers_peripheral_status(canvas, state);
#endif
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_POINTER_STATUS_PERIPHERAL)
    draw_pointer_status(canvas, state->pointer_x, state->pointer_y, state->pointer_buttons,
                        state->pointer_ripple);
#endif

    // Rotate for horizontal display
    rotate_canvas(canvas, cbuf);
}

// One redraw per burst of changes (idea of @chalukyaj): an update only stores its state and asks
// for a redraw. The work item is queued once on the display queue, so the updates that are
// already waiting there run first and the canvas is drawn and rotated a single time for all.
static void redraw_work_cb(struct k_work *work) {
    struct zmk_widget_screen *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) {
        draw_canvas(widget->obj, widget->cbuf, &widget->state);
    }
}

static K_WORK_DEFINE(redraw_work, redraw_work_cb);

static void request_redraw(void) { k_work_submit_to_queue(zmk_display_work_q(), &redraw_work); }

/**
 * Battery status
 **/

static void set_battery_status(struct zmk_widget_screen *widget,
                               struct battery_status_state state) {

#if IS_ENABLED(CONFIG_USB_DEVICE_STACK)
    widget->state.charging = state.usb_present;
#endif /* IS_ENABLED(CONFIG_USB_DEVICE_STACK) */

    widget->state.battery = state.level;

    request_redraw();

    // draw_animation(widget->obj, widget);

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_SMART_BATTERY)
    if (widget->state.charging) {
        // mostrar
        // lv_obj_remove_flag(widget->obj, LV_OBJ_FLAG_HIDDEN);
        animation_smart_battery_on(widget->obj);

    } else {
        // quitar
        // lv_obj_add_flag(widget->art, LV_OBJ_FLAG_HIDDEN);
        animation_smart_battery_off(widget->obj);
    }
#endif
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

/**
 * Peripheral status
 **/

static struct peripheral_status_state get_state(const zmk_event_t *_eh) {
    return (struct peripheral_status_state){.connected = zmk_split_bt_peripheral_is_connected()};
}

static void set_connection_status(struct zmk_widget_screen *widget,
                                  struct peripheral_status_state state) {
    widget->state.connected = state.connected;

    request_redraw();
}

static void output_status_update_cb(struct peripheral_status_state state) {
    struct zmk_widget_screen *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) { set_connection_status(widget, state); }
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_peripheral_status, struct peripheral_status_state,
                            output_status_update_cb, get_state)
ZMK_SUBSCRIPTION(widget_peripheral_status, zmk_split_peripheral_status_changed);

#if NICE_OLED_WIDGET_MODIFIERS_PERIPHERAL_FIXED_ACTIVE
static void set_modifiers_peripheral_status(struct zmk_widget_screen *widget,
                                            struct modifiers_peripheral_status_state state) {
    widget->state.mod_state = state.mods;
    request_redraw();
}

static void modifiers_peripheral_status_update_cb(struct modifiers_peripheral_status_state state) {
    struct zmk_widget_screen *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) {
        set_modifiers_peripheral_status(widget, state);
    }
}

static struct modifiers_peripheral_status_state
modifiers_peripheral_status_get_state(const zmk_event_t *eh) {
    return (struct modifiers_peripheral_status_state){.mods = zmk_modifiers_peripheral_get_state()};
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_modifiers_peripheral_status,
                            struct modifiers_peripheral_status_state,
                            modifiers_peripheral_status_update_cb,
                            modifiers_peripheral_status_get_state)
ZMK_SUBSCRIPTION(widget_modifiers_peripheral_status, zmk_modifiers_peripheral_state_changed);
#endif

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_PROFILE_PERIPHERAL)
static void set_profile_peripheral_status(struct zmk_widget_screen *widget,
                                          struct zmk_profile_peripheral_state state) {
    widget->state.active_profile_index = state.index;
    widget->state.active_profile_connected = state.connected;
    widget->state.active_profile_bonded = state.bonded;
    request_redraw();
}

static void profile_peripheral_status_update_cb(struct zmk_profile_peripheral_state state) {
    struct zmk_widget_screen *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) {
        set_profile_peripheral_status(widget, state);
    }
}

static struct zmk_profile_peripheral_state profile_peripheral_status_get_state(const zmk_event_t *eh) {
    return zmk_profile_peripheral_get_state();
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_profile_peripheral_status,
                            struct zmk_profile_peripheral_state,
                            profile_peripheral_status_update_cb,
                            profile_peripheral_status_get_state)
ZMK_SUBSCRIPTION(widget_profile_peripheral_status, zmk_profile_peripheral_state_changed);
#endif

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_LAYER_PERIPHERAL)
static void set_layer_peripheral_status(struct zmk_widget_screen *widget,
                                        struct zmk_layer_peripheral_state state) {
    widget->state.layer_index = state.index;
    widget->state.layer_label = state.label;
    request_redraw();
}

static void layer_peripheral_status_update_cb(struct zmk_layer_peripheral_state state) {
    struct zmk_widget_screen *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) {
        set_layer_peripheral_status(widget, state);
    }
}

static struct zmk_layer_peripheral_state layer_peripheral_status_get_state(const zmk_event_t *eh) {
    return zmk_layer_peripheral_get_state();
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_layer_peripheral_status,
                            struct zmk_layer_peripheral_state,
                            layer_peripheral_status_update_cb,
                            layer_peripheral_status_get_state)
ZMK_SUBSCRIPTION(widget_layer_peripheral_status, zmk_layer_peripheral_state_changed);
#endif

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_BATTERY_PERIPHERAL)
static void set_central_battery_peripheral_status(
    struct zmk_widget_screen *widget, struct zmk_central_battery_peripheral_state_changed state) {
    widget->state.central_battery_level = state.level;
    widget->state.central_battery_charging = state.charging;
    request_redraw();
}

static void central_battery_peripheral_status_update_cb(
    struct zmk_central_battery_peripheral_state_changed state) {
    struct zmk_widget_screen *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) {
        set_central_battery_peripheral_status(widget, state);
    }
}

static struct zmk_central_battery_peripheral_state_changed
central_battery_peripheral_status_get_state(const zmk_event_t *eh) {
    return (struct zmk_central_battery_peripheral_state_changed){
        .level = zmk_peripheral_status_sync_get_central_battery(),
        .charging = zmk_peripheral_status_sync_get_central_battery_charging()};
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_central_battery_peripheral_status,
                            struct zmk_central_battery_peripheral_state_changed,
                            central_battery_peripheral_status_update_cb,
                            central_battery_peripheral_status_get_state)
ZMK_SUBSCRIPTION(widget_central_battery_peripheral_status,
                 zmk_central_battery_peripheral_state_changed);
#endif

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_POINTER_STATUS_PERIPHERAL)
static void set_pointer_peripheral_status(struct zmk_widget_screen *widget,
                                          struct zmk_pointer_status_state state) {
    widget->state.pointer_x = state.x;
    widget->state.pointer_y = state.y;
    widget->state.pointer_buttons = state.buttons;
    widget->state.pointer_ripple = state.ripple;
    request_redraw();
}

static void pointer_peripheral_status_update_cb(struct zmk_pointer_status_state state) {
    struct zmk_widget_screen *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) {
        set_pointer_peripheral_status(widget, state);
    }
}

static struct zmk_pointer_status_state pointer_peripheral_status_get_state(const zmk_event_t *eh) {
    return zmk_pointer_status_get_state();
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_pointer_peripheral_status, struct zmk_pointer_status_state,
                            pointer_peripheral_status_update_cb,
                            pointer_peripheral_status_get_state)
ZMK_SUBSCRIPTION(widget_pointer_peripheral_status, zmk_pointer_status_changed);
#endif

/**
 * Initialization
 **/

int zmk_widget_screen_init(struct zmk_widget_screen *widget, lv_obj_t *parent) {
    widget->obj = lv_obj_create(parent);
    lv_obj_set_size(widget->obj, CANVAS_HEIGHT, CANVAS_WIDTH);

    lv_obj_t *canvas = lv_canvas_create(widget->obj);
    lv_obj_align(canvas, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_canvas_set_buffer(canvas, widget->cbuf, CANVAS_HEIGHT, CANVAS_HEIGHT, CANVAS_COLOR_FORMAT);

    sys_slist_append(&widgets, &widget->node);

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_BATTERY_PERIPHERAL)
    widget->state.central_battery_level = ZMK_PERIPHERAL_STATUS_SYNC_BATTERY_UNKNOWN;
    widget->state.central_battery_charging = false;
#endif

#if (IS_ENABLED(CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL) ||                                  \
     IS_ENABLED(CONFIG_NICE_OLED_WIDGET_STATIC_IMAGE_PERIPHERAL)) &&                              \
    !IS_ENABLED(CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_SMART_BATTERY)
    draw_animation(canvas, widget);
#endif
    widget_battery_status_init();
    widget_peripheral_status_init();
#if NICE_OLED_WIDGET_MODIFIERS_PERIPHERAL_FIXED_ACTIVE
    widget_modifiers_peripheral_status_init();
#endif
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_PROFILE_PERIPHERAL)
    widget_profile_peripheral_status_init();
#endif
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_LAYER_PERIPHERAL)
    widget_layer_peripheral_status_init();
#endif
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_BATTERY_PERIPHERAL)
    widget_central_battery_peripheral_status_init();
#endif
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_POINTER_STATUS_PERIPHERAL)
    widget_pointer_peripheral_status_init();
#endif

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_PERIPHERAL) &&                                          \
    IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_PERIPHERAL_LUNA)
    zmk_widget_luna_init(&luna_widget, canvas);
    lv_obj_align(zmk_widget_luna_obj(&luna_widget), LV_ALIGN_TOP_LEFT, CONFIG_NICE_OLED_WIDGET_LUNA_CUSTOM_X, CONFIG_NICE_OLED_WIDGET_LUNA_CUSTOM_Y);
#endif

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_PERIPHERAL) &&                                          \
    IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_PERIPHERAL_NUMBER)
    zmk_widget_wpm_peripheral_status_init(&wpm_peripheral_status_widget, canvas);
    lv_obj_align(zmk_widget_wpm_peripheral_status_obj(&wpm_peripheral_status_widget),
                 LV_ALIGN_TOP_LEFT, CONFIG_NICE_OLED_WIDGET_WPM_LABEL_CUSTOM_X,
                 CONFIG_NICE_OLED_WIDGET_WPM_LABEL_CUSTOM_Y);
#endif

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_PERIPHERAL) &&                                          \
    IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_PERIPHERAL_BONGO_CAT)
    zmk_widget_wpm_bongo_cat_init(&wpm_bongo_cat_widget, canvas);
    lv_obj_align(zmk_widget_wpm_bongo_cat_obj(&wpm_bongo_cat_widget), LV_ALIGN_TOP_LEFT,
                 CONFIG_NICE_OLED_WIDGET_BONGO_CAT_CUSTOM_X,
                 CONFIG_NICE_OLED_WIDGET_BONGO_CAT_CUSTOM_Y);
#endif

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_PERIPHERAL_INDICATORS_LUNA) ||                    \
    IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_PERIPHERAL_INDICATORS_BONGO_CAT)
    zmk_widget_modifiers_init(&modifiers_widget, canvas);
    lv_obj_align(zmk_widget_modifiers_obj(&modifiers_widget), LV_ALIGN_TOP_LEFT,
                 CONFIG_NICE_OLED_WIDGET_MODIFIERS_CUSTOM_X,
                 CONFIG_NICE_OLED_WIDGET_MODIFIERS_CUSTOM_Y);
#endif

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RESPONSIVE_BONGO_CAT) ||                                   \
    IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RESPONSIVE_BONGO_CAT_PERIPHERAL)
    zmk_widget_responsive_bongo_cat_init(&responsive_bongo_cat_widget, canvas);
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RESPONSIVE_BONGO_CAT_VERTICAL)
    lv_obj_align(zmk_widget_responsive_bongo_cat_obj(&responsive_bongo_cat_widget),
                 LV_ALIGN_TOP_LEFT, CONFIG_NICE_OLED_WIDGET_RESPONSIVE_BONGO_CAT_CUSTOM_X,
                 CONFIG_NICE_OLED_WIDGET_RESPONSIVE_BONGO_CAT_CUSTOM_Y);
#else
    lv_obj_align(zmk_widget_responsive_bongo_cat_obj(&responsive_bongo_cat_widget),
                 LV_ALIGN_TOP_LEFT, CONFIG_NICE_OLED_WIDGET_HID_INDICATORS_CUSTOM_X,
                 CONFIG_NICE_OLED_WIDGET_HID_INDICATORS_CUSTOM_Y);
#endif
#endif

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_HID_INDICATORS)
    zmk_widget_hid_indicators_init(&hid_indicators_widget, canvas);
    lv_obj_align(zmk_widget_hid_indicators_obj(&hid_indicators_widget), LV_ALIGN_TOP_LEFT,
                 CONFIG_NICE_OLED_WIDGET_HID_INDICATORS_CUSTOM_X,
                 CONFIG_NICE_OLED_WIDGET_HID_INDICATORS_CUSTOM_Y);
#endif

#if IS_ENABLED(CONFIG_NICE_OLED_SHOW_SLEEP_ART_ON_IDLE) ||                                         \
    IS_ENABLED(CONFIG_NICE_OLED_SHOW_SLEEP_ART_ON_SLEEP)
    zmk_widget_sleep_status_init(&sleep_status_widget, canvas);
    lv_obj_align(zmk_widget_sleep_status_obj(&sleep_status_widget), LV_ALIGN_TOP_LEFT, CONFIG_NICE_OLED_WIDGET_SLEEP_STATUS_CUSTOM_X, CONFIG_NICE_OLED_WIDGET_SLEEP_STATUS_CUSTOM_Y);
#endif

    return 0;
}

lv_obj_t *zmk_widget_screen_obj(struct zmk_widget_screen *widget) { return widget->obj; }
