#include <stdbool.h>
#include <stdint.h>

// uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
//     static uint16_t non_mod_input_timer;
//     non_mod_input_timer = timer_read();
//     switch (keycode) {
//         case HRM_N:
//         case HRM_T:
//         case HRM_R:
//         case HRM_I:
//         case HRM_E:
//         case HRM_S:
//           return timer_elapsed(non_mod_input_timer) > 500 ? 50 : TAPPING_TERM + 100;
//           /* return TAPPING_TERM; */
//         case HRM_A:
//           return TAPPING_TERM*2;
//         case HRM_O:
//           return TAPPING_TERM*2;
//         default:
//           return TAPPING_TERM;
//     }
// }

// bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
//     switch (keycode) {
//         case HRM_A:
//         case HRM_R:
//         case HRM_O:
//         case HRM_I:
//             return true;
//         default:
//             return false;
//     }
// }

enum rgb_background {
    RGB_BG_OFF,      // No backlight
    RGB_BG_WHITE     // Static white backlight
};

static enum rgb_background current_rgb_bg = RGB_BG_OFF;

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    bool is_set;
} rgb_layer_color_t;

// Max expected layers.
#define MAX_RGB_LAYERS 10

// Instead of querying by layer + (col, row) on every frame (which is slow),
// we pre-compute and store the intended color for every LED physically on the board,
// per layer. LED_MATRIX_LED_COUNT or similar is typically available,
// but we can just use the absolute max QMK allows for most boards (or a generous size like 100)
#define MAX_LEDS 100
static rgb_layer_color_t layer_led_colors[MAX_RGB_LAYERS][MAX_LEDS];
static bool layer_has_colors[MAX_RGB_LAYERS] = {false};

#define RGB_BASE_COLOR 0xFFFF

// Helper function to easily register colors per keycode inside post_init
void register_color_for_keycode(uint8_t layer, uint16_t target_keycode, uint8_t red, uint8_t green, uint8_t blue) {
    layer_has_colors[layer] = true;
    for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
        for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
            uint8_t led_index = g_led_config.matrix_co[row][col];
            if (led_index != NO_LED && led_index < MAX_LEDS) {
                uint16_t key = keymap_key_to_keycode(layer, (keypos_t){col, row});

                // If it's the exact key, or if we passed RGB_BASE_COLOR to match ANY key on that layer that isn't empty (KC_TRNS or KC_NO)
                if ((key == target_keycode) || (target_keycode == RGB_BASE_COLOR && key > KC_TRNS && !layer_led_colors[layer][led_index].is_set)) {
                    layer_led_colors[layer][led_index].red = red;
                    layer_led_colors[layer][led_index].green = green;
                    layer_led_colors[layer][led_index].blue = blue;
                    layer_led_colors[layer][led_index].is_set = true;
                }
            }
        }
    }
}

void keyboard_post_init_user(void) {
    switch (current_rgb_bg) {
        case RGB_BG_OFF:
            rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
            rgb_matrix_sethsv_noeeprom(HSV_OFF);
            break;
        case RGB_BG_WHITE:
            rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
            rgb_matrix_sethsv_noeeprom(HSV_WHITE);
            break;
    }

    // --- POPULATE RGB MATRIX MAPPINGS ---
    // Here we map keys to colors once at boot!

    // 1. Set specific overrides FIRST

    // NAV layer - Page navigation
    register_color_for_keycode(_NAV, KC_HOME, 0, 255, 0);   // Green
    register_color_for_keycode(_NAV, KC_END,  0, 255, 0);   // Green
    register_color_for_keycode(_NAV, KC_PGUP, 255, 0, 0);   // Red
    register_color_for_keycode(_NAV, KC_PGDN, 255, 0, 0);   // Red

    // NAV layer - Arrow keys
    register_color_for_keycode(_NAV, KC_LEFT, 0, 255, 255);  // Cyan
    register_color_for_keycode(_NAV, KC_DOWN,  0, 255, 255);  // Cyan
    register_color_for_keycode(_NAV, KC_UP,    0, 255, 255);  // Cyan
    register_color_for_keycode(_NAV, KC_RGHT,  0, 255, 255);  // Cyan

    // NAV layer - Modifiers
    register_color_for_keycode(_NAV, KC_LSFT, 255, 255, 0);   // Yellow
    register_color_for_keycode(_NAV, KC_LCTL, 255, 255, 0);   // Yellow
    register_color_for_keycode(_NAV, KC_LALT, 255, 255, 0);   // Yellow
    register_color_for_keycode(_NAV, KC_LGUI, 255, 255, 0);   // Yellow

    // NAV layer - Special actions
    register_color_for_keycode(_NAV, KC_SCRL, 255, 120, 0);   // Orange
    register_color_for_keycode(_NAV, KC_DEL,  255, 120, 0);   // Orange

    // FKEYS layer - Function keys (color per row for easy identification)
    register_color_for_keycode(_FKEYS, KC_F1,  0, 200, 200);    // Teal (bottom row)
    register_color_for_keycode(_FKEYS, KC_F2,  0, 200, 200);    // Teal
    register_color_for_keycode(_FKEYS, KC_F3,  0, 200, 200);    // Teal
    register_color_for_keycode(_FKEYS, KC_F4,  0, 200, 200);    // Teal
    register_color_for_keycode(_FKEYS, KC_F5,  0, 100, 255);   // Blue (middle row)
    register_color_for_keycode(_FKEYS, KC_F6,  0, 100, 255);   // Blue
    register_color_for_keycode(_FKEYS, KC_F7,  0, 100, 255);   // Blue
    register_color_for_keycode(_FKEYS, KC_F8,  0, 100, 255);   // Blue
    register_color_for_keycode(_FKEYS, KC_F9,  128, 0, 255);   // Purple (top row)
    register_color_for_keycode(_FKEYS, KC_F10, 128, 0, 255);   // Purple
    register_color_for_keycode(_FKEYS, KC_F11, 128, 0, 255);   // Purple
    register_color_for_keycode(_FKEYS, KC_F12, 128, 0, 255);   // Purple

    // FKEYS layer - Modifiers
    register_color_for_keycode(_FKEYS, KC_RSFT, 255, 255, 0); // Yellow
    register_color_for_keycode(_FKEYS, KC_RCTL, 255, 255, 0); // Yellow
    register_color_for_keycode(_FKEYS, KC_LALT, 255, 255, 0); // Yellow
    register_color_for_keycode(_FKEYS, KC_RGUI, 255, 255, 0); // Yellow

    // ADJUST layer - Layer switches
    register_color_for_keycode(_ADJUST, BASE,   0, 255, 0);   // Green
    register_color_for_keycode(_ADJUST, GAMING, 255, 0, 0);   // Red
    register_color_for_keycode(_ADJUST, QWERTY, 0, 100, 255); // Blue

    // ADJUST layer - RGB controls
    register_color_for_keycode(_ADJUST, RGB_MODE_TOGGLE, 255, 0, 255); // Magenta (custom toggle)
    register_color_for_keycode(_ADJUST, RM_TOGG, 255, 0, 0);           // Red (kills all lights)
    register_color_for_keycode(_ADJUST, RM_VALU, 255, 255, 0);         // Yellow (brightness up)
    register_color_for_keycode(_ADJUST, RM_VALD, 255, 255, 0);         // Yellow (brightness down)
    register_color_for_keycode(_ADJUST, RM_SATU, 255, 120, 0);         // Orange (saturation up)
    register_color_for_keycode(_ADJUST, RM_SATD, 255, 120, 0);         // Orange (saturation down)
    register_color_for_keycode(_ADJUST, RM_NEXT, 0, 255, 255);         // Cyan (cycle modes)

    // ADJUST layer - Media controls
    register_color_for_keycode(_ADJUST, KC_VOLU, 255, 255, 0); // Yellow (volume)
    register_color_for_keycode(_ADJUST, KC_VOLD, 255, 255, 0); // Yellow (volume)
    register_color_for_keycode(_ADJUST, KC_MUTE, 255, 0, 0);   // Red (mute = stop sound)
    register_color_for_keycode(_ADJUST, KC_MRWD, 0, 100, 255); // Blue (prev track)
    register_color_for_keycode(_ADJUST, KC_MFFD, 0, 100, 255); // Blue (next track)
    register_color_for_keycode(_ADJUST, KC_MPLY, 0, 255, 0);   // Green (play/pause)

    // ADJUST layer - Brightness
    register_color_for_keycode(_ADJUST, KC_BRIU, 255, 255, 0); // Yellow
    register_color_for_keycode(_ADJUST, KC_BRID, 255, 255, 0); // Yellow

    // ADJUST layer - Other function key
    register_color_for_keycode(_ADJUST, KC_F13, 128, 0, 255); // Purple

    // 2. Set base layer colors AFTER (using RGB_BASE_COLOR to indicate "any other filled keycode")
    register_color_for_keycode(_NAV, RGB_BASE_COLOR, 0, 50, 255);      // Blue base for NAV
    register_color_for_keycode(_FKEYS, RGB_BASE_COLOR, 255, 0, 0);     // Red base for FKEYS
    register_color_for_keycode(_ADJUST, RGB_BASE_COLOR, 0, 255, 0);    // Green base for ADJUST
}


bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    uint8_t layer = get_highest_layer(layer_state);

    if (layer < MAX_RGB_LAYERS && layer_has_colors[layer]) {
        for (uint8_t i = led_min; i < led_max; ++i) {
            if (i < MAX_LEDS && layer_led_colors[layer][i].is_set) {
                rgb_matrix_set_color(i,
                    layer_led_colors[layer][i].red,
                    layer_led_colors[layer][i].green,
                    layer_led_colors[layer][i].blue
                );
            }
        }
    }
    return false;
}


bool undead(uint16_t  character, bool pressed) {
  if (pressed) {
    tap_code16(character);
    tap_code(KC_SPC);
  }
  return false;
}

void unmod(uint16_t keycode) {
  uint8_t mod_state = get_mods();
  uint8_t osm_state = get_oneshot_mods();
  uint8_t weak_mods_state = get_weak_mods();
  /* clear_mods(); clear_oneshot_mods(); clear_macro_mods(); clear_weak_mods(); */
  unregister_mods(MOD_MASK_SHIFT);
  del_oneshot_mods(MOD_MASK_SHIFT);
  del_weak_mods(MOD_MASK_SHIFT);
  tap_code16(keycode);
  register_mods(mod_state);
  add_oneshot_mods(osm_state);
  add_weak_mods(weak_mods_state);
}

bool accented_letter(uint16_t accent, uint16_t letter, bool pressed) {
  if (pressed) {                        // On press:
    unmod(accent);                      // Press accent, unmoded
    register_code(letter);              // Press letter.
  } else {                              // On release: (this is for repeats on hold down)
    unregister_code(letter);            // Release letter
  }                                     // If shift is pressed it'll be released by the user
  return false;                         // Don't continue with the key handling.
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    const bool pressed = record->event.pressed;

    switch (keycode) {
        case RGB_MODE_TOGGLE:
            if (record->event.pressed) {
                current_rgb_bg = (current_rgb_bg + 1) % 2;
                keyboard_post_init_user();  // Re-apply the RGB mode
            }
            return false;

        // Undead characters
        case UD_APO: return undead(KC_QUOT, pressed);
        case UD_GRV: return undead(KC_GRV, pressed);
        case UD_TLD: return undead(S(KC_GRV), pressed);
        case UD_CIRC: return undead(S(KC_6), pressed);

        // Accented letters
        case E_ACUTE: return accented_letter(KC_QUOT, KC_E, pressed);
        case E_GRV: return accented_letter(KC_GRV, KC_E, pressed);
        case E_CIRC: return accented_letter(S(KC_6), KC_E, pressed);
        case A_GRV: return accented_letter(KC_GRV, KC_A, pressed);
        case U_GRV: return accented_letter(KC_GRV, KC_U, pressed);
        case A_CIRC: return accented_letter(S(KC_6), KC_A, pressed);
        case I_CIRC: return accented_letter(S(KC_6), KC_I, pressed);
        case O_CIRC: return accented_letter(S(KC_6), KC_O, pressed);
        case U_CIRC: return accented_letter(S(KC_6), KC_U, pressed);
        case C_CED: return accented_letter(KC_QUOT, KC_C, pressed);


        default:
          return true;
    }

    return true;
}
