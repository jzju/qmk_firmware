#include QMK_KEYBOARD_H
#include "debug.h"
#include "action_layer.h"
#include "keymap_swedish.h"

#define BASE 0 // default layer
#define SYMB 2 // symbols
#define GAME 1 // Gameing
#define CTRL 3 // QWERTY Ctrl
#define STAR 4
#define GSYM 5


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap 0: Basic layer
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |   Esc  |   7  |   5  |   3  |   1  |   9  |      |           | Mute |   8  |   0  |   2  |   4  |   6  |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |   Tab  |   Q  |   W  |   F  |   P  |   B  |      |           |  Up  |   J  |   L  |   U  |   Y  |   -  |   ´    |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |   Tab  |   A  |   R  |   S  |   T  |   G  |------|           |------|   M  |   N  |   E  |   I  |   O  |   '    |
 * |--------+------+------+------+------+------|      |           | Down |------+------+------+------+------+--------|
 * | LShift |   Z  |   X  |   C  |   D  |   V  |      |           |      |   K  |   H  |   ,  |   .  |   /  | RShift |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   | Ctrl |      |  Win |  Alt | MDIA |                                       | Ctrl |      |      |      |      |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        | Del  | GAME |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      | Home |       | PgUp |      |      |
 *                                 | Space| SYMB |------|       |------| BkSp |Enter |
 *                                 |      |      | End  |       | PgDn |      |      |
 *                                 `--------------------'       `--------------------'
 */
// If it accepts an argument (i.e, is a function), it doesn't need KC_.
// Otherwise, it needs KC_*
[BASE] = LAYOUT_ergodox(  // layer 0 : default
        // left hand
        KC_ESC,      KC_7,      KC_5,   KC_3,   KC_1,   KC_9,   KC_MPLY,
        KC_TAB,      KC_Q,      KC_W,   KC_F,   KC_P,   KC_B,   KC_WAKE,
        KC_TAB,      KC_A,      KC_R,   KC_S,   KC_T,   KC_G,
        KC_LSFT,     KC_Z,      KC_X,   KC_C,   KC_D,   KC_V,   KC_CAPS,
        LM(CTRL,MOD_LCTL),KC_NO,KC_LGUI,KC_LALT,KC_LALT,
                                                      KC_NO, TO(GAME),
                                                              KC_PGUP,
                                              KC_SPC,MO(SYMB),KC_PGDN,
        // right hand
        KC_MUTE,   KC_0,   KC_2,   KC_4,   KC_6,   KC_8,    KC_GRV,
        KC_VOLU,   KC_J,   KC_L,   KC_U,   KC_Y,   NO_MINS, NO_ACUT,
                   KC_M,   KC_N,   KC_E,   KC_I,   KC_O,    NO_APOS,
        KC_VOLD,   KC_K,   KC_H,   KC_COMM,KC_DOT, NO_SLSH, KC_RSFT,
                           KC_LCTL,  KC_LALT,  KC_NO,  KC_NO,   KC_NO,
        TO(STAR),        KC_DEL,
        KC_HOME,
        KC_END,KC_BSPC,KC_ENT
    ),

[SYMB] = LAYOUT_ergodox(
       // left hand
       KC_NO, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_NO,
       KC_NO, KC_EXLM, NO_AT,   NO_LCBR, NO_RCBR, KC_PERC, KC_NO,
       KC_NO, KC_HASH, NO_DLR,  NO_LPRN, NO_RPRN, NO_AMPR,
       KC_LSFT, NO_LESS, NO_GRTR, NO_LBRC, NO_RBRC, NO_PIPE, KC_NO,
       KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
                                           KC_NO, KC_NO,
                                                  KC_NO,
                                  KC_NO, KC_NO, KC_NO,
       // right hand
       KC_MUTE,   KC_F6, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,
       KC_VOLU, NO_CIRC, NO_EQL,  NO_ASTR, NO_PLUS, NO_QUES, KC_F12,
                NO_QUO2, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_PSCR,
       KC_VOLD, NO_TILD, NO_AA,   NO_AE,   NO_OSLH, NO_BSLS, KC_RSFT,
                         KC_BTN2,KC_NO,KC_NO,KC_NO, KC_NO,
       KC_NO, KC_NO,
       KC_NO,
       KC_NO, KC_BTN1, KC_BTN3
),

[GAME] = LAYOUT_ergodox(
        // left hand
        KC_ESC,   KC_1,      KC_2,   KC_3,   KC_4,   KC_5,   KC_6,
        KC_I,     KC_TAB,    KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,
        KC_M,     KC_CAPS,   KC_A,   KC_S,   KC_D,   KC_F,
        KC_H,     KC_LSFT,   KC_Z,   KC_X,   KC_C,   KC_V,   KC_G,
        KC_B,     KC_LCTL,   KC_NO, KC_LGUI,KC_LALT,
                                                     KC_LOCK, TO(BASE),
                                                               KC_PGUP,
                                              KC_SPC,MO(GSYM), KC_PGDN,
        // right hand
        KC_NO, KC_6,   KC_7,  KC_8,   KC_9,   KC_0,        KC_NO,
        KC_NO, KC_Y,   KC_U,  KC_I,   KC_O,   KC_P,        KC_NO,
                 KC_H,   KC_J,  KC_K,   KC_L,   KC_NO,     KC_NO,
        KC_NO,     KC_N,   KC_M,  KC_NO,KC_NO, KC_NO,     KC_NO,
                             KC_LEFT,KC_DOWN,KC_UP,  KC_RGHT, KC_NO,
        KC_NO,KC_NO,
        KC_NO,
        KC_NO,KC_NO,KC_NO
),

[CTRL] = LAYOUT_ergodox(
    // left hand
    KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,   KC_NO,
    KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,   KC_NO,
    KC_BSPC, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,
    KC_LSFT, KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,   KC_B,
    KC_LCTL, KC_NO, KC_LGUI, KC_LALT, KC_NO,
                                                   KC_H,  KC_J,
                                                          KC_O,
                                           KC_SPC, KC_N,  KC_P,
    // right hand
    KC_NO,     KC_6,   KC_7,   KC_8,    KC_9,    KC_0,       NO_PLUS,
    KC_NO,     KC_Y,   KC_U,   KC_I,    KC_O,    KC_P,       NO_AA,
               KC_H,   KC_J,   KC_K,    KC_L,    NO_OSLH,    NO_AE,
    KC_NO,     KC_N,   KC_M,   KC_COMM, KC_DOT,  NO_MINS,    KC_RSFT,
                       KC_LEFT,KC_DOWN, KC_UP,   KC_RGHT,    KC_NO,
    KC_NO,   KC_NO,
    KC_NO,
    KC_NO,   KC_NO,  KC_NO
),

[STAR] = LAYOUT_ergodox(  // layer 0 : default
        // left hand
        KC_ESC,      KC_1,      KC_2,   KC_3,   KC_4,   KC_5,   KC_6,
        KC_F10,    KC_Q,      KC_W,   KC_F,   KC_P,   KC_B,   KC_7,
        KC_TAB,    KC_A,      KC_R,   KC_S,   KC_T,   KC_G,
        KC_LSFT,   KC_Z,      KC_X,   KC_C,   KC_D,   KC_V,   KC_8,
        KC_LCTL,KC_NO,KC_LGUI,KC_LALT,KC_LALT,
                                                      KC_DEL, KC_ESC,
                                                              KC_HOME,
                                              KC_LCTL,KC_SPC,KC_END,
        // right hand
        KC_MUTE,   KC_8,   KC_0,   KC_2,   KC_4,   KC_6,    KC_F10,
        KC_VOLU,   KC_J,   KC_L,   KC_U,   KC_Y,   NO_MINS, NO_ACUT,
                   KC_M,   KC_N,   KC_E,   KC_I,   KC_O,    NO_APOS,
        KC_VOLD,   KC_K,   KC_H,   KC_COMM,KC_DOT, NO_SLSH, KC_RSFT,
                           KC_LCTL,  KC_NO,  KC_NO,  KC_NO,   KC_NO,
        TO(BASE),        KC_NO,
        KC_PGUP,
        KC_PGDN,KC_BSPC,KC_ENT
),

[GSYM] = LAYOUT_ergodox(
       // left hand
       KC_NO, KC_F1,   KC_F2, KC_F3,   KC_F4,   KC_F5,   KC_NO,
       KC_NO, KC_EXLM, KC_P1, KC_P2, KC_P3, KC_NLCK, KC_NO,
       KC_NO, KC_HASH, KC_P4, KC_P5, KC_P6, NO_AMPR,
       KC_NO, NO_LESS, KC_P7, KC_P8, KC_P9, NO_PIPE, KC_NO,
       KC_NO, KC_NO, KC_NO, KC_NO, KC_P0,
                                           KC_NO, KC_NO,
                                                    KC_NO,
                                  KC_NO, KC_NO, KC_NO,
       // right hand
       KC_MUTE,   KC_F6, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,
       KC_VOLU, NO_CIRC, NO_EQL,  NO_ASTR, NO_PLUS, NO_QUES, KC_F12,
                NO_QUO2, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_PSCR,
       KC_VOLD, NO_TILD, NO_AA,   NO_AE,   NO_OSLH, NO_BSLS, KC_SLEP,
                         KC_NO,KC_NO,KC_NO,KC_NO, KC_NO,
       KC_NO, KC_NO,
       KC_NO,
       KC_NO, KC_BTN2, KC_BTN1
),

};


const uint16_t PROGMEM fn_actions[] = {
    [1] = ACTION_LAYER_TAP_TOGGLE(SYMB)                // FN1 - Momentary Layer 1 (Symbols)
};

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
  // MACRODOWN only works in this function
      switch(id) {
        case 0:
        if (record->event.pressed) {
          register_code(KC_RSFT);
        } else {
          unregister_code(KC_RSFT);
        }
        break;
      }
    return MACRO_NONE;
};

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {

};

// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {

    uint8_t layer = biton32(layer_state);

    ergodox_board_led_off();
    ergodox_right_led_1_off();
    ergodox_right_led_2_off();
    ergodox_right_led_3_off();
    switch (layer) {
      // TODO: Make this relevant to the ErgoDox EZ.
        case 1:
            ergodox_right_led_1_on();
            break;
        case 2:
            ergodox_right_led_2_on();
            break;
        case 3:
            ergodox_right_led_1_on();
            ergodox_right_led_2_on();
            break;
        case 4:
            ergodox_right_led_3_on();
            break;
        default:
            // none
            break;
    }

};