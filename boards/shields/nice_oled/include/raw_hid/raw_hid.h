#pragma once

#include <zmk/hid.h>
#include <zephyr/usb/class/hid.h>
#include <zephyr/usb/class/usb_hid.h>

#define HID_USAGE_PAGE16(page, page2)                                                              \
    HID_ITEM(HID_ITEM_TAG_USAGE_PAGE, HID_ITEM_TYPE_GLOBAL, 2), page, page2

#define HID_USAGE_PAGE16_SINGLE(a) HID_USAGE_PAGE16((a & 0xFF), ((a >> 8) & 0xFF))

static const uint8_t raw_hid_report_desc[] = {
    HID_USAGE_PAGE16_SINGLE(CONFIG_NICE_OLED_WIDGET_RAW_HID_USAGE_PAGE),
    HID_USAGE(CONFIG_NICE_OLED_WIDGET_RAW_HID_USAGE),

    HID_COLLECTION(0x01),

    HID_LOGICAL_MIN8(0x00),
    HID_LOGICAL_MAX16(0xFF, 0x00),
    HID_REPORT_SIZE(0x08),
    HID_REPORT_COUNT(CONFIG_NICE_OLED_WIDGET_RAW_HID_REPORT_SIZE),

    HID_USAGE(0x01),
    HID_INPUT(ZMK_HID_MAIN_VAL_DATA | ZMK_HID_MAIN_VAL_VAR | ZMK_HID_MAIN_VAL_ABS),

    HID_USAGE(0x02),
    HID_OUTPUT(ZMK_HID_MAIN_VAL_DATA | ZMK_HID_MAIN_VAL_VAR | ZMK_HID_MAIN_VAL_ABS |
               ZMK_HID_MAIN_VAL_NON_VOL),

    HID_END_COLLECTION,
};
