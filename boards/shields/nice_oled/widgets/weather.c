#include <zephyr/kernel.h>
#include <zmk/display/status_screen.h>
#include <zmk/event_manager.h>
#include <zmk/events/raw_hid.h>
#include <lvgl.h>

#include "weather.h"

#define ZMK_RAW_HID_DATATYPE_WEATHER 0xAF

static lv_obj_t *weather_label;

void update_weather_widget_label(int8_t temperature) {
    if (weather_label != NULL) {
        if (temperature == 127) { // Valor especial para N/A
            lv_label_set_text(weather_label, "N/A");
        } else {
            lv_label_set_text_fmt(weather_label, "%d C", temperature);
        }
    }
}

void zmk_raw_hid_receive_cb_weather(const uint8_t *data, uint8_t length) {
    if (length < 2 || data[0] != ZMK_RAW_HID_DATATYPE_WEATHER) {
        return;
    }
    int8_t temperature = (int8_t)data[1];
    update_weather_widget_label(temperature);
}

int raw_hid_weather_listener(const zmk_event_t *eh) {
    const struct zmk_raw_hid_event *ev = as_zmk_raw_hid_event(eh);
    if (ev) {
        zmk_raw_hid_receive_cb_weather(ev->data, ev->len);
    }
    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(raw_hid_weather_listener, raw_hid_weather_listener);
ZMK_SUBSCRIPTION(raw_hid_weather_listener, zmk_raw_hid_event);

lv_obj_t *zmk_widget_weather_init(lv_obj_t *parent) {
    weather_label = lv_label_create(parent);
    update_weather_widget_label(127); // Iniciar con N/A
    return weather_label;
}
