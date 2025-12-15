/*
 * Copyright (c) 2024 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <lvgl.h>
#include <zephyr/kernel.h>
#include "util.h"

/* Widget de modificadores: ahora se utiliza un label para mostrar el estado en texto */
struct zmk_widget_modifiers {
    sys_snode_t node;
    lv_obj_t *obj;
};

/**
 * Inicializa el widget de modificadores.
 *
 * @param widget Puntero a la estructura del widget.
 * @param parent Objeto padre de LVGL en el que se creará el widget.
 * @return 0 si la inicialización fue exitosa.
 */
int zmk_widget_modifiers_init(struct zmk_widget_modifiers *widget, lv_obj_t *parent);
