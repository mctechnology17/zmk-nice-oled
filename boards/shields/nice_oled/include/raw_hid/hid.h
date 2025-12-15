#pragma once

#include <zmk/event_manager.h>

#ifdef CONFIG_NICE_OLED_WIDGET_RAW_HID

struct is_connected_notification {
    bool value;
};

ZMK_EVENT_DECLARE(is_connected_notification);

struct time_notification {
    uint8_t hour;
    uint8_t minute;
};

ZMK_EVENT_DECLARE(time_notification);

struct volume_notification {
    uint8_t value;
};

ZMK_EVENT_DECLARE(volume_notification);

struct weather_notification {
    int8_t temperature;
};
ZMK_EVENT_DECLARE(weather_notification);

#ifdef CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_SPOTIFY_MACOS
struct spotify_notification {
    char media_player[11];
};
ZMK_EVENT_DECLARE(spotify_notification);
#endif

#ifdef CONFIG_NICE_OLED_WIDGET_RAW_HID_LAYOUT
struct layout_notification {
    uint8_t value;
};

ZMK_EVENT_DECLARE(layout_notification);
#endif
#endif
