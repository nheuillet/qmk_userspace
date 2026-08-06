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

// --- Color palette: one definition per category, shared across all layers ---
// Pass these straight to register_color_for_keycode(); never hardcode RGB values.

#define C_EDIT 255, 120, 0      // Orange — editing actions (cut/copy/paste/undo/redo/select/find) & special actions (screen lock, RGB saturation)
#define C_MOVE 0, 255, 255      // Cyan — movement & navigation (arrows, page up/down, line start/end, RGB mode next)
#define C_MOVE_DEEP 0, 160, 255  // Deeper blue — EDIT layer text movement (line start/end)
#define C_PREFIX 128, 0, 255    // Purple — system prefixes & F-keys (tmux, generic prefixes, F13)
#define C_APP 255, 0, 255       // Magenta — app commands (command palette, devtools)
#define C_GO 0, 255, 0          // Green — positive/confirm (home/end, play, BASE layer)
#define C_MEDIA 0, 100, 255     // Blue — media prev/next, QWERTY layer
#define C_MOD 255, 255, 0       // Yellow — modifiers & level adjustments (mods, volume, brightness, RGB value)
#define C_DESTRUCT 255, 0, 0    // Red — destructive/stop (mute, delete, GAMING layer, RGB matrix toggle)
#define C_NEUTRAL 180, 180, 180 // White — neutral targets (combo accent vowels)
#define C_GOLD 255, 200, 0      // Gold — combo acute trigger

// F-key rows — one hue per row so you can locate F-keys by color
#define C_FROW_1 0, 200, 200 // Teal — F1–F4
#define C_FROW_2 0, 100, 255 // Blue — F5–F8
#define C_FROW_3 128, 0, 255 // Purple — F9–F12
#define C_FROW_4 255, 0, 255 // Magenta — F13–F16

// Combo indicator colors (accent triggers, shown while F15 is held)
#define C_COMBO_GRAVE 255, 110, 0 // Orange — grave trigger (L)
#define C_COMBO_ACUTE C_GOLD      // Gold — acute partner (N)
#define C_COMBO_CIRC 0, 220, 255  // Cyan — circumflex trigger (H)
#define C_COMBO_TRM 220, 0, 220   // Magenta — trema trigger (M)
#define C_COMBO_CED 0, 210, 60    // Green — cedilla trigger (COMM)
#define C_COMBO_VOWEL C_NEUTRAL   // White — accent vowels

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

    // NAV layer - Page navigation (movement = cyan)
    register_color_for_keycode(_NAV, KC_HOME, C_GO);   // Green — home
    register_color_for_keycode(_NAV, KC_END, C_GO);    // Green — end
    register_color_for_keycode(_NAV, KC_PGUP, C_MOVE); // Cyan — page up
    register_color_for_keycode(_NAV, KC_PGDN, C_MOVE); // Cyan — page down

    // NAV layer - Arrow keys (movement = cyan)
    register_color_for_keycode(_NAV, KC_LEFT, C_MOVE);
    register_color_for_keycode(_NAV, KC_DOWN, C_MOVE);
    register_color_for_keycode(_NAV, KC_UP, C_MOVE);
    register_color_for_keycode(_NAV, KC_RGHT, C_MOVE);

    // NAV layer - Modifiers
    register_color_for_keycode(_NAV, KC_LSFT, C_MOD);
    register_color_for_keycode(_NAV, KC_LCTL, C_MOD);
    register_color_for_keycode(_NAV, KC_LALT, C_MOD);
    register_color_for_keycode(_NAV, KC_LGUI, C_MOD);

    // NAV layer - Special actions
    register_color_for_keycode(_NAV, G(KC_L), C_EDIT);    // Orange — screen lock (Super+L)
    register_color_for_keycode(_NAV, KC_DEL, C_DESTRUCT); // Red — delete

    // NAV layer - Function keys (one hue per row)
    register_color_for_keycode(_NAV, KC_F1, C_FROW_1);  // Teal
    register_color_for_keycode(_NAV, KC_F2, C_FROW_1);  // Teal
    register_color_for_keycode(_NAV, KC_F3, C_FROW_1);  // Teal
    register_color_for_keycode(_NAV, KC_F4, C_FROW_1);  // Teal
    register_color_for_keycode(_NAV, KC_F5, C_FROW_2);  // Blue
    register_color_for_keycode(_NAV, KC_F6, C_FROW_2);  // Blue
    register_color_for_keycode(_NAV, KC_F7, C_FROW_2);  // Blue
    register_color_for_keycode(_NAV, KC_F8, C_FROW_2);  // Blue
    register_color_for_keycode(_NAV, KC_F9, C_FROW_3);  // Purple
    register_color_for_keycode(_NAV, KC_F10, C_FROW_3); // Purple
    register_color_for_keycode(_NAV, KC_F11, C_FROW_3); // Purple
    register_color_for_keycode(_NAV, KC_F12, C_FROW_3); // Purple
    register_color_for_keycode(_NAV, KC_F13, C_FROW_4); // Magenta
    register_color_for_keycode(_NAV, KC_F14, C_FROW_4); // Magenta
    register_color_for_keycode(_NAV, KC_F15, C_FROW_4); // Magenta
    register_color_for_keycode(_NAV, KC_F16, C_FROW_4); // Magenta

    // EDIT layer - Editing actions
    // Undo/Redo keep orange (C_EDIT) — reversals/undo = warm
    register_color_for_keycode(_EDIT, EDIT_UNDO, C_EDIT);
    register_color_for_keycode(_EDIT, EDIT_REDO, C_EDIT);
    // Cut/Copy/Paste go green (C_GO) — "do the clipboard action"
    register_color_for_keycode(_EDIT, EDIT_CUT, C_GO);
    register_color_for_keycode(_EDIT, EDIT_COPY, C_GO);
    register_color_for_keycode(_EDIT, EDIT_PASTE, C_GO);
    // Selection & find pop in yellow (C_MOD) — highlight/select association
    register_color_for_keycode(_EDIT, EDIT_SELALL, C_MOD);
    register_color_for_keycode(_EDIT, EDIT_FIND, C_MOD);

    // EDIT layer - Text movement (line start/end) — deeper blue, distinct from NAV cyan
    register_color_for_keycode(_EDIT, EDIT_LINE_START, C_MOVE_DEEP); // Ctrl+A — beginning of line
    register_color_for_keycode(_EDIT, EDIT_LINE_END, C_MOVE_DEEP);   // Ctrl+E — end of line

    // EDIT layer - App prefixes
    register_color_for_keycode(_EDIT, PREFIX_TMUX, C_PREFIX);
    register_color_for_keycode(_EDIT, PREFIX_GEN1, C_PREFIX);
    register_color_for_keycode(_EDIT, PREFIX_GEN2, C_PREFIX);
    register_color_for_keycode(_EDIT, PREFIX_GEN3, C_PREFIX);

    // EDIT layer - App commands
    register_color_for_keycode(_EDIT, APP_PALETTE, C_APP);  // Magenta
    register_color_for_keycode(_EDIT, APP_DEVTOOLS, C_APP); // Magenta

    // EDIT layer - Media
    register_color_for_keycode(_EDIT, KC_MPLY, C_GO);       // Green — play
    register_color_for_keycode(_EDIT, KC_MPRV, C_MEDIA);    // Blue — prev
    register_color_for_keycode(_EDIT, KC_MNXT, C_MEDIA);    // Blue — next
    register_color_for_keycode(_EDIT, KC_VOLU, C_MOD);      // Yellow — volume up
    register_color_for_keycode(_EDIT, KC_VOLD, C_MOD);      // Yellow — volume down
    register_color_for_keycode(_EDIT, KC_MUTE, C_DESTRUCT); // Red — mute

    // EDIT layer - Utility
    register_color_for_keycode(_EDIT, KC_DEL, C_DESTRUCT); // Red — delete

    // ADJUST layer - Layer switches
    register_color_for_keycode(_ADJUST, BASE, C_GO);         // Green
    register_color_for_keycode(_ADJUST, GAMING, C_DESTRUCT); // Red
    register_color_for_keycode(_ADJUST, QWERTY, C_MEDIA);    // Blue

    // ADJUST layer - RGB controls
    register_color_for_keycode(_ADJUST, RGB_MODE_TOGGLE, C_APP); // Magenta
    register_color_for_keycode(_ADJUST, RM_TOGG, C_DESTRUCT);    // Red
    register_color_for_keycode(_ADJUST, RM_VALU, C_MOD);         // Yellow
    register_color_for_keycode(_ADJUST, RM_VALD, C_MOD);         // Yellow
    register_color_for_keycode(_ADJUST, RM_SATU, C_EDIT);        // Orange
    register_color_for_keycode(_ADJUST, RM_SATD, C_EDIT);        // Orange
    register_color_for_keycode(_ADJUST, RM_NEXT, C_MOVE);        // Cyan

    // ADJUST layer - Brightness & misc
    register_color_for_keycode(_ADJUST, KC_BRIU, C_MOD);   // Yellow
    register_color_for_keycode(_ADJUST, KC_BRID, C_MOD);   // Yellow
    register_color_for_keycode(_ADJUST, KC_F13, C_PREFIX); // Purple

    // 2. Wildcard fills any remaining active key on each layer
    register_color_for_keycode(_NAV, RGB_WILDCARD, 0, 50, 255); // Blue base
    register_color_for_keycode(_ADJUST, RGB_WILDCARD, C_GO);    // Green base

    // --- COMBO INDICATOR COLORS (visible when F15 is held) ---
    // L = grave (`) trigger | N = acute (´) partner | H = circumflex (^) trigger
    // M = trema (¨) trigger | COMM = cedilla (¸) trigger | White vowels = accent targets
    register_combo_color(KC_L, C_COMBO_GRAVE);  // Orange — grave trigger
    register_combo_color(KC_N, C_COMBO_ACUTE);  // Gold   — acute partner (N+E → é)
    register_combo_color(KC_H, C_COMBO_CIRC);   // Cyan   — circumflex trigger
    register_combo_color(KC_M, C_COMBO_TRM);    // Magenta — trema trigger
    register_combo_color(KC_COMM, C_COMBO_CED); // Green  — cedilla trigger (,+C → ç)
    register_combo_color(KC_E, C_COMBO_VOWEL);  // White  — accent vowel
    register_combo_color(KC_A, C_COMBO_VOWEL);  // White  — accent vowel
    register_combo_color(KC_U, C_COMBO_VOWEL);  // White  — accent vowel
    register_combo_color(KC_I, C_COMBO_VOWEL);  // White  — accent vowel
    register_combo_color(KC_O, C_COMBO_VOWEL);  // White  — accent vowel
    register_combo_color(KC_C, C_COMBO_VOWEL);  // White  — accent vowel
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

// Send a modifier+key chord (e.g. Ctrl+Shift+P), preserving any mods already held.
static void send_chord(uint8_t mods, uint16_t keycode) {
    register_mods(mods);
    tap_code16(keycode);
    unregister_mods(mods);
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

        // EDIT layer - text editing (Ctrl chords)
        case EDIT_UNDO:
            if (pressed) send_chord(MOD_LCTL, KC_Z);
            return false;
        case EDIT_REDO:
            if (pressed) send_chord(MOD_LCTL | MOD_LSFT, KC_Z);
            return false;
        case EDIT_CUT:
            if (pressed) send_chord(MOD_LCTL, KC_X);
            return false;
        case EDIT_COPY:
            if (pressed) send_chord(MOD_LCTL, KC_C);
            return false;
        case EDIT_PASTE:
            if (pressed) send_chord(MOD_LCTL, KC_V);
            return false;
        case EDIT_SELALL:
            if (pressed) send_chord(MOD_LCTL, KC_A);
            return false;
        case EDIT_FIND:
            if (pressed) send_chord(MOD_LCTL, KC_F);
            return false;
        case EDIT_LINE_START: // Ctrl+A — beginning of line (terminals)
            if (pressed) send_chord(MOD_LCTL, KC_A);
            return false;
        case EDIT_LINE_END: // Ctrl+E — end of line (terminals)
            if (pressed) send_chord(MOD_LCTL, KC_E);
            return false;

        // EDIT layer - app commands
        case APP_PALETTE:
            if (pressed) send_chord(MOD_LCTL | MOD_LSFT, KC_P);
            return false;
        case APP_DEVTOOLS:
            if (pressed) send_chord(MOD_LCTL | MOD_LSFT, KC_I);
            return false;

        // EDIT layer - app prefixes
        case PREFIX_TMUX:
            if (pressed) send_chord(MOD_LCTL, KC_B);
            return false;
        case PREFIX_GEN1:
            if (pressed) send_chord(MOD_LCTL | MOD_LALT | MOD_LSFT, KC_RBRC);
            return false;
        case PREFIX_GEN2:
            if (pressed) send_chord(MOD_LCTL | MOD_LALT | MOD_LSFT, KC_LBRC);
            return false;
        case PREFIX_GEN3:
            if (pressed) send_chord(MOD_LCTL | MOD_LALT | MOD_LSFT, KC_TAB);
            return false;

        default:
            return true;
    }
}

// Only fire accent combos when F15 is held (COMBO_INDICATOR_LAYER active)
bool combo_should_trigger(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
    return layer_state_is(COMBO_INDICATOR_LAYER);
}
