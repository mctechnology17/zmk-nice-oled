#include <raw_hid/events.h>

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

ZMK_EVENT_IMPL(raw_hid_received_event);
ZMK_EVENT_IMPL(raw_hid_sent_event);
