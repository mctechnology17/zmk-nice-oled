#include <stdlib.h>
#include <zephyr/kernel.h>
#include "animation.h"

LV_IMG_DECLARE(crystal_01);
LV_IMG_DECLARE(crystal_02);

const lv_img_dsc_t *anim_imgs[] = {
    &crystal_01,
    &crystal_02,
};

void draw_animation(lv_obj_t *canvas, struct zmk_widget_screen *widget) {
#if IS_ENABLED(CONFIG_NICE_OLED_GEM_ANIMATION)
    lv_obj_t *art = lv_animimg_create(widget->obj);
    lv_obj_center(art);

    lv_animimg_set_src(art, (const void **)anim_imgs, 2);
    lv_animimg_set_duration(art, CONFIG_NICE_OLED_GEM_ANIMATION_MS);
    lv_animimg_set_repeat_count(art, LV_ANIM_REPEAT_INFINITE);
    lv_animimg_start(art);
#else
    lv_obj_t *art = lv_img_create(widget->obj);

    int length = sizeof(anim_imgs) / sizeof(anim_imgs[0]);
    srand(k_uptime_get_32());
    int random_index = rand() % length;

    lv_img_set_src(art, anim_imgs[random_index]);
#endif

    lv_obj_align(art, LV_ALIGN_TOP_LEFT, 36, 0);
}
