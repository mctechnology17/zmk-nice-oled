#include "util.h"
#include <ctype.h>
#include <zephyr/kernel.h>

void to_uppercase(char *str) {
  for (int i = 0; str[i] != '\0'; i++) {
    str[i] = toupper(str[i]);
  }
}

void rotate_canvas(lv_obj_t *canvas, lv_color_t cbuf[]) {
  static lv_color_t cbuf_tmp[CANVAS_HEIGHT * CANVAS_HEIGHT];
  memcpy(cbuf_tmp, cbuf, sizeof(cbuf_tmp));

  lv_img_dsc_t img;
  img.data = (void *)cbuf_tmp;
  img.header.cf = LV_IMG_CF_TRUE_COLOR;
  img.header.w = CANVAS_HEIGHT;
  img.header.h = CANVAS_HEIGHT;

  lv_canvas_fill_bg(canvas, LVGL_BACKGROUND, LV_OPA_COVER);
  lv_canvas_transform(canvas, &img, 900, LV_IMG_ZOOM_NONE, -1, 0,
                      CANVAS_HEIGHT / 2, CANVAS_HEIGHT / 2, false);
}

void draw_background(lv_obj_t *canvas) {
  lv_draw_rect_dsc_t rect_black_dsc;
  init_rect_dsc(&rect_black_dsc, LVGL_BACKGROUND);

  lv_canvas_draw_rect(canvas, 0, 0, CANVAS_WIDTH, CANVAS_HEIGHT,
                      &rect_black_dsc);
}

void draw_pointer_status(lv_obj_t *canvas, uint8_t x, uint8_t y, uint8_t buttons, uint8_t ripple) {
  lv_draw_line_dsc_t line_dsc;
  init_line_dsc(&line_dsc, LVGL_FOREGROUND, buttons ? 2 : 1);

  int px = MIN(x, CANVAS_WIDTH - 1);
  int py = MIN(y, CANVAS_HEIGHT - 1);
  int left = MAX(px - 2, 0);
  int right = MIN(px + 2, CANVAS_WIDTH - 1);
  int top = MAX(py - 2, 0);
  int bottom = MIN(py + 2, CANVAS_HEIGHT - 1);

  lv_point_t h_points[] = {{left, py}, {right, py}};
  lv_point_t v_points[] = {{px, top}, {px, bottom}};
  lv_canvas_draw_line(canvas, h_points, 2, &line_dsc);
  lv_canvas_draw_line(canvas, v_points, 2, &line_dsc);

  if (ripple > 0) {
    lv_draw_rect_dsc_t rect_dsc;
    init_rect_dsc(&rect_dsc, LVGL_BACKGROUND);
    rect_dsc.border_color = LVGL_FOREGROUND;
    rect_dsc.border_width = 1;
    rect_dsc.bg_opa = LV_OPA_TRANSP;

    int radius = 2 + ripple;
    int rx = MAX(px - radius, 0);
    int ry = MAX(py - radius, 0);
    int rw = MIN((radius * 2) + 1, CANVAS_WIDTH - rx);
    int rh = MIN((radius * 2) + 1, CANVAS_HEIGHT - ry);
    lv_canvas_draw_rect(canvas, rx, ry, rw, rh, &rect_dsc);
  }
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
