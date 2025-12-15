/*******************************************************************************
 * Size: 12 px
 * Bpp: 1
 * Opts: --font PixelOperatorMono.ttf --size 12 --format lvgl --bpp 1 --range
 *0x20-0x7F --no-compress -o pixel_operator_mono_12.c
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl.h"
#endif

#ifndef PIXEL_OPERATOR_MONO_12
#define PIXEL_OPERATOR_MONO_12 1
#endif

#if PIXEL_OPERATOR_MONO_12

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap_12[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xfc,

    /* U+0022 "\"" */
    0xf0,

    /* U+0023 "#" */
    0x57, 0xd4, 0xaf, 0xa8,

    /* U+0024 "$" */
    0x4b, 0xe4, 0xfa, 0x40,

    /* U+0025 "%" */
    0x6, 0xc, 0x81, 0x80,

    /* U+0026 "&" */
    0xf8, 0x69, 0x97,

    /* U+0027 "'" */
    0xc0,

    /* U+0028 "(" */
    0x72, 0x44, 0x40,

    /* U+0029 ")" */
    0xc4, 0x95, 0x0,

    /* U+002A "*" */
    0xaf, 0xc8,

    /* U+002B "+" */
    0x27, 0xc8,

    /* U+002C "," */
    0x60,

    /* U+002D "-" */
    0xf0,

    /* U+002E "." */
    0x80,

    /* U+002F "/" */
    0x25, 0x29, 0x0,

    /* U+0030 "0" */
    0xf9, 0xbd, 0x96,

    /* U+0031 "1" */
    0x74, 0x92, 0x40,

    /* U+0032 "2" */
    0xf0, 0x24, 0x8f,

    /* U+0033 "3" */
    0xf1, 0x61, 0x96,

    /* U+0034 "4" */
    0x35, 0x9f, 0x11,

    /* U+0035 "5" */
    0xf8, 0xf1, 0x96,

    /* U+0036 "6" */
    0xf8, 0xe9, 0x96,

    /* U+0037 "7" */
    0xf1, 0x24, 0x88,

    /* U+0038 "8" */
    0xf9, 0x69, 0x96,

    /* U+0039 "9" */
    0xf9, 0x71, 0x96,

    /* U+003A ":" */
    0x88,

    /* U+003B ";" */
    0x40, 0x60,

    /* U+003C "<" */
    0x2b, 0x10,

    /* U+003D "=" */
    0xff,

    /* U+003E ">" */
    0xae,

    /* U+003F "?" */
    0xf0, 0x22, 0x2,

    /* U+0040 "@" */
    0xfd, 0xf7, 0xbb, 0x38,

    /* U+0041 "A" */
    0x69, 0x9f, 0x99,

    /* U+0042 "B" */
    0xf9, 0xe9, 0x9e,

    /* U+0043 "C" */
    0xf8, 0x88, 0x96,

    /* U+0044 "D" */
    0xf9, 0x99, 0x9e,

    /* U+0045 "E" */
    0xf8, 0xe8, 0x8f,

    /* U+0046 "F" */
    0xf8, 0xe8, 0x88,

    /* U+0047 "G" */
    0xf8, 0xb9, 0x97,

    /* U+0048 "H" */
    0x99, 0xf9, 0x99,

    /* U+0049 "I" */
    0xfc,

    /* U+004A "J" */
    0x11, 0x11, 0x96,

    /* U+004B "K" */
    0x9a, 0xcc, 0xa9,

    /* U+004C "L" */
    0x88, 0x88, 0x8f,

    /* U+004D "M" */
    0x8e, 0xeb, 0x18, 0xc4,

    /* U+004E "N" */
    0x99, 0xdb, 0x99,

    /* U+004F "O" */
    0xf9, 0x99, 0x96,

    /* U+0050 "P" */
    0xf9, 0xe8, 0x88,

    /* U+0051 "Q" */
    0x60, 0x99, 0xb5,

    /* U+0052 "R" */
    0xf9, 0xe8, 0xa9,

    /* U+0053 "S" */
    0xf8, 0x61, 0x96,

    /* U+0054 "T" */
    0xf9, 0x8, 0x42, 0x10,

    /* U+0055 "U" */
    0x99, 0x99, 0x96,

    /* U+0056 "V" */
    0x99, 0x99, 0xf4,

    /* U+0057 "W" */
    0x8d, 0x6b, 0x5a, 0xa8,

    /* U+0058 "X" */
    0x99, 0x66, 0x99,

    /* U+0059 "Y" */
    0xb4, 0x24, 0x80,

    /* U+005A "Z" */
    0xf1, 0x24, 0x8f,

    /* U+005B "[" */
    0xf2, 0x49, 0xc0,

    /* U+005C "\\" */
    0x91, 0x22, 0x40,

    /* U+005D "]" */
    0xe4, 0x93, 0xc0,

    /* U+005E "^" */
    0xe9,

    /* U+005F "_" */
    0xf0,

    /* U+0060 "`" */
    0x90,

    /* U+0061 "a" */
    0x69, 0x79, 0x70,

    /* U+0062 "b" */
    0x8e, 0x99, 0x9e,

    /* U+0063 "c" */
    0x69, 0x89, 0x60,

    /* U+0064 "d" */
    0x17, 0x99, 0x97,

    /* U+0065 "e" */
    0x69, 0xf9, 0x60,

    /* U+0066 "f" */
    0x34, 0xf4, 0x44,

    /* U+0067 "g" */
    0xf9, 0x97, 0x96,

    /* U+0068 "h" */
    0x8e, 0x89, 0x99,

    /* U+0069 "i" */
    0xfc,

    /* U+006A "j" */
    0x11, 0x11, 0x19, 0x60,

    /* U+006B "k" */
    0x89, 0xac, 0xa9,

    /* U+006C "l" */
    0xfc,

    /* U+006D "m" */
    0xd4, 0x2b, 0x5a, 0x80,

    /* U+006E "n" */
    0xe8, 0x99, 0x90,

    /* U+006F "o" */
    0x69, 0x99, 0x60,

    /* U+0070 "p" */
    0xe9, 0x99, 0xe8,

    /* U+0071 "q" */
    0x79, 0x99, 0x71,

    /* U+0072 "r" */
    0xbe, 0x88, 0x80,

    /* U+0073 "s" */
    0x69, 0x69, 0x60,

    /* U+0074 "t" */
    0x4f, 0x44, 0x43,

    /* U+0075 "u" */
    0x99, 0x90, 0x60,

    /* U+0076 "v" */
    0x99, 0x96, 0x40,

    /* U+0077 "w" */
    0xad, 0x6a, 0x5, 0x0,

    /* U+0078 "x" */
    0x96, 0x46, 0x90,

    /* U+0079 "y" */
    0x99, 0x97, 0x96,

    /* U+007A "z" */
    0xf3, 0x4c, 0xf0,

    /* U+007B "{" */
    0x74, 0x84, 0x43,

    /* U+007C "|" */
    0xfc,

    /* U+007D "}" */
    0xe2, 0x12, 0x2c,

    /* U+007E "~" */
    0xdb};

/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc_12[] = {
    {.bitmap_index = 0,
     .adv_w = 0,
     .box_w = 0,
     .box_h = 0,
     .ofs_x = 0,
     .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0,
     .adv_w = 96,
     .box_w = 1,
     .box_h = 1,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 1,
     .adv_w = 96,
     .box_w = 1,
     .box_h = 6,
     .ofs_x = 2,
     .ofs_y = 0},
    {.bitmap_index = 2,
     .adv_w = 96,
     .box_w = 2,
     .box_h = 2,
     .ofs_x = 1,
     .ofs_y = 4},
    {.bitmap_index = 3,
     .adv_w = 96,
     .box_w = 5,
     .box_h = 6,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 7,
     .adv_w = 96,
     .box_w = 3,
     .box_h = 9,
     .ofs_x = 1,
     .ofs_y = -1},
    {.bitmap_index = 11,
     .adv_w = 96,
     .box_w = 5,
     .box_h = 6,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 15,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 6,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 18,
     .adv_w = 96,
     .box_w = 1,
     .box_h = 2,
     .ofs_x = 2,
     .ofs_y = 4},
    {.bitmap_index = 19,
     .adv_w = 96,
     .box_w = 3,
     .box_h = 6,
     .ofs_x = 2,
     .ofs_y = 0},
    {.bitmap_index = 22,
     .adv_w = 96,
     .box_w = 3,
     .box_h = 6,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 25,
     .adv_w = 96,
     .box_w = 5,
     .box_h = 3,
     .ofs_x = 0,
     .ofs_y = 3},
    {.bitmap_index = 27,
     .adv_w = 96,
     .box_w = 5,
     .box_h = 3,
     .ofs_x = 0,
     .ofs_y = 2},
    {.bitmap_index = 29,
     .adv_w = 96,
     .box_w = 2,
     .box_h = 2,
     .ofs_x = 1,
     .ofs_y = -1},
    {.bitmap_index = 30,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 1,
     .ofs_x = 1,
     .ofs_y = 3},
    {.bitmap_index = 31,
     .adv_w = 96,
     .box_w = 1,
     .box_h = 1,
     .ofs_x = 2,
     .ofs_y = 0},
    {.bitmap_index = 32,
     .adv_w = 96,
     .box_w = 3,
     .box_h = 6,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 35,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 6,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 38,
     .adv_w = 96,
     .box_w = 3,
     .box_h = 6,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 41,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 6,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 44,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 6,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 47,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 6,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 50,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 6,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 53,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 6,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 56,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 6,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 59,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 6,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 62,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 6,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 65,
     .adv_w = 96,
     .box_w = 1,
     .box_h = 5,
     .ofs_x = 2,
     .ofs_y = 0},
    {.bitmap_index = 66,
     .adv_w = 96,
     .box_w = 2,
     .box_h = 6,
     .ofs_x = 1,
     .ofs_y = -1},
    {.bitmap_index = 68,
     .adv_w = 96,
     .box_w = 3,
     .box_h = 4,
     .ofs_x = 2,
     .ofs_y = 1},
    {.bitmap_index = 70,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 2,
     .ofs_x = 1,
     .ofs_y = 2},
    {.bitmap_index = 71,
     .adv_w = 96,
     .box_w = 2,
     .box_h = 4,
     .ofs_x = 1,
     .ofs_y = 1},
    {.bitmap_index = 72,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 6,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 75,
     .adv_w = 96,
     .box_w = 5,
     .box_h = 6,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 79,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 6,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 82,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 6,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 85,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 6,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 88,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 6,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 91,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 6,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 94,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 6,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 97,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 6,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 100,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 6,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 103,
     .adv_w = 96,
     .box_w = 1,
     .box_h = 6,
     .ofs_x = 2,
     .ofs_y = 0},
    {.bitmap_index = 104,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 6,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 107,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 6,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 110,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 6,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 113,
     .adv_w = 96,
     .box_w = 5,
     .box_h = 6,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 117,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 6,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 120,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 6,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 123,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 6,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 126,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 6,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 129,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 6,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 132,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 6,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 135,
     .adv_w = 96,
     .box_w = 5,
     .box_h = 6,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 139,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 6,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 142,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 6,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 145,
     .adv_w = 96,
     .box_w = 5,
     .box_h = 6,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 149,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 6,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 152,
     .adv_w = 96,
     .box_w = 3,
     .box_h = 6,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 155,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 6,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 158,
     .adv_w = 96,
     .box_w = 3,
     .box_h = 6,
     .ofs_x = 2,
     .ofs_y = 0},
    {.bitmap_index = 161,
     .adv_w = 96,
     .box_w = 3,
     .box_h = 6,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 164,
     .adv_w = 96,
     .box_w = 3,
     .box_h = 6,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 167,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 2,
     .ofs_x = 1,
     .ofs_y = 4},
    {.bitmap_index = 168,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 1,
     .ofs_x = 1,
     .ofs_y = -2},
    {.bitmap_index = 169,
     .adv_w = 96,
     .box_w = 2,
     .box_h = 2,
     .ofs_x = 1,
     .ofs_y = 5},
    {.bitmap_index = 170,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 5,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 173,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 6,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 176,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 5,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 179,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 6,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 182,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 5,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 185,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 6,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 188,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 6,
     .ofs_x = 1,
     .ofs_y = -1},
    {.bitmap_index = 191,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 6,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 194,
     .adv_w = 96,
     .box_w = 1,
     .box_h = 6,
     .ofs_x = 2,
     .ofs_y = 0},
    {.bitmap_index = 195,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 7,
     .ofs_x = 1,
     .ofs_y = -1},
    {.bitmap_index = 199,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 6,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 202,
     .adv_w = 96,
     .box_w = 1,
     .box_h = 6,
     .ofs_x = 2,
     .ofs_y = 0},
    {.bitmap_index = 203,
     .adv_w = 96,
     .box_w = 5,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 207,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 5,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 210,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 5,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 213,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 6,
     .ofs_x = 1,
     .ofs_y = -1},
    {.bitmap_index = 216,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 6,
     .ofs_x = 1,
     .ofs_y = -1},
    {.bitmap_index = 219,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 5,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 222,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 5,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 225,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 6,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 228,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 5,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 231,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 5,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 234,
     .adv_w = 96,
     .box_w = 5,
     .box_h = 5,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 238,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 5,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 241,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 6,
     .ofs_x = 1,
     .ofs_y = -1},
    {.bitmap_index = 244,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 5,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 247,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 6,
     .ofs_x = 1,
     .ofs_y = 0},
    {.bitmap_index = 250,
     .adv_w = 96,
     .box_w = 1,
     .box_h = 6,
     .ofs_x = 2,
     .ofs_y = 0},
    {.bitmap_index = 251,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 6,
     .ofs_x = 0,
     .ofs_y = 0},
    {.bitmap_index = 254,
     .adv_w = 96,
     .box_w = 4,
     .box_h = 2,
     .ofs_x = 1,
     .ofs_y = 5}};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps_12[] = {
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
static const lv_font_fmt_txt_dsc_t font_dsc_12 = {
#else
static lv_font_fmt_txt_dsc_t font_dsc_12 = {
#endif
    .glyph_bitmap = glyph_bitmap_12,
    .glyph_dsc = glyph_dsc_12,
    .cmaps = cmaps_12,
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
const lv_font_t pixel_operator_mono_12 = {
#else
lv_font_t pixel_operator_mono_12 = {
#endif
    .get_glyph_dsc =
        lv_font_get_glyph_dsc_fmt_txt, /*Function pointer to get glyph's data*/
    .get_glyph_bitmap =
        lv_font_get_bitmap_fmt_txt, /*Function pointer to get glyph's bitmap*/
    .line_height = 10, /*The maximum line height required by the font*/
    .base_line = 2,    /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -2,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc_12, /*The custom font data. Will be accessed by
                         `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};

#endif /*#if PIXEL_OPERATOR_MONO_12*/
