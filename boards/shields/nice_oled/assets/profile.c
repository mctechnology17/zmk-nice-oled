#include <lvgl.h>

#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif

#ifndef LV_ATTRIBUTE_IMG_PROFILE
#define LV_ATTRIBUTE_IMG_PROFILE
#endif

const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_PROFILE uint8_t
    profile_map[] = {
#if CONFIG_NICE_OLED_WIDGET_INVERTED
        0x00, 0x00, 0x00, 0xff, /*Color of index 0*/
        0xff, 0xff, 0xff, 0xff, /*Color of index 1*/
#else
        0xff, 0xff, 0xff, 0xff, /*Color of index 0*/
        0x00, 0x00, 0x00, 0xff, /*Color of index 1*/
#endif

        0x15, 0x40, 0x40, 0x00, 0x00, 0x10, 0x80, 0x00, 0x00, 0x10, 0x80, 0x00,
        0x00, 0x10, 0x80, 0x00, 0x00, 0x10, 0x80, 0x00, 0x00, 0x20, 0x2a, 0x80,
};

const lv_img_dsc_t profile = {
    .header.cf = LV_IMG_CF_INDEXED_1BIT,
    .header.always_zero = 0,
    .header.reserved = 0,
    .header.w = 12,
    .header.h = 12,
    .data_size = 32,
    .data = profile_map,
};
