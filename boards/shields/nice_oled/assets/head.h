#ifndef _5_OUTPUT_IMAGES_ROTATE_FLIP_H
#define _5_OUTPUT_IMAGES_ROTATE_FLIP_H
#ifdef __cplusplus
extern "C" {
#endif
#include "lvgl.h"
// -------- Image Descriptors Declarations --------
LV_IMG_DECLARE(head_00);
LV_IMG_DECLARE(head_01);
LV_IMG_DECLARE(head_02);
LV_IMG_DECLARE(head_03);
LV_IMG_DECLARE(head_04);
LV_IMG_DECLARE(head_05);
LV_IMG_DECLARE(head_06);
LV_IMG_DECLARE(head_07);
LV_IMG_DECLARE(head_08);
LV_IMG_DECLARE(head_09);
LV_IMG_DECLARE(head_10);
LV_IMG_DECLARE(head_11);
LV_IMG_DECLARE(head_12);
LV_IMG_DECLARE(head_13);
LV_IMG_DECLARE(head_14);
LV_IMG_DECLARE(head_15);


// -------- Array of Pointers to Image Descriptors --------
// Provides easy access to all images defined in the corresponding .c file
const lv_img_dsc_t *5_output_images_rotate_flip_images[16] = {
    &head_00,
    &head_01,
    &head_02,
    &head_03,
    &head_04,
    &head_05,
    &head_06,
    &head_07,
    &head_08,
    &head_09,
    &head_10,
    &head_11,
    &head_12,
    &head_13,
    &head_14,
    &head_15
};

#define 5_OUTPUT_IMAGES_ROTATE_FLIP_IMAGES_NUM_IMAGES 16

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* _5_OUTPUT_IMAGES_ROTATE_FLIP_H */
