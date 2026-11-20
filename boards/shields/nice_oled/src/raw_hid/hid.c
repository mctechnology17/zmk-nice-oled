#include <raw_hid/hid.h>
#include <raw_hid/events.h>

#include <zephyr/kernel.h>

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

ZMK_EVENT_IMPL(is_connected_notification);
ZMK_EVENT_IMPL(time_notification);
ZMK_EVENT_IMPL(volume_notification);
ZMK_EVENT_IMPL(weather_notification); // NUEVO
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID_WORLD_CUP)
ZMK_EVENT_IMPL(world_cup_notification);
#endif
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_SPOTIFY_MACOS) ||                     \
    IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_LINUX)
ZMK_EVENT_IMPL(spotify_notification);
#endif
#ifdef CONFIG_NICE_OLED_WIDGET_RAW_HID_LAYOUT
ZMK_EVENT_IMPL(layout_notification);
#endif

typedef enum {
    _TIME = 0xAA, // random value, must match companion app
    _VOLUME,
    _LAYOUT,
    _WEATHER = 0xAF, // NUEVO
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_SPOTIFY_MACOS)
    _SPOTIFY = 0xAE,
#endif
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_LINUX)
    _MEDIA_PLAYER_LINUX = 0xB0,
#endif
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID_WORLD_CUP)
    _WORLD_CUP = 0xB1,
#endif
} hid_data_type;

static bool is_connected = false;

// Both run on the system work queue, not in a kernel timer: a timer callback is interrupt
// context, and raising an event there runs every listener (the display ones take a mutex) in it.
static void disconnect_work_handler(struct k_work *work) {
    LOG_INF("raise_connection_notification: false");
    is_connected = false;
    raise_is_connected_notification((struct is_connected_notification){.value = false});
}

static K_WORK_DELAYABLE_DEFINE(disconnect_work, disconnect_work_handler);

static uint8_t last_hid_volume = 0;
static uint8_t last_raised_volume = 0;

static void raise_volume_if_changed(void) {
    // prevent raising event with the same value multiple times
    if (last_raised_volume != last_hid_volume) {
        last_raised_volume = last_hid_volume;
        LOG_INF("raise_volume_notification %i", last_hid_volume);
        raise_volume_notification((struct volume_notification){.value = last_hid_volume});
    }
}

static void volume_work_handler(struct k_work *work) { raise_volume_if_changed(); }

static K_WORK_DELAYABLE_DEFINE(volume_work, volume_work_handler);

static void process_raw_hid_data(uint8_t *data) {
    LOG_INF("display_process_raw_hid_data - received data_type %u", data[0]);

    // raise disconnect notification after 65 seconds of inactivity
    k_work_reschedule(&disconnect_work, K_SECONDS(65));
    if (!is_connected) {
        LOG_INF("raise_connection_notification: true");
        is_connected = true;
        raise_is_connected_notification((struct is_connected_notification){.value = true});
    }

    uint8_t data_type = data[0];
    switch (data_type) {
    case _TIME:
        raise_time_notification((struct time_notification){.hour = data[1], .minute = data[2]});
        break;

    case _VOLUME:
        last_hid_volume = data[1];

        // debounce volume change events: the first one is shown at once, the last one of a
        // burst 150 ms later
        if (!k_work_delayable_is_pending(&volume_work)) {
            k_work_schedule(&volume_work, K_MSEC(150));
            raise_volume_if_changed();
        }

        break;

#ifdef CONFIG_NICE_OLED_WIDGET_RAW_HID_LAYOUT
    case _LAYOUT:
        raise_layout_notification((struct layout_notification){.value = data[1]});
        break;
#endif

    // --- NUEVO: Caso para Clima ---
    case _WEATHER:
        raise_weather_notification((struct weather_notification){.temperature = (int8_t)data[1]});
        break;

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID_WORLD_CUP)
    case _WORLD_CUP: {
        struct world_cup_notification notification;
        memcpy(notification.score, &data[1], sizeof(notification.score));
        notification.score[sizeof(notification.score) - 1] = '\0';
        raise_world_cup_notification(notification);
        break;
    }
#endif

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_SPOTIFY_MACOS)
    case _SPOTIFY: {
        struct spotify_notification notification;
        memcpy(notification.media_player, &data[1], sizeof(notification.media_player));
        notification.media_player[sizeof(notification.media_player) - 1] = '\0';
        raise_spotify_notification(notification);
        break;
    }
#endif
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_LINUX)
    case _MEDIA_PLAYER_LINUX: {
        struct spotify_notification notification;
        memcpy(notification.media_player, &data[1], sizeof(notification.media_player));
        notification.media_player[sizeof(notification.media_player) - 1] = '\0';
        raise_spotify_notification(notification);
        break;
    }
#endif
    }
}

static int raw_hid_received_event_listener(const zmk_event_t *eh) {
    struct raw_hid_received_event *event = as_raw_hid_received_event(eh);
    if (event && event->length > 0) {
        // A host can write fewer bytes than a full report (over Bluetooth any host that finds the
        // characteristic can write to it): work on a zero-padded copy, never past what arrived.
        uint8_t report[CONFIG_NICE_OLED_WIDGET_RAW_HID_REPORT_SIZE] = {0};
        memcpy(report, event->data, MIN(event->length, sizeof(report)));
        process_raw_hid_data(report);
    }

    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(process_raw_hid_event, raw_hid_received_event_listener);
ZMK_SUBSCRIPTION(process_raw_hid_event, raw_hid_received_event);
