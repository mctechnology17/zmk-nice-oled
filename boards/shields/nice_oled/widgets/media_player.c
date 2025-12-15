#include <zephyr/kernel.h>
#include <zmk/display/status_screen.h>
#include <zmk/event_manager.h>
#include <zmk/events/raw_hid.h>
#include <lvgl.h>

#include "media_player.h"

#define ZMK_RAW_HID_DATATYPE_MEDIA 0xB0
#define ZMK_RAW_HID_MEDIA_SUB_STATUS 0x01
#define ZMK_RAW_HID_MEDIA_SUB_ARTIST 0x02
#define ZMK_RAW_HID_MEDIA_SUB_TITLE  0x03

static lv_obj_t *media_icon;
static lv_obj_t *artist_label;
static lv_obj_t *title_label;

void update_media_status_icon(bool is_playing) {
    if (media_icon != NULL) {
        lv_label_set_text(media_icon, is_playing ? LV_SYMBOL_PAUSE : LV_SYMBOL_PLAY);
    }
}

void update_media_artist_label(const char *text) {
    if (artist_label != NULL) {
        lv_label_set_text(artist_label, text);
    }
}

void update_media_title_label(const char *text) {
    if (title_label != NULL) {
        lv_label_set_text(title_label, text);
    }
}

void zmk_raw_hid_receive_cb_media(const uint8_t *data, uint8_t length) {
    if (length < 3 || data[0] != ZMK_RAW_HID_DATATYPE_MEDIA) {
        return;
    }

    uint8_t sub_type = data[1];
    const char *text_data = (const char *)&data[2];

    switch (sub_type) {
        case ZMK_RAW_HID_MEDIA_SUB_STATUS:
            update_media_status_icon(data[2] == 1);
            break;
        case ZMK_RAW_HID_MEDIA_SUB_ARTIST:
            update_media_artist_label(text_data);
            break;
        case ZMK_RAW_HID_MEDIA_SUB_TITLE:
            update_media_title_label(text_data);
            break;
    }
}

int raw_hid_media_listener(const zmk_event_t *eh) {
    const struct zmk_raw_hid_event *ev = as_zmk_raw_hid_event(eh);
    if (ev) {
        zmk_raw_hid_receive_cb_media(ev->data, ev->len);
    }
    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(raw_hid_media_listener, raw_hid_media_listener);
ZMK_SUBSCRIPTION(raw_hid_media_listener, zmk_raw_hid_event);

lv_obj_t *zmk_widget_media_player_init(lv_obj_t *parent) {
    lv_obj_t *container = lv_obj_create(parent);
    lv_obj_remove_style_all(container);
    lv_obj_set_size(container, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(container, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);

    media_icon = lv_label_create(container);
    artist_label = lv_label_create(container);
    title_label = lv_label_create(container);

    lv_obj_set_width(artist_label, 64);
    lv_label_set_long_mode(artist_label, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_width(title_label, 64);
    lv_label_set_long_mode(title_label, LV_LABEL_LONG_SCROLL_CIRCULAR);

    update_media_status_icon(false);
    update_media_artist_label("---");
    update_media_title_label("---");

    return container;
}
