/*
 * Compatibility shims for the LVGL 8 canvas drawing API removed in LVGL 9.
 *
 * lv_canvas_draw_image(), lv_canvas_draw_rect() and lv_canvas_draw_text()
 * no longer exist in LVGL 9.  They are re-implemented here on top of the
 * lv_canvas_init_layer() + lv_draw_*() + lv_canvas_finish_layer() pattern so
 * the existing drawing code keeps its immediate-mode semantics.
 */

#pragma once

#include <lvgl.h>

static inline void lv_canvas_draw_image(lv_obj_t *canvas, lv_coord_t x, lv_coord_t y,
                                        const void *src, lv_draw_image_dsc_t *dsc)
{
    const lv_image_dsc_t *img = (const lv_image_dsc_t *)src;
    lv_layer_t layer;
    lv_area_t area;

    lv_canvas_init_layer(canvas, &layer);
    dsc->src = src;
    area.x1 = x;
    area.y1 = y;
    area.x2 = x + img->header.w - 1;
    area.y2 = y + img->header.h - 1;
    lv_draw_image(&layer, dsc, &area);
    lv_canvas_finish_layer(canvas, &layer);
}

static inline void lv_canvas_draw_rect(lv_obj_t *canvas, lv_coord_t x, lv_coord_t y,
                                       lv_coord_t w, lv_coord_t h, lv_draw_rect_dsc_t *dsc)
{
    lv_layer_t layer;
    lv_area_t area;

    lv_canvas_init_layer(canvas, &layer);
    area.x1 = x;
    area.y1 = y;
    area.x2 = x + w - 1;
    area.y2 = y + h - 1;
    lv_draw_rect(&layer, dsc, &area);
    lv_canvas_finish_layer(canvas, &layer);
}

static inline void lv_canvas_draw_text(lv_obj_t *canvas, lv_coord_t x, lv_coord_t y,
                                       lv_coord_t max_w, lv_draw_label_dsc_t *dsc,
                                       const char *text)
{
    lv_layer_t layer;
    lv_area_t area;
    lv_coord_t line_h;

    lv_canvas_init_layer(canvas, &layer);
    dsc->text = text;
    line_h = dsc->font ? dsc->font->line_height : 8;
    area.x1 = x;
    area.y1 = y;
    area.x2 = x + max_w - 1;
    area.y2 = y + line_h - 1;
    lv_draw_label(&layer, dsc, &area);
    lv_canvas_finish_layer(canvas, &layer);
}

static inline void lv_canvas_draw_line(lv_obj_t *canvas, const lv_point_t *points,
                                       uint32_t point_cnt, lv_draw_line_dsc_t *dsc)
{
    lv_layer_t layer;
    uint32_t i;

    lv_canvas_init_layer(canvas, &layer);
    for (i = 0; i + 1 < point_cnt; i++) {
        dsc->p1 = lv_point_to_precise(&points[i]);
        dsc->p2 = lv_point_to_precise(&points[i + 1]);
        lv_draw_line(&layer, dsc);
    }
    lv_canvas_finish_layer(canvas, &layer);
}
