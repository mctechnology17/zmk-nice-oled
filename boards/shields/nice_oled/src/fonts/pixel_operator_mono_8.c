/*******************************************************************************
 * Size: 8 px
 * Bpp: 1
 * Opts: --font PixelOperatorMono.ttf --size 8 --format lvgl --bpp 1 --range
 *0x20-0x7F --no-compress -o pixel_operator_mono_8.c
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl.h"
#endif

#ifndef PIXEL_OPERATOR_MONO_8
#define PIXEL_OPERATOR_MONO_8 1
#endif

#if PIXEL_OPERATOR_MONO_8

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap_8[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xf8,

    /* U+0022 "\"" */
    0xf0,

    /* U+0023 "#" */
    0xbe, 0xfa,

    /* U+0024 "$" */
    0x5f, 0x3e, 0x90,

    /* U+0025 "%" */
    0xce, 0x43, 0x0,

    /* U+0026 "&" */
    0xf1, 0x56,

    /* U+0027 "'" */
    0xc0,

    /* U+0028 "(" */
    0x6a, 0x40,

    /* U+0029 ")" */
    0x95, 0x80,

    /* U+002A "*" */
    0xfd, 0x0,

    /* U+002B "+" */
    0x5d, 0x0,

    /* U+002C "," */
    0x80,

    /* U+002D "-" */
    0xc0,

    /* U+002E "." */
    0x80,

    /* U+002F "/" */
    0x2d, 0x48,

    /* U+0030 "0" */
    0xf7, 0xd4,

    /* U+0031 "1" */
    0x75, 0x40,

    /* U+0032 "2" */
    0xe1, 0x4e,

    /* U+0033 "3" */
    0xe5, 0x1e,

    /* U+0034 "4" */
    0x2f, 0x92,

    /* U+0035 "5" */
    0xf8, 0x9e,

    /* U+0036 "6" */
    0xf3, 0x54,

    /* U+0037 "7" */
    0xe5, 0x48,

    /* U+0038 "8" */
    0xf5, 0x54,

    /* U+0039 "9" */
    0xf5, 0x9e,

    /* U+003A ":" */
    0x90,

    /* U+003B ";" */
    0x90,

    /* U+003C "<" */
    0x74,

    /* U+003D "=" */
    0xf0,

    /* U+003E ">" */
    0xd8,

    /* U+003F "?" */
    0xe1, 0x24,

    /* U+0040 "@" */
    0xff, 0xfa, 0x60,

    /* U+0041 "A" */
    0xf7, 0xda,

    /* U+0042 "B" */
    0xf7, 0x5c,

    /* U+0043 "C" */
    0xf2, 0x4e,

    /* U+0044 "D" */
    0xf6, 0xdc,

    /* U+0045 "E" */
    0xf3, 0x4e,

    /* U+0046 "F" */
    0xf3, 0x48,

    /* U+0047 "G" */
    0xf2, 0xd6,

    /* U+0048 "H" */
    0xb7, 0xda,

    /* U+0049 "I" */
    0xf8,

    /* U+004A "J" */
    0x24, 0x9e,

    /* U+004B "K" */
    0xbb, 0x4a,

    /* U+004C "L" */
    0x92, 0x4e,

    /* U+004D "M" */
    0x9f, 0x99, 0x90,

    /* U+004E "N" */
    0xb7, 0xda,

    /* U+004F "O" */
    0xf6, 0xd4,

    /* U+0050 "P" */
    0xf7, 0x48,

    /* U+0051 "Q" */
    0xf6, 0xfe,

    /* U+0052 "R" */
    0xf7, 0x4a,

    /* U+0053 "S" */
    0xf1, 0x1e,

    /* U+0054 "T" */
    0xe9, 0x24,

    /* U+0055 "U" */
    0xb6, 0xde,

    /* U+0056 "V" */
    0xb6, 0xd4,

    /* U+0057 "W" */
    0x8d, 0x6b, 0x5f, 0x80,

    /* U+0058 "X" */
    0xb5, 0x5a,

    /* U+0059 "Y" */
    0xb5, 0x24,

    /* U+005A "Z" */
    0xe5, 0x4e,

    /* U+005B "[" */
    0xea, 0xc0,

    /* U+005C "\\" */
    0x99, 0x12,

    /* U+005D "]" */
    0xd5, 0xc0,

    /* U+005E "^" */
    0x74,

    /* U+005F "_" */
    0xe0,

    /* U+0060 "`" */
    0xc0,

    /* U+0061 "a" */
    0xee, 0xb0,

    /* U+0062 "b" */
    0x9a, 0xdc,

    /* U+0063 "c" */
    0xf2, 0x70,

    /* U+0064 "d" */
    0x2e, 0xd6,

    /* U+0065 "e" */
    0x5e, 0x70,

    /* U+0066 "f" */
    0xfa, 0x80,

    /* U+0067 "g" */
    0x75, 0x9e,

    /* U+0068 "h" */
    0x9a, 0xda,

    /* U+0069 "i" */
    0xf8,

    /* U+006A "j" */
    0x24, 0x93, 0xc0,

    /* U+006B "k" */
    0x97, 0x6a,

    /* U+006C "l" */
    0xf8,

    /* U+006D "m" */
    0xd5, 0x6b, 0x10,

    /* U+006E "n" */
    0xd6, 0xd0,

    /* U+006F "o" */
    0x56, 0xa0,

    /* U+0070 "p" */
    0xf6, 0xe8,

    /* U+0071 "q" */
    0xf6, 0xb2,

    /* U+0072 "r" */
    0xf2, 0x40,

    /* U+0073 "s" */
    0x5c, 0x70,

    /* U+0074 "t" */
    0xba, 0x40,

    /* U+0075 "u" */
    0xb6, 0xa0,

    /* U+0076 "v" */
    0xb6, 0xa0,

    /* U+0077 "w" */
    0x8d, 0x6a, 0xa0,

    /* U+0078 "x" */
    0xa8, 0x50,

    /* U+0079 "y" */
    0xb7, 0x9e,

    /* U+007A "z" */
    0xea, 0x70,

    /* U+007B "{" */
    0xe2, 0x40,

    /* U+007C "|" */
    0xf8,

    /* U+007D "}" */
    0xd1, 0x80,

    /* U+007E "~" */
    0x5a};

/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc_8[] = {
    {.bitmap_index = 0,
     .adv_w = 0,
     .box_w = 0,
     .box_h = 0,
     .ofs_x = 0,
     .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0,
     .adv_w = 64,
     .box_w = 1,
     .box_h = 1,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 1,
     .adv_w = 64,
     .box_w = 1,
     .box_h = 5,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 2,
     .adv_w = 64,
     .box_w = 2,
     .box_h = 2,
     .ofs_x = 1,
     .ofs_y = 3},
    {.bitmap_index = 3,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 5,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 5,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 7,
     .ofs_x = 0,
     .ofs_y = -1},
    {.bitmap_index = 8,
     .adv_w = 64,
     .box_w = 4,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 11,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 13,
     .adv_w = 64,
     .box_w = 1,
     .box_h = 2,
     .ofs_x = 1,
     .ofs_y = 3},
    {.bitmap_index = 14,
     .adv_w = 64,
     .box_w = 2,
     .box_h = 5,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 16,
     .adv_w = 64,
     .box_w = 2,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 18,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 3,
     .ofs_x = 0,
     .ofs_y = 2},
    {.bitmap_index = 20,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 3,
     .ofs_x = 0,
     .ofs_y = 1},
    {.bitmap_index = 22,
     .adv_w = 64,
     .box_w = 1,
     .box_h = 2,
     .ofs_x = 1,
     .ofs_y = -1},
    {.bitmap_index = 23,
     .adv_w = 64,
     .box_w = 2,
     .box_h = 1,
     .ofs_x = 1,
     .ofs_y = 2},
    {.bitmap_index = 24,
     .adv_w = 64,
     .box_w = 1,
     .box_h = 1,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 25,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 5,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 27,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 29,
     .adv_w = 64,
     .box_w = 2,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 31,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 33,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 35,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 37,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 39,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 41,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 43,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 45,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 47,
     .adv_w = 64,
     .box_w = 1,
     .box_h = 4,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 48,
     .adv_w = 64,
     .box_w = 1,
     .box_h = 5,
     .ofs_x = 1,
     .ofs_y = -1},
    {.bitmap_index = 49,
     .adv_w = 64,
     .box_w = 2,
     .box_h = 3,
     .ofs_x = 1,
     .ofs_y = 1},
    {.bitmap_index = 50,
     .adv_w = 64,
     .box_w = 2,
     .box_h = 2,
     .ofs_x = 1,
     .ofs_y = 2},
    {.bitmap_index = 51,
     .adv_w = 64,
     .box_w = 2,
     .box_h = 3,
     .ofs_x = 0,
     .ofs_y = 1},
    {.bitmap_index = 52,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 54,
     .adv_w = 64,
     .box_w = 4,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 57,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 59,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 61,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 63,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 65,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 67,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 69,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 71,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 73,
     .adv_w = 64,
     .box_w = 1,
     .box_h = 5,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 74,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 76,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 78,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 80,
     .adv_w = 64,
     .box_w = 4,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 83,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 85,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 87,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 89,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 91,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 93,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 95,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 97,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 99,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 101,
     .adv_w = 64,
     .box_w = 5,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 105,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 107,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 109,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 111,
     .adv_w = 64,
     .box_w = 2,
     .box_h = 5,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 113,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 5,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 115,
     .adv_w = 64,
     .box_w = 2,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 117,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 2,
     .ofs_x = 0,
     .ofs_y = 3},
    {.bitmap_index = 118,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 1,
     .ofs_x = 0,
     .ofs_y = -1},
    {.bitmap_index = 119,
     .adv_w = 64,
     .box_w = 1,
     .box_h = 2,
     .ofs_x = 1,
     .ofs_y = 4},
    {.bitmap_index = 120,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 4,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 122,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 124,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 4,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 126,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 128,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 4,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 130,
     .adv_w = 64,
     .box_w = 2,
     .box_h = 5,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 132,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = -1},
    {.bitmap_index = 134,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 136,
     .adv_w = 64,
     .box_w = 1,
     .box_h = 5,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 137,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 6,
     .ofs_x = 0,
     .ofs_y = -1},
    {.bitmap_index = 140,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 142,
     .adv_w = 64,
     .box_w = 1,
     .box_h = 5,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 143,
     .adv_w = 64,
     .box_w = 5,
     .box_h = 4,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 146,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 4,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 148,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 4,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 150,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = -1},
    {.bitmap_index = 152,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = -1},
    {.bitmap_index = 154,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 4,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 156,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 4,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 158,
     .adv_w = 64,
     .box_w = 2,
     .box_h = 5,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 160,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 4,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 162,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 4,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 164,
     .adv_w = 64,
     .box_w = 5,
     .box_h = 4,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 167,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 4,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 169,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = -1},
    {.bitmap_index = 171,
     .adv_w = 64,
     .box_w = 3,
     .box_h = 4,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 173,
     .adv_w = 64,
     .box_w = 2,
     .box_h = 5,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 175,
     .adv_w = 64,
     .box_w = 1,
     .box_h = 5,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 176,
     .adv_w = 64,
     .box_w = 2,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 178,
     .adv_w = 64,
     .box_w = 4,
     .box_h = 2,
     .ofs_x = 0,
     .ofs_y = 4}};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps_8[] = {
    {.range_start = 32,
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
static const lv_font_fmt_txt_dsc_t font_dsc_8 = {
#else
static lv_font_fmt_txt_dsc_t font_dsc_8 = {
#endif
    .glyph_bitmap = glyph_bitmap_8,
    .glyph_dsc = glyph_dsc_8,
    .cmaps = cmaps_8,
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
const lv_font_t pixel_operator_mono_8 = {
#else
lv_font_t pixel_operator_mono_8 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt, /*Function pointer to
                                                           get glyph's data*/
    .get_glyph_bitmap =
        lv_font_get_bitmap_fmt_txt, /*Function pointer to get glyph's bitmap*/
    .line_height = 7, /*The maximum line height required by the font*/
    .base_line = 1,   /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc_8, /*The custom font data. Will be accessed by
                         `get_glyph_bitmap_8/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};

#endif /*#if PIXEL_OPERATOR_MONO_8*/
