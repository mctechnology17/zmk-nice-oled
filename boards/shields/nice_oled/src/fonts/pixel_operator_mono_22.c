/*******************************************************************************
 * Size: 22 px
 * Bpp: 1
 * Opts: --font PixelOperatorMono.ttf --size 22 --format lvgl --bpp 1 --range 0x20-0x7F
 *--no-compress -o pixel_operator_mono_22.c
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include <lvgl.h>
#endif

#ifndef PIXEL_OPERATOR_MONO_22
#define PIXEL_OPERATOR_MONO_22 1
#endif

#if PIXEL_OPERATOR_MONO_22

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xff, 0xc8,

    /* U+0022 "\"" */
    0x99, 0x99,

    /* U+0023 "#" */
    0x22, 0x22, 0x22, 0xff, 0x22, 0x22, 0x22, 0x22, 0x22, 0xff, 0x22, 0x22, 0x22,

    /* U+0024 "$" */
    0x10, 0x21, 0xe0, 0x89, 0x32, 0x24, 0x48, 0x78, 0x24, 0x48, 0x99, 0x22, 0x1e, 0x8, 0x10, 0x20,

    /* U+0025 "%" */
    0x20, 0x48, 0x24, 0x12, 0x20, 0x1, 0x10, 0x20, 0x22, 0x0, 0x11, 0x40, 0xa0, 0x50, 0x10,

    /* U+0026 "&" */
    0x78, 0x2, 0xc, 0x8, 0x10, 0x1e, 0x41, 0x83, 0x6, 0x8, 0x17, 0xe0,

    /* U+0027 "'" */
    0xf0,

    /* U+0028 "(" */
    0x8, 0x9, 0x8, 0x42, 0x10, 0x84, 0x8, 0x0, 0x80,

    /* U+0029 ")" */
    0x80, 0x8, 0x10, 0x84, 0x21, 0x8, 0x48, 0x8, 0x0,

    /* U+002A "*" */
    0x10, 0x22, 0x4b, 0xc9, 0x22, 0x4, 0x0,

    /* U+002B "+" */
    0x10, 0x20, 0x47, 0xf1, 0x2, 0x4, 0x0,

    /* U+002C "," */
    0x24, 0xc0,

    /* U+002D "-" */
    0xfc,

    /* U+002E "." */
    0x80,

    /* U+002F "/" */
    0x8, 0x42, 0x10, 0x10, 0x84, 0x24, 0x21, 0x8, 0x0,

    /* U+0030 "0" */
    0x78, 0x2, 0xc, 0x18, 0x31, 0xe4, 0xf1, 0x83, 0x6, 0xc, 0x17, 0x80,

    /* U+0031 "1" */
    0x11, 0x79, 0x11, 0x11, 0x11, 0x11, 0x10,

    /* U+0032 "2" */
    0x38, 0x2, 0x8, 0x10, 0x21, 0x4, 0x10, 0x81, 0x2, 0x4, 0xf, 0xe0,

    /* U+0033 "3" */
    0x78, 0x2, 0x8, 0x10, 0x20, 0x4e, 0x1, 0x2, 0x6, 0x8, 0x7, 0x80,

    /* U+0034 "4" */
    0x2, 0x4, 0x38, 0x90, 0x24, 0x60, 0xff, 0x2, 0x4, 0x8, 0x10, 0x20,

    /* U+0035 "5" */
    0xff, 0x2, 0x4, 0x8, 0x1f, 0x0, 0x81, 0x2, 0x6, 0x8, 0x7, 0x80,

    /* U+0036 "6" */
    0x78, 0x2, 0xc, 0x8, 0x10, 0x3e, 0x41, 0x83, 0x6, 0x8, 0x7, 0x80,

    /* U+0037 "7" */
    0xfe, 0x4, 0x8, 0x10, 0x21, 0x4, 0x10, 0x1, 0x2, 0x4, 0x8, 0x0,

    /* U+0038 "8" */
    0x78, 0x2, 0xc, 0x18, 0x30, 0x5e, 0x41, 0x83, 0x6, 0x8, 0x7, 0x80,

    /* U+0039 "9" */
    0x78, 0x2, 0xc, 0x18, 0x30, 0x5f, 0x81, 0x2, 0x6, 0x8, 0x7, 0x80,

    /* U+003A ":" */
    0x80, 0x40,

    /* U+003B ";" */
    0x20, 0x0, 0x0, 0x4, 0x98,

    /* U+003C "<" */
    0x9, 0x1, 0x2, 0x0, 0x20,

    /* U+003D "=" */
    0xfc, 0x0, 0x3f,

    /* U+003E ">" */
    0x81, 0x0, 0x12, 0x2, 0x0,

    /* U+003F "?" */
    0x78, 0x2, 0x8, 0x10, 0x21, 0x4, 0x8, 0x10, 0x20, 0x0, 0x1, 0x0,

    /* U+0040 "@" */
    0x7f, 0x0, 0x20, 0x31, 0xd8, 0x2c, 0x96, 0x4b, 0x25, 0x92, 0xc7, 0xa0, 0x10, 0x7, 0xf0,

    /* U+0041 "A" */
    0x78, 0x2, 0xc, 0x18, 0x30, 0x60, 0xff, 0x83, 0x6, 0xc, 0x18, 0x20,

    /* U+0042 "B" */
    0xfd, 0x2, 0xc, 0x18, 0x30, 0x7f, 0x41, 0x83, 0x6, 0xc, 0xf, 0xc0,

    /* U+0043 "C" */
    0x78, 0x2, 0xc, 0x8, 0x10, 0x20, 0x40, 0x81, 0x2, 0x8, 0x7, 0x80,

    /* U+0044 "D" */
    0xfd, 0x2, 0xc, 0x18, 0x30, 0x60, 0xc1, 0x83, 0x6, 0xc, 0x1f, 0xc0,

    /* U+0045 "E" */
    0xff, 0x2, 0x4, 0x8, 0x10, 0x3c, 0x40, 0x81, 0x2, 0x4, 0xf, 0xe0,

    /* U+0046 "F" */
    0xff, 0x2, 0x4, 0x8, 0x10, 0x3c, 0x40, 0x81, 0x2, 0x4, 0x8, 0x0,

    /* U+0047 "G" */
    0x78, 0x2, 0xc, 0x8, 0x10, 0x23, 0xc1, 0x83, 0x6, 0xc, 0x17, 0xe0,

    /* U+0048 "H" */
    0x83, 0x6, 0xc, 0x18, 0x30, 0x7f, 0xc1, 0x83, 0x6, 0xc, 0x18, 0x20,

    /* U+0049 "I" */
    0xff, 0xf8,

    /* U+004A "J" */
    0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x81, 0x2, 0x6, 0x8, 0x7, 0x80,

    /* U+004B "K" */
    0x83, 0x6, 0xc, 0x48, 0x12, 0x38, 0x48, 0x81, 0x12, 0xc, 0x18, 0x20,

    /* U+004C "L" */
    0x81, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x81, 0x2, 0x4, 0xf, 0xe0,

    /* U+004D "M" */
    0x80, 0xc0, 0x60, 0x3c, 0x38, 0xc, 0x96, 0x23, 0x1, 0x80, 0xc0, 0x60, 0x30, 0x18, 0x8,

    /* U+004E "N" */
    0x83, 0x6, 0xc, 0x18, 0x3c, 0x64, 0xc7, 0x83, 0x6, 0xc, 0x18, 0x20,

    /* U+004F "O" */
    0x78, 0x2, 0xc, 0x18, 0x30, 0x60, 0xc1, 0x83, 0x6, 0xc, 0x17, 0x80,

    /* U+0050 "P" */
    0xfd, 0x2, 0xc, 0x18, 0x30, 0x7f, 0x40, 0x81, 0x2, 0x4, 0x8, 0x0,

    /* U+0051 "Q" */
    0x78, 0x2, 0xc, 0x18, 0x30, 0x60, 0xc1, 0x83, 0x26, 0x20, 0x7, 0x20,

    /* U+0052 "R" */
    0xf9, 0x2, 0xc, 0x18, 0x30, 0x7e, 0x48, 0x91, 0x12, 0xc, 0x18, 0x20,

    /* U+0053 "S" */
    0x78, 0x2, 0xc, 0x8, 0x10, 0x1e, 0x1, 0x2, 0x6, 0x8, 0x7, 0x80,

    /* U+0054 "T" */
    0xfe, 0x20, 0x40, 0x81, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x81, 0x0,

    /* U+0055 "U" */
    0x83, 0x6, 0xc, 0x18, 0x30, 0x60, 0xc1, 0x83, 0x6, 0x8, 0x7, 0x80,

    /* U+0056 "V" */
    0x83, 0x6, 0xc, 0x18, 0x30, 0x60, 0xc1, 0x83, 0x4, 0xa0, 0x1, 0x0,

    /* U+0057 "W" */
    0x80, 0xc0, 0x60, 0x31, 0x18, 0x8c, 0x46, 0x23, 0x11, 0x88, 0xc4, 0x62, 0x20, 0x7, 0x70,

    /* U+0058 "X" */
    0x83, 0x6, 0xc, 0x10, 0x5, 0x4, 0x14, 0x1, 0x6, 0xc, 0x18, 0x20,

    /* U+0059 "Y" */
    0x83, 0x6, 0xc, 0x10, 0x5, 0x4, 0x8, 0x10, 0x20, 0x40, 0x81, 0x0,

    /* U+005A "Z" */
    0xfe, 0x4, 0x8, 0x10, 0x21, 0x4, 0x10, 0x81, 0x2, 0x4, 0xf, 0xe0,

    /* U+005B "[" */
    0xf8, 0x88, 0x88, 0x88, 0x88, 0x88, 0xf0,

    /* U+005C "\\" */
    0x84, 0x21, 0x0, 0x10, 0x84, 0x20, 0x42, 0x10, 0x80,

    /* U+005D "]" */
    0xf1, 0x11, 0x11, 0x11, 0x11, 0x11, 0xf0,

    /* U+005E "^" */
    0x10, 0x50, 0x4, 0x10,

    /* U+005F "_" */
    0xfe,

    /* U+0060 "`" */
    0x80, 0x80,

    /* U+0061 "a" */
    0x78, 0x2, 0x8, 0x17, 0xe0, 0x60, 0xc1, 0x82, 0xfc,

    /* U+0062 "b" */
    0x81, 0x2, 0x7, 0xe8, 0x30, 0x60, 0xc1, 0x83, 0x6, 0xc, 0xf, 0xc0,

    /* U+0063 "c" */
    0x78, 0x2, 0xc, 0x8, 0x10, 0x20, 0x41, 0x0, 0xf0,

    /* U+0064 "d" */
    0x2, 0x4, 0xb, 0xf8, 0x30, 0x60, 0xc1, 0x83, 0x6, 0x8, 0x17, 0xe0,

    /* U+0065 "e" */
    0x79, 0x6, 0xc, 0x1f, 0xf0, 0x20, 0x41, 0x0, 0xf0,

    /* U+0066 "f" */
    0x1e, 0x0, 0x81, 0x2, 0x1f, 0x88, 0x10, 0x20, 0x40, 0x81, 0x2, 0x0,

    /* U+0067 "g" */
    0x7e, 0x6, 0xc, 0x18, 0x30, 0x60, 0xbf, 0x2, 0x6, 0x8, 0x7, 0x80,

    /* U+0068 "h" */
    0x81, 0x2, 0x7, 0xe8, 0x10, 0x60, 0xc1, 0x83, 0x6, 0xc, 0x18, 0x20,

    /* U+0069 "i" */
    0x9f, 0xf8,

    /* U+006A "j" */
    0x2, 0x0, 0x0, 0x10, 0x20, 0x40, 0x81, 0x2, 0x4, 0x8, 0x10, 0x30, 0x40, 0x3c,

    /* U+006B "k" */
    0x81, 0x2, 0x4, 0x18, 0x11, 0x24, 0x70, 0x81, 0x22, 0x24, 0x8, 0x20,

    /* U+006C "l" */
    0xff, 0xf8,

    /* U+006D "m" */
    0xf7, 0x40, 0x22, 0x31, 0x18, 0x8c, 0x46, 0x23, 0x1, 0x80, 0xc0, 0x40,

    /* U+006E "n" */
    0xfd, 0x2, 0xc, 0x18, 0x30, 0x60, 0xc1, 0x83, 0x4,

    /* U+006F "o" */
    0x79, 0x6, 0xc, 0x18, 0x30, 0x60, 0xc1, 0x0, 0xf0,

    /* U+0070 "p" */
    0xfd, 0x2, 0xc, 0x18, 0x30, 0x60, 0xc1, 0x83, 0xfa, 0x4, 0x8, 0x0,

    /* U+0071 "q" */
    0x7e, 0x6, 0xc, 0x18, 0x30, 0x60, 0xc1, 0x82, 0xfc, 0x8, 0x10, 0x20,

    /* U+0072 "r" */
    0x8f, 0x2, 0x47, 0x8, 0x10, 0x20, 0x40, 0x81, 0x0,

    /* U+0073 "s" */
    0x79, 0x6, 0x4, 0x7, 0x80, 0x40, 0xc1, 0x0, 0xf0,

    /* U+0074 "t" */
    0x21, 0xf8, 0x81, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x78,

    /* U+0075 "u" */
    0x83, 0x6, 0xc, 0x18, 0x30, 0x60, 0xc1, 0x0, 0xf0,

    /* U+0076 "v" */
    0x83, 0x6, 0xc, 0x18, 0x30, 0x60, 0x94, 0x0, 0x20,

    /* U+0077 "w" */
    0x80, 0xc0, 0x60, 0x31, 0x18, 0x8c, 0x46, 0x23, 0x11, 0x0, 0x3b, 0x80,

    /* U+0078 "x" */
    0x83, 0x6, 0x9, 0x41, 0x0, 0xa, 0x41, 0x83, 0x4,

    /* U+0079 "y" */
    0x83, 0x6, 0xc, 0x18, 0x30, 0x60, 0xbf, 0x2, 0x6, 0x8, 0x7, 0x80,

    /* U+007A "z" */
    0xfe, 0x4, 0x8, 0x41, 0x0, 0x8, 0x40, 0x81, 0xfc,

    /* U+007B "{" */
    0xe, 0x0, 0x81, 0x2, 0x4, 0x20, 0x10, 0x20, 0x40, 0x80, 0x0, 0xe0,

    /* U+007C "|" */
    0xff, 0xf8,

    /* U+007D "}" */
    0xe0, 0x4, 0x21, 0x8, 0x22, 0x10, 0x84, 0xe, 0x0,

    /* U+007E "~" */
    0x71, 0x0, 0x8e};

/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0,
     .adv_w = 0,
     .box_w = 0,
     .box_h = 0,
     .ofs_x = 0,
     .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 176, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 176, .box_w = 1, .box_h = 13, .ofs_x = 4, .ofs_y = 0},
    {.bitmap_index = 3, .adv_w = 176, .box_w = 4, .box_h = 4, .ofs_x = 3, .ofs_y = 9},
    {.bitmap_index = 5, .adv_w = 176, .box_w = 8, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 18, .adv_w = 176, .box_w = 7, .box_h = 18, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 34, .adv_w = 176, .box_w = 9, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 49, .adv_w = 176, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 61, .adv_w = 176, .box_w = 1, .box_h = 4, .ofs_x = 4, .ofs_y = 9},
    {.bitmap_index = 62, .adv_w = 176, .box_w = 5, .box_h = 13, .ofs_x = 4, .ofs_y = 0},
    {.bitmap_index = 71, .adv_w = 176, .box_w = 5, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 80, .adv_w = 176, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 6},
    {.bitmap_index = 87, .adv_w = 176, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 94, .adv_w = 176, .box_w = 3, .box_h = 4, .ofs_x = 3, .ofs_y = -3},
    {.bitmap_index = 96, .adv_w = 176, .box_w = 6, .box_h = 1, .ofs_x = 3, .ofs_y = 6},
    {.bitmap_index = 97, .adv_w = 176, .box_w = 1, .box_h = 1, .ofs_x = 4, .ofs_y = 0},
    {.bitmap_index = 98, .adv_w = 176, .box_w = 5, .box_h = 13, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 107, .adv_w = 176, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 119, .adv_w = 176, .box_w = 4, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 126, .adv_w = 176, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 138, .adv_w = 176, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 150, .adv_w = 176, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 162, .adv_w = 176, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 174, .adv_w = 176, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 186, .adv_w = 176, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 198, .adv_w = 176, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 210, .adv_w = 176, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 222, .adv_w = 176, .box_w = 1, .box_h = 10, .ofs_x = 4, .ofs_y = 0},
    {.bitmap_index = 224, .adv_w = 176, .box_w = 3, .box_h = 13, .ofs_x = 3, .ofs_y = -3},
    {.bitmap_index = 229, .adv_w = 176, .box_w = 5, .box_h = 7, .ofs_x = 4, .ofs_y = 3},
    {.bitmap_index = 234, .adv_w = 176, .box_w = 6, .box_h = 4, .ofs_x = 3, .ofs_y = 5},
    {.bitmap_index = 237, .adv_w = 176, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 242, .adv_w = 176, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 254, .adv_w = 176, .box_w = 9, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 269, .adv_w = 176, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 281, .adv_w = 176, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 293, .adv_w = 176, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 305, .adv_w = 176, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 317, .adv_w = 176, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 329, .adv_w = 176, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 341, .adv_w = 176, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 353, .adv_w = 176, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 365, .adv_w = 176, .box_w = 1, .box_h = 13, .ofs_x = 4, .ofs_y = 0},
    {.bitmap_index = 367, .adv_w = 176, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 379, .adv_w = 176, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 391, .adv_w = 176, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 403, .adv_w = 176, .box_w = 9, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 418, .adv_w = 176, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 430, .adv_w = 176, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 442, .adv_w = 176, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 454, .adv_w = 176, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 466, .adv_w = 176, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 478, .adv_w = 176, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 490, .adv_w = 176, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 502, .adv_w = 176, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 514, .adv_w = 176, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 526, .adv_w = 176, .box_w = 9, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 541, .adv_w = 176, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 553, .adv_w = 176, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 565, .adv_w = 176, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 577, .adv_w = 176, .box_w = 4, .box_h = 13, .ofs_x = 4, .ofs_y = 0},
    {.bitmap_index = 584, .adv_w = 176, .box_w = 5, .box_h = 13, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 593, .adv_w = 176, .box_w = 4, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 600, .adv_w = 176, .box_w = 7, .box_h = 4, .ofs_x = 1, .ofs_y = 9},
    {.bitmap_index = 604, .adv_w = 176, .box_w = 7, .box_h = 1, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 605, .adv_w = 176, .box_w = 3, .box_h = 3, .ofs_x = 3, .ofs_y = 10},
    {.bitmap_index = 607, .adv_w = 176, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 616, .adv_w = 176, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 628, .adv_w = 176, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 637, .adv_w = 176, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 649, .adv_w = 176, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 658, .adv_w = 176, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 670, .adv_w = 176, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 682, .adv_w = 176, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 694, .adv_w = 176, .box_w = 1, .box_h = 13, .ofs_x = 4, .ofs_y = 0},
    {.bitmap_index = 696, .adv_w = 176, .box_w = 7, .box_h = 16, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 710, .adv_w = 176, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 722, .adv_w = 176, .box_w = 1, .box_h = 13, .ofs_x = 4, .ofs_y = 0},
    {.bitmap_index = 724, .adv_w = 176, .box_w = 9, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 736, .adv_w = 176, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 745, .adv_w = 176, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 754, .adv_w = 176, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 766, .adv_w = 176, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 778, .adv_w = 176, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 787, .adv_w = 176, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 796, .adv_w = 176, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 806, .adv_w = 176, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 815, .adv_w = 176, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 824, .adv_w = 176, .box_w = 9, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 836, .adv_w = 176, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 845, .adv_w = 176, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 857, .adv_w = 176, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 866, .adv_w = 176, .box_w = 7, .box_h = 13, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 878, .adv_w = 176, .box_w = 1, .box_h = 13, .ofs_x = 4, .ofs_y = 0},
    {.bitmap_index = 880, .adv_w = 176, .box_w = 5, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 889, .adv_w = 176, .box_w = 8, .box_h = 3, .ofs_x = 1, .ofs_y = 10}};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] = {{.range_start = 32,
                                                .range_length = 95,
                                                .glyph_id_start = 1,
                                                .unicode_list = NULL,
                                                .glyph_id_ofs_list = NULL,
                                                .list_length = 0,
                                                .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY}};

/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 1,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};

/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t pixel_operator_mono_22 = {
#else
lv_font_t pixel_operator_mono_22 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt, /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt, /*Function pointer to get glyph's bitmap*/
    .line_height = 18,                              /*The maximum line height required by the font*/
    .base_line = 3, /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -4,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc, /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};

#endif /*#if PIXEL_OPERATOR_MONO_22*/
