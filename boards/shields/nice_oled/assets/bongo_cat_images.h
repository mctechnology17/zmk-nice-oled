#ifndef _5_OUTPUT_IMAGES_ROTATE_FLIP_H
#define _5_OUTPUT_IMAGES_ROTATE_FLIP_H
#ifdef __cplusplus
extern "C" {
#endif
#include "lvgl.h"
// -------- Image Descriptors Declarations --------
LV_IMG_DECLARE(bongo_cat_double_tap1_01);
LV_IMG_DECLARE(bongo_cat_double_tap1_02);
LV_IMG_DECLARE(bongo_cat_double_tap1_03);
LV_IMG_DECLARE(bongo_cat_double_tap1_04);
LV_IMG_DECLARE(bongo_cat_double_tap1_05);
LV_IMG_DECLARE(bongo_cat_double_tap1_06);
LV_IMG_DECLARE(bongo_cat_double_tap2_01);
LV_IMG_DECLARE(bongo_cat_double_tap2_02);
LV_IMG_DECLARE(bongo_cat_double_tap2_03);
LV_IMG_DECLARE(bongo_cat_tap1_01);
LV_IMG_DECLARE(bongo_cat_tap1_02);
LV_IMG_DECLARE(bongo_cat_tap1_03);
LV_IMG_DECLARE(bongo_cat_tap1_04);
LV_IMG_DECLARE(bongo_cat_tap2_01);
LV_IMG_DECLARE(bongo_cat_tap2_02);
LV_IMG_DECLARE(bongo_cat_tap2_03);
LV_IMG_DECLARE(bongo_cat_tap2_04);


// -------- Array of Pointers to Image Descriptors --------
// Provides easy access to all images defined in the corresponding .c file
const lv_img_dsc_t *5_output_images_rotate_flip_images[17] = {
    &bongo_cat_double_tap1_01,
    &bongo_cat_double_tap1_02,
    &bongo_cat_double_tap1_03,
    &bongo_cat_double_tap1_04,
    &bongo_cat_double_tap1_05,
    &bongo_cat_double_tap1_06,
    &bongo_cat_double_tap2_01,
    &bongo_cat_double_tap2_02,
    &bongo_cat_double_tap2_03,
    &bongo_cat_tap1_01,
    &bongo_cat_tap1_02,
    &bongo_cat_tap1_03,
    &bongo_cat_tap1_04,
    &bongo_cat_tap2_01,
    &bongo_cat_tap2_02,
    &bongo_cat_tap2_03,
    &bongo_cat_tap2_04
};

#define 5_OUTPUT_IMAGES_ROTATE_FLIP_IMAGES_NUM_IMAGES 17

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* _5_OUTPUT_IMAGES_ROTATE_FLIP_H */
