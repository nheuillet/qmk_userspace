#include QMK_KEYBOARD_H
#include "custom_keycodes.h"

// Virtual layer index used to signal "F15 is held" across both halves.
// Must be higher than any real layer so get_highest_layer() picks it up.
// Layer state is synced automatically via SPLIT_LAYER_STATE_ENABLE.
#define COMBO_INDICATOR_LAYER 9

enum rgb_background {
    RGB_BG_OFF,
    RGB_BG_WHITE,
};

static enum rgb_background current_rgb_bg = RGB_BG_OFF;

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    bool    is_set;
} rgb_layer_color_t;

// One color slot per LED per layer, pre-computed at boot.
#define MAX_RGB_LAYERS 10
static rgb_layer_color_t layer_led_colors[MAX_RGB_LAYERS][RGB_MATRIX_LED_COUNT];
static bool              layer_has_colors[MAX_RGB_LAYERS] = {false};

// Sentinel passed to register_color_for_keycode to mean "any non-empty key on this layer".
#define RGB_WILDCARD 0xFFFF

// --- Helpers ----------------------------------------------------------------

static void apply_rgb_background(void) {
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

// Map a specific keycode (or RGB_WILDCARD for "any key") on a given layer to an LED color.
// Specific keycodes always overwrite; RGB_WILDCARD only fills LEDs not yet assigned.
void register_color_for_keycode(uint8_t layer, uint16_t target_keycode, uint8_t red, uint8_t green, uint8_t blue) {
    layer_has_colors[layer] = true;
    for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
        for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
            uint8_t led_index = g_led_config.matrix_co[row][col];
            if (led_index == NO_LED || led_index >= RGB_MATRIX_LED_COUNT) continue;

            uint16_t key         = keymap_key_to_keycode(layer, (keypos_t){col, row});
            bool     is_wildcard = (target_keycode == RGB_WILDCARD && key > KC_TRNS && !layer_led_colors[layer][led_index].is_set);
            if (key != target_keycode && !is_wildcard) continue;

            layer_led_colors[layer][led_index].red    = red;
            layer_led_colors[layer][led_index].green  = green;
            layer_led_colors[layer][led_index].blue   = blue;
            layer_led_colors[layer][led_index].is_set = true;
        }
    }
}

// Like register_color_for_keycode but looks up position from the BASE layer
// and stores in the virtual COMBO_INDICATOR_LAYER.
void register_combo_color(uint16_t target_keycode, uint8_t red, uint8_t green, uint8_t blue) {
    layer_has_colors[COMBO_INDICATOR_LAYER] = true;
    for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
        for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
            uint8_t led_index = g_led_config.matrix_co[row][col];
            if (led_index == NO_LED || led_index >= RGB_MATRIX_LED_COUNT) continue;

            uint16_t key = keymap_key_to_keycode(_BASE, (keypos_t){col, row});
            if (key != target_keycode) continue;

            layer_led_colors[COMBO_INDICATOR_LAYER][led_index].red    = red;
            layer_led_colors[COMBO_INDICATOR_LAYER][led_index].green  = green;
            layer_led_colors[COMBO_INDICATOR_LAYER][led_index].blue   = blue;
            layer_led_colors[COMBO_INDICATOR_LAYER][led_index].is_set = true;
        }
    }
}

// --- QMK callbacks ----------------------------------------------------------

void keyboard_post_init_user(void) {
    apply_rgb_background();

    // --- POPULATE RGB MATRIX MAPPINGS ---
    // Colors are pre-computed once at boot; the indicator callback just reads this table.

    // 1. Specific key overrides first (wildcards fill the rest after)

    // NAV layer - Page navigation
    register_color_for_keycode(_NAV, KC_HOME, 0, 255, 0); // Green
    register_color_for_keycode(_NAV, KC_END, 0, 255, 0);  // Green
    register_color_for_keycode(_NAV, KC_PGUP, 255, 0, 0); // Red
    register_color_for_keycode(_NAV, KC_PGDN, 255, 0, 0); // Red

    // NAV layer - Arrow keys
    register_color_for_keycode(_NAV, KC_LEFT, 0, 255, 255); // Cyan
    register_color_for_keycode(_NAV, KC_DOWN, 0, 255, 255); // Cyan
    register_color_for_keycode(_NAV, KC_UP, 0, 255, 255);   // Cyan
    register_color_for_keycode(_NAV, KC_RGHT, 0, 255, 255); // Cyan

    // NAV layer - Modifiers
    register_color_for_keycode(_NAV, KC_LSFT, 255, 255, 0); // Yellow
    register_color_for_keycode(_NAV, KC_LCTL, 255, 255, 0); // Yellow
    register_color_for_keycode(_NAV, KC_LALT, 255, 255, 0); // Yellow
    register_color_for_keycode(_NAV, KC_LGUI, 255, 255, 0); // Yellow

    // NAV layer - Special actions
    register_color_for_keycode(_NAV, KC_SCRL, 255, 120, 0); // Orange
    register_color_for_keycode(_NAV, KC_DEL, 255, 120, 0);  // Orange

    // FKEYS layer - Function keys (color by row)
    register_color_for_keycode(_FKEYS, KC_F1, 0, 200, 200);  // Teal
    register_color_for_keycode(_FKEYS, KC_F2, 0, 200, 200);  // Teal
    register_color_for_keycode(_FKEYS, KC_F3, 0, 200, 200);  // Teal
    register_color_for_keycode(_FKEYS, KC_F4, 0, 200, 200);  // Teal
    register_color_for_keycode(_FKEYS, KC_F5, 0, 100, 255);  // Blue
    register_color_for_keycode(_FKEYS, KC_F6, 0, 100, 255);  // Blue
    register_color_for_keycode(_FKEYS, KC_F7, 0, 100, 255);  // Blue
    register_color_for_keycode(_FKEYS, KC_F8, 0, 100, 255);  // Blue
    register_color_for_keycode(_FKEYS, KC_F9, 128, 0, 255);  // Purple
    register_color_for_keycode(_FKEYS, KC_F10, 128, 0, 255); // Purple
    register_color_for_keycode(_FKEYS, KC_F11, 128, 0, 255); // Purple
    register_color_for_keycode(_FKEYS, KC_F12, 128, 0, 255); // Purple

    // FKEYS layer - Modifiers
    register_color_for_keycode(_FKEYS, KC_RSFT, 255, 255, 0); // Yellow
    register_color_for_keycode(_FKEYS, KC_RCTL, 255, 255, 0); // Yellow
    register_color_for_keycode(_FKEYS, KC_LALT, 255, 255, 0); // Yellow
    register_color_for_keycode(_FKEYS, KC_RGUI, 255, 255, 0); // Yellow

    // ADJUST layer - Layer switches
    register_color_for_keycode(_ADJUST, BASE, 0, 255, 0);     // Green
    register_color_for_keycode(_ADJUST, GAMING, 255, 0, 0);   // Red
    register_color_for_keycode(_ADJUST, QWERTY, 0, 100, 255); // Blue

    // ADJUST layer - RGB controls
    register_color_for_keycode(_ADJUST, RGB_MODE_TOGGLE, 255, 0, 255); // Magenta
    register_color_for_keycode(_ADJUST, RM_TOGG, 255, 0, 0);           // Red
    register_color_for_keycode(_ADJUST, RM_VALU, 255, 255, 0);         // Yellow
    register_color_for_keycode(_ADJUST, RM_VALD, 255, 255, 0);         // Yellow
    register_color_for_keycode(_ADJUST, RM_SATU, 255, 120, 0);         // Orange
    register_color_for_keycode(_ADJUST, RM_SATD, 255, 120, 0);         // Orange
    register_color_for_keycode(_ADJUST, RM_NEXT, 0, 255, 255);         // Cyan

    // ADJUST layer - Media controls
    register_color_for_keycode(_ADJUST, KC_VOLU, 255, 255, 0); // Yellow
    register_color_for_keycode(_ADJUST, KC_VOLD, 255, 255, 0); // Yellow
    register_color_for_keycode(_ADJUST, KC_MUTE, 255, 0, 0);   // Red
    register_color_for_keycode(_ADJUST, KC_MRWD, 0, 100, 255); // Blue
    register_color_for_keycode(_ADJUST, KC_MFFD, 0, 100, 255); // Blue
    register_color_for_keycode(_ADJUST, KC_MPLY, 0, 255, 0);   // Green

    // ADJUST layer - Brightness
    register_color_for_keycode(_ADJUST, KC_BRIU, 255, 255, 0); // Yellow
    register_color_for_keycode(_ADJUST, KC_BRID, 255, 255, 0); // Yellow
    register_color_for_keycode(_ADJUST, KC_F13, 128, 0, 255);  // Purple

    // 2. Wildcard fills any remaining active key on each layer
    register_color_for_keycode(_NAV, RGB_WILDCARD, 0, 50, 255);   // Blue base
    register_color_for_keycode(_FKEYS, RGB_WILDCARD, 255, 0, 0);  // Red base
    register_color_for_keycode(_ADJUST, RGB_WILDCARD, 0, 255, 0); // Green base

    // --- COMBO INDICATOR COLORS (visible when F15 is held) ---
    // H = circumflex (^) trigger | M = trema (¨) trigger
    // White vowels = accent receivers | Gold N = acute | Orange = grave/ô | Green = cedilla
    register_combo_color(KC_H, 0, 220, 255);   // Cyan   — circumflex trigger
    register_combo_color(KC_M, 220, 0, 220);   // Magenta — trema trigger
    register_combo_color(KC_E, 180, 180, 180); // White  — accent vowel
    register_combo_color(KC_A, 180, 180, 180); // White  — accent vowel
    register_combo_color(KC_U, 180, 180, 180); // White  — accent vowel
    register_combo_color(KC_N, 255, 200, 0);   // Gold   — acute partner (E+N → é)
    register_combo_color(KC_I, 255, 110, 0);   // Orange — grave partner (E+I → è) + ô pair
    register_combo_color(KC_S, 255, 110, 0);   // Orange — grave partner (A+S → à)
    register_combo_color(KC_Y, 255, 110, 0);   // Orange — grave partner (U+Y → ù)
    register_combo_color(KC_O, 255, 110, 0);   // Orange — ô pair (I+O → ô)
    register_combo_color(KC_X, 0, 210, 60);    // Green  — cedilla pair (X+C → ç)
    register_combo_color(KC_C, 0, 210, 60);    // Green  — cedilla pair
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    uint8_t layer = get_highest_layer(layer_state);

    if (layer < MAX_RGB_LAYERS && layer_has_colors[layer]) {
        for (uint8_t i = led_min; i < led_max; ++i) {
            if (layer_led_colors[layer][i].is_set) {
                rgb_matrix_set_color(i, layer_led_colors[layer][i].red, layer_led_colors[layer][i].green, layer_led_colors[layer][i].blue);
            }
        }
    }

    return false; // false = let QMK apply any remaining default indicators
}

// --- French accent helpers --------------------------------------------------

bool undead(uint16_t character, bool pressed) {
    if (pressed) {
        tap_code16(character);
        tap_code(KC_SPC);
    }
    return false;
}

void unmod(uint16_t keycode) {
    uint8_t mod_state       = get_mods();
    uint8_t osm_state       = get_oneshot_mods();
    uint8_t weak_mods_state = get_weak_mods();
    unregister_mods(MOD_MASK_SHIFT);
    del_oneshot_mods(MOD_MASK_SHIFT);
    del_weak_mods(MOD_MASK_SHIFT);
    tap_code16(keycode);
    register_mods(mod_state);
    add_oneshot_mods(osm_state);
    add_weak_mods(weak_mods_state);
}

bool accented_letter(uint16_t accent, uint16_t letter, bool pressed) {
    if (pressed) {
        unmod(accent);         // tap the dead key, stripping shift so it stays dead
        register_code(letter); // immediately follow with the letter
    } else {
        unregister_code(letter); // release on key-up (enables held-key repeat)
    }
    return false;
}

// --- QMK callbacks ----------------------------------------------------------

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    const bool pressed = record->event.pressed;

    // F15 activates the combo indicator layer so both halves reflect the state
    // (synced automatically via SPLIT_LAYER_STATE_ENABLE in config.h)
    if (keycode == KC_F15) {
        if (pressed) {
            layer_on(COMBO_INDICATOR_LAYER);
        } else {
            layer_off(COMBO_INDICATOR_LAYER);
        }
        return true;
    }

    switch (keycode) {
        case RGB_MODE_TOGGLE:
            if (pressed) {
                current_rgb_bg = (current_rgb_bg + 1) % 2;
                apply_rgb_background(); // only the background mode changes; LED colors stay pre-computed
            }
            return false;

        // Undead characters
        case UD_APO:
            return undead(KC_QUOT, pressed);
        case UD_GRV:
            return undead(KC_GRV, pressed);
        case UD_TLD:
            return undead(S(KC_GRV), pressed);
        case UD_CIRC:
            return undead(S(KC_6), pressed);

        // Quote key mod-tap: tap = literal ' or " (undead: dead key + space, so it
        // types immediately on layouts with dead keys); hold = Right Ctrl, which is
        // left to QMK's default mod-tap handling below.
        case CTL_QUOT:
            if (record->tap.count > 0) {
                return undead(KC_QUOT, pressed);
            }
            return true; // held: default mod-tap processing registers/unregisters RCtrl

        // Number row: Shift+6 types a literal ^ (undead on layouts with dead keys);
        // unshifted, 6 types normally.
        case KC_6:
            if (pressed && ((get_mods() | get_oneshot_mods() | get_weak_mods()) & MOD_MASK_SHIFT)) {
                return undead(S(KC_6), true);
            }
            return true; // plain 6 / release: default handling

        // Accented letters
        case E_ACUTE:
            return accented_letter(KC_QUOT, KC_E, pressed);
        case E_GRV:
            return accented_letter(KC_GRV, KC_E, pressed);
        case E_CIRC:
            return accented_letter(S(KC_6), KC_E, pressed);
        case A_GRV:
            return accented_letter(KC_GRV, KC_A, pressed);
        case U_GRV:
            return accented_letter(KC_GRV, KC_U, pressed);
        case A_CIRC:
            return accented_letter(S(KC_6), KC_A, pressed);
        case I_CIRC:
            return accented_letter(S(KC_6), KC_I, pressed);
        case O_CIRC:
            return accented_letter(S(KC_6), KC_O, pressed);
        case U_CIRC:
            return accented_letter(S(KC_6), KC_U, pressed);
        case C_CED:
            return accented_letter(KC_QUOT, KC_C, pressed);

        // Trema letters
        case E_TRM:
            return accented_letter(S(KC_QUOT), KC_E, pressed);
        case I_TRM:
            return accented_letter(S(KC_QUOT), KC_I, pressed);
        case U_TRM:
            return accented_letter(S(KC_QUOT), KC_U, pressed);

        default:
            return true;
    }
}

// Only fire accent combos when F15 is held (COMBO_INDICATOR_LAYER active)
bool combo_should_trigger(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
    return layer_state_is(COMBO_INDICATOR_LAYER);
}
