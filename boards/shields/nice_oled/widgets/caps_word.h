/*
 * ========================================
 * FileName: caps_word.h
 * Date: 19.November.2026
 * Author: Marcos Chow Castro
 * Email: mctechnology170318@gmail.com
 * GitHub: https://github.com/mctechnology17
 * Brief: caps word state for the widgets of zmk-nice-oled: one place, every widget reads it the same way.
 * =========================================
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <stdbool.h>
#include <zephyr/kernel.h>
#include <zmk/event_manager.h>

/*
 * How a widget follows caps word:
 *
 *   #include "caps_word.h"
 *   ... nice_oled_caps_word_active() inside its get_state ...
 *   NICE_OLED_CAPS_WORD_SUBSCRIPTIONS(widget_name);   // after ZMK_DISPLAY_WIDGET_LISTENER
 *
 * Without CONFIG_NICE_OLED_CAPS_WORD the function returns false and the macro adds nothing, so the
 * widget needs no #if of its own.
 */
#if IS_ENABLED(CONFIG_NICE_OLED_CAPS_WORD)

#include <zmk/events/keycode_state_changed.h>
#include <zmk/events/position_state_changed.h>

bool nice_oled_caps_word_active(void);

/*
 * ZMK raises no event when caps word changes. The key that turns it on sends no keycode, so the
 * release of that key (a position event) is what shows the new state; the key that ends it is seen
 * with its keycode event.
 */
#define NICE_OLED_CAPS_WORD_SUBSCRIPTIONS(listener)                                                \
    ZMK_SUBSCRIPTION(listener, zmk_position_state_changed);                                        \
    ZMK_SUBSCRIPTION(listener, zmk_keycode_state_changed)

#else

static inline bool nice_oled_caps_word_active(void) { return false; }

#define NICE_OLED_CAPS_WORD_SUBSCRIPTIONS(listener)

#endif
