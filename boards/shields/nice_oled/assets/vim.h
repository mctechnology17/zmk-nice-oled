#ifndef _5_OUTPUT_IMAGES_ROTATE_FLIP_H
#define _5_OUTPUT_IMAGES_ROTATE_FLIP_H
#ifdef __cplusplus
extern "C" {
#endif
#include "lvgl.h"
// -------- Image Descriptors Declarations --------
LV_IMG_DECLARE(vim_32x100);
LV_IMG_DECLARE(vim_32x128);
LV_IMG_DECLARE(vim_68x69);
LV_IMG_DECLARE(vim_68x160);


// -------- Array of Pointers to Image Descriptors --------
// Provides easy access to all images defined in the corresponding .c file
const lv_img_dsc_t *5_output_images_rotate_flip_images[4] = {
    &vim_32x100,
    &vim_32x128,
    &vim_68x69,
    &vim_68x160
};

#define 5_OUTPUT_IMAGES_ROTATE_FLIP_IMAGES_NUM_IMAGES 4

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* _5_OUTPUT_IMAGES_ROTATE_FLIP_H */
