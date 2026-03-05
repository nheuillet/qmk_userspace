uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    static uint16_t non_mod_input_timer;
    non_mod_input_timer = timer_read();
    switch (keycode) {
        case HRM_N:
        case HRM_T:
        case HRM_R:
        case HRM_I:
        case HRM_E:
        case HRM_S:
          return timer_elapsed(non_mod_input_timer) > 500 ? 50 : TAPPING_TERM + 100;
          /* return TAPPING_TERM; */
        case HRM_A:
          return TAPPING_TERM*2;
        case HRM_O:
          return TAPPING_TERM*2;
        default:
          return TAPPING_TERM;
    }
}

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

static enum rgb_background current_rgb_bg = RGB_BG_WHITE;

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
}


bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    uint8_t layer = get_highest_layer(layer_state);

    // Only set colors for Nav, FKeys, and Adjust layers
    if (layer == _NAV || layer == _FKEYS || layer == _ADJUST) {
        // Define colors for each layer (R, G, B)
        uint8_t red, green, blue;
        switch (layer) {
            case _NAV:
                // Blue
                red = 0;
                green = 50;
                blue = 255;
                break;
            case _FKEYS:
                // Red
                red = 255;
                green = 0;
                blue = 0;
                break;
            case _ADJUST:
                // Gree,
                red = 0;
                green = 255;
                blue = 0;
                break;
            default:
                return false;
        }

        for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
            for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
                uint8_t index = g_led_config.matrix_co[row][col];

                if (index >= led_min && index < led_max && index != NO_LED &&
                keymap_key_to_keycode(layer, (keypos_t){col,row}) > KC_TRNS) {
                    rgb_matrix_set_color(index, red, green, blue);
                }
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
