#pragma once

// Swap hands feature - allows mirroring the keyboard for one-handed typing
// #define SWAP_HANDS_ENABLE

// Home row mods options
#define PERMISSIVE_HOLD_PER_KEY
#define TAPPING_TERM 150
#define TAPPING_TERM_PER_KEY
#define QUICK_TAP_TERM 50
// #undef TAPPING_FORCE_HOLD

// One shot mods timeout
#define ONESHOT_TIMEOUT 1000

// Combos - from Vermoot's keymap
#define COMBO_VARIABLE_LEN
#define COMBO_TERM 20
#define COMBO_TERM_PER_COMBO
#define COMBO_SHOULD_TRIGGER

// Caps Word settings
#define BOTH_SHIFTS_TURNS_ON_CAPS_WORD
#define DOUBLE_TAP_SHIFT_TURNS_ON_CAPS_WORD
#define CAPS_WORD_IDLE_TIMEOUT 2000 // milliseconds
