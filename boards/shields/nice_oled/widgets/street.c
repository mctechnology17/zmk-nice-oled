/*
 * ========================================
 * FileName: street.c
 * Date: 19.November.2026
 * Author: Marcos Chow Castro
 * Email: mctechnology170318@gmail.com
 * GitHub: https://github.com/mctechnology17
 * Brief: street character widget of zmk-nice-oled: runs with the WPM, fights with the modifiers and takes a bat out with Caps Lock.
 * =========================================
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/kernel.h>

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#include <zmk/display.h>
#include <zmk/event_manager.h>
#include <zmk/events/keycode_state_changed.h>
#include <zmk/events/wpm_state_changed.h>
#include <zmk/hid.h>
#include <zmk/wpm.h>
#if IS_ENABLED(CONFIG_ZMK_HID_INDICATORS)
#include <zmk/events/hid_indicators_changed.h>
#include <zmk/hid_indicators.h>
#endif

#include <dt-bindings/zmk/modifiers.h>

#include "street.h"

#define HID_LED_CAPS_LOCK 0x02

/* Frames of every animation, big (nice!view) or small (32 pixel wide OLED). */
#include "../assets/street_images.h"

enum street_anim {
    STREET_ANIM_IDLE,
    STREET_ANIM_WALK,
    STREET_ANIM_RUN,
    STREET_ANIM_SPRINT,
    STREET_ANIM_JUMP,
    STREET_ANIM_PUNCH,
    STREET_ANIM_KICK,
    STREET_ANIM_UPPER,
    STREET_ANIM_BAT,
    STREET_ANIM_COUNT,
    STREET_ANIM_NONE,
};

struct street_loop {
    const lv_image_dsc_t *const *frames;
    uint8_t count;
    uint16_t ms; /* time of one loop */
};

#define STREET_LOOP(name, loop_ms)                                                                 \
    { .frames = street_##name##_frames, .count = ARRAY_SIZE(street_##name##_frames), .ms = loop_ms }

static const struct street_loop street_loops[STREET_ANIM_COUNT] = {
    [STREET_ANIM_IDLE] = STREET_LOOP(idle, 1800),  [STREET_ANIM_WALK] = STREET_LOOP(walk, 880),
    [STREET_ANIM_RUN] = STREET_LOOP(run, 560),     [STREET_ANIM_SPRINT] = STREET_LOOP(sprint, 400),
    [STREET_ANIM_JUMP] = STREET_LOOP(jump, 680),   [STREET_ANIM_PUNCH] = STREET_LOOP(punch, 360),
    [STREET_ANIM_KICK] = STREET_LOOP(kick, 440),   [STREET_ANIM_UPPER] = STREET_LOOP(upper, 440),
    [STREET_ANIM_BAT] = STREET_LOOP(bat, 480),
};

struct street_state {
    uint8_t wpm;
    uint8_t mods;
    bool caps;
    bool bat;       /* bat switched on from the keymap with &street */
    uint8_t action; /* animation held from the keymap with &street, STREET_ANIM_NONE if none */
};

/* Values of <behaviors/street.dtsi>. */
#define STREET_KEY_BAT_OFF 0
#define STREET_KEY_BAT_ON 1
#define STREET_KEY_BAT_TOG 2
#define STREET_KEY_JUMP 10
#define STREET_KEY_PUNCH 11
#define STREET_KEY_KICK 12
#define STREET_KEY_UPPER 13
#define STREET_KEY_BAT 14

static bool street_bat_from_keymap;
static uint8_t street_action_from_keymap = STREET_ANIM_NONE;

static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);
static enum street_anim current_anim = STREET_ANIM_NONE;

/*
 * A key of the keymap (&street) wins over everything. Then Caps Lock or the bat
 * switch, then the modifiers, and last the typing speed.
 */
static enum street_anim street_pick(struct street_state state) {
    if (state.action != STREET_ANIM_NONE) {
        return state.action;
    }
    if (state.caps || state.bat) {
        return STREET_ANIM_BAT;
    }
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_STREET_MODIFIERS)
    if (state.mods & (MOD_LGUI | MOD_RGUI)) {
        return STREET_ANIM_JUMP;
    }
    if (state.mods & (MOD_LCTL | MOD_RCTL)) {
        return STREET_ANIM_PUNCH;
    }
    if (state.mods & (MOD_LALT | MOD_RALT)) {
        return STREET_ANIM_KICK;
    }
    if (state.mods & (MOD_LSFT | MOD_RSFT)) {
        return STREET_ANIM_UPPER;
    }
#endif
    if (state.wpm < CONFIG_NICE_OLED_WIDGET_STREET_WPM_WALK) {
        return STREET_ANIM_IDLE;
    }
    if (state.wpm < CONFIG_NICE_OLED_WIDGET_STREET_WPM_RUN) {
        return STREET_ANIM_WALK;
    }
    if (state.wpm < CONFIG_NICE_OLED_WIDGET_STREET_WPM_SPRINT) {
        return STREET_ANIM_RUN;
    }
    return STREET_ANIM_SPRINT;
}

static void street_set_animation(lv_obj_t *obj, enum street_anim anim) {
    const struct street_loop *loop = &street_loops[anim];

    lv_animimg_set_src(obj, (const void **)loop->frames, loop->count);
    lv_animimg_set_duration(obj, loop->ms);
    lv_animimg_set_repeat_count(obj, LV_ANIM_REPEAT_INFINITE);
    lv_animimg_start(obj);
}

static void street_update_cb(struct street_state state) {
    enum street_anim anim = street_pick(state);

    if (anim == current_anim) {
        return;
    }
    current_anim = anim;

    struct zmk_widget_street *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) { street_set_animation(widget->obj, anim); }
}

/* Every event only says "something changed": the state is read again in full. */
static struct street_state street_get_state(const zmk_event_t *eh) {
    ARG_UNUSED(eh);

    return (struct street_state){
        .wpm = zmk_wpm_get_state(),
        .mods = zmk_hid_get_explicit_mods(),
#if IS_ENABLED(CONFIG_ZMK_HID_INDICATORS)
        .caps = (zmk_hid_indicators_get_current_profile() & HID_LED_CAPS_LOCK) != 0,
#else
        .caps = false,
#endif
        .bat = street_bat_from_keymap,
        .action = street_action_from_keymap,
    };
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_street, struct street_state, street_update_cb, street_get_state)

ZMK_SUBSCRIPTION(widget_street, zmk_wpm_state_changed);
ZMK_SUBSCRIPTION(widget_street, zmk_keycode_state_changed);
#if IS_ENABLED(CONFIG_ZMK_HID_INDICATORS)
ZMK_SUBSCRIPTION(widget_street, zmk_hid_indicators_changed);
#endif

void zmk_widget_street_key(uint8_t param, bool pressed) {
    uint8_t action = STREET_ANIM_NONE;

    switch (param) {
    case STREET_KEY_BAT_OFF:
    case STREET_KEY_BAT_ON:
    case STREET_KEY_BAT_TOG:
        /* The switches act on the press only. */
        if (!pressed) {
            return;
        }
        street_bat_from_keymap =
            (param == STREET_KEY_BAT_TOG) ? !street_bat_from_keymap : (param == STREET_KEY_BAT_ON);
        widget_street_refresh_state(NULL);
        return;
    case STREET_KEY_JUMP:
        action = STREET_ANIM_JUMP;
        break;
    case STREET_KEY_PUNCH:
        action = STREET_ANIM_PUNCH;
        break;
    case STREET_KEY_KICK:
        action = STREET_ANIM_KICK;
        break;
    case STREET_KEY_UPPER:
        action = STREET_ANIM_UPPER;
        break;
    case STREET_KEY_BAT:
        action = STREET_ANIM_BAT;
        break;
    default:
        return;
    }

    if (pressed) {
        street_action_from_keymap = action;
    } else if (street_action_from_keymap == action) {
        /* Only the key that started the animation ends it. */
        street_action_from_keymap = STREET_ANIM_NONE;
    }
    widget_street_refresh_state(NULL);
}

int zmk_widget_street_init(struct zmk_widget_street *widget, lv_obj_t *parent) {
    widget->obj = lv_animimg_create(parent);

    sys_slist_append(&widgets, &widget->node);

    widget_street_init();

    return 0;
}

lv_obj_t *zmk_widget_street_obj(struct zmk_widget_street *widget) { return widget->obj; }

/* vim: set ft=c fdm=marker: */
