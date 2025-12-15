#ifndef _5_OUTPUT_IMAGES_ROTATE_FLIP_H
#define _5_OUTPUT_IMAGES_ROTATE_FLIP_H
#ifdef __cplusplus
extern "C" {
#endif
#include "lvgl.h"
// -------- Image Descriptors Declarations --------
LV_IMG_DECLARE(cat_0);
LV_IMG_DECLARE(cat_1);
LV_IMG_DECLARE(cat_2);
LV_IMG_DECLARE(cat_3);
LV_IMG_DECLARE(cat_4);
LV_IMG_DECLARE(cat_5);
LV_IMG_DECLARE(cat_6);
LV_IMG_DECLARE(cat_7);


// -------- Array of Pointers to Image Descriptors --------
// Provides easy access to all images defined in the corresponding .c file
const lv_img_dsc_t *5_output_images_rotate_flip_images[8] = {
    &cat_0,
    &cat_1,
    &cat_2,
    &cat_3,
    &cat_4,
    &cat_5,
    &cat_6,
    &cat_7
};

#define 5_OUTPUT_IMAGES_ROTATE_FLIP_IMAGES_NUM_IMAGES 8

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* _5_OUTPUT_IMAGES_ROTATE_FLIP_H */
