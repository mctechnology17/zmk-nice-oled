#include <lvgl.h>

#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif

#ifndef LV_ATTRIBUTE_IMG_BOLT
#define LV_ATTRIBUTE_IMG_BOLT
#endif

const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_BOLT uint8_t bolt_map[] = {
#if CONFIG_NICE_OLED_WIDGET_INVERTED
    0x00, 0x00, 0x00, 0xff, /*Color of index 0*/
    0xff, 0xff, 0xff, 0xff, /*Color of index 1*/
#else
    0xff, 0xff, 0xff, 0xff, /*Color of index 0*/
    0x00, 0x00, 0x00, 0xff, /*Color of index 1*/
#endif

    0x04, 0x08, 0x18, 0x30, 0x7f, 0xfe, 0x1c, 0x18, 0x30, 0x20, 0x40,
};

const lv_img_dsc_t bolt = {
    .header.cf = LV_IMG_CF_INDEXED_1BIT,
    .header.always_zero = 0,
    .header.reserved = 0,
    .header.w = 8,
    .header.h = 11,
    .data_size = 19,
    .data = bolt_map,
};
