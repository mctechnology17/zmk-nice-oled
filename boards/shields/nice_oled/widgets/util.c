#include "util.h"
#include <ctype.h>
#include <zephyr/kernel.h>

void to_uppercase(char *str) {
  for (int i = 0; str[i] != '\0'; i++) {
    str[i] = toupper(str[i]);
  }
}

void rotate_canvas(lv_obj_t *canvas) {
  uint8_t *buf = lv_canvas_get_draw_buf(canvas)->data;
  static uint8_t buf_copy[CANVAS_BUF_SIZE];
  memcpy(buf_copy, buf, sizeof(buf_copy));

  const uint32_t stride = lv_draw_buf_width_to_stride(CANVAS_HEIGHT, CANVAS_COLOR_FORMAT);
  lv_draw_sw_rotate(buf_copy, buf, CANVAS_HEIGHT, CANVAS_HEIGHT, stride, stride,
                    LV_DISPLAY_ROTATION_270, CANVAS_COLOR_FORMAT);
}

void draw_background(lv_obj_t *canvas) {
  lv_draw_rect_dsc_t rect_black_dsc;
  init_rect_dsc(&rect_black_dsc, LVGL_BACKGROUND);

  canvas_draw_rect(canvas, 0, 0, CANVAS_WIDTH, CANVAS_HEIGHT, &rect_black_dsc);
}

void init_label_dsc(lv_draw_label_dsc_t *label_dsc, lv_color_t color,
                    const lv_font_t *font, lv_text_align_t align) {
  lv_draw_label_dsc_init(label_dsc);
  label_dsc->color = color;
  label_dsc->font = font;
  label_dsc->align = align;
}

void init_rect_dsc(lv_draw_rect_dsc_t *rect_dsc, lv_color_t bg_color) {
  lv_draw_rect_dsc_init(rect_dsc);
  rect_dsc->bg_color = bg_color;
}

void init_line_dsc(lv_draw_line_dsc_t *line_dsc, lv_color_t color,
                   uint8_t width) {
  lv_draw_line_dsc_init(line_dsc);
  line_dsc->color = color;
  line_dsc->width = width;
}

void canvas_draw_line(lv_obj_t *canvas, const lv_point_t points[], uint32_t point_cnt,
                      lv_draw_line_dsc_t *draw_dsc) {
  lv_layer_t layer;
  lv_canvas_init_layer(canvas, &layer);

  for (uint32_t i = 1; i < point_cnt; ++i) {
    draw_dsc->p1.x = points[i - 1].x;
    draw_dsc->p1.y = points[i - 1].y;
    draw_dsc->p2.x = points[i].x;
    draw_dsc->p2.y = points[i].y;
    lv_draw_line(&layer, draw_dsc);
  }

  lv_canvas_finish_layer(canvas, &layer);
}

void canvas_draw_rect(lv_obj_t *canvas, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h,
                      lv_draw_rect_dsc_t *draw_dsc) {
  lv_layer_t layer;
  lv_canvas_init_layer(canvas, &layer);

  lv_area_t coords = {x, y, x + w - 1, y + h - 1};
  lv_draw_rect(&layer, draw_dsc, &coords);

  lv_canvas_finish_layer(canvas, &layer);
}

void canvas_draw_text(lv_obj_t *canvas, lv_coord_t x, lv_coord_t y, lv_coord_t max_w,
                      lv_draw_label_dsc_t *draw_dsc, const char *txt) {
  lv_layer_t layer;
  lv_canvas_init_layer(canvas, &layer);

  draw_dsc->text = txt;
  lv_area_t coords = {x, y, x + max_w, y + CANVAS_HEIGHT};
  lv_draw_label(&layer, draw_dsc, &coords);

  lv_canvas_finish_layer(canvas, &layer);
}

void canvas_draw_img(lv_obj_t *canvas, lv_coord_t x, lv_coord_t y, const lv_image_dsc_t *src,
                     lv_draw_image_dsc_t *draw_dsc) {
  lv_layer_t layer;
  lv_canvas_init_layer(canvas, &layer);

  draw_dsc->src = src;
  lv_area_t coords = {x, y, x + src->header.w - 1, y + src->header.h - 1};
  lv_draw_image(&layer, draw_dsc, &coords);

  lv_canvas_finish_layer(canvas, &layer);
}
