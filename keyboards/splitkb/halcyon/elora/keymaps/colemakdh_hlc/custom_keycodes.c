

// Modtaps
#define TAB_MEH MEH_T(KC_TAB)
#define ESC_ALT MT(MOD_LALT, KC_ESC)

// Home Row mods
// Left Side
#define HRM_A MT(MOD_LSFT, KC_A)
#define HRM_R MT(MOD_LCTL, KC_R)
#define HRM_S MT(MOD_LALT, KC_S)
#define HRM_T MT(MOD_LGUI, KC_T)

// Right Side
#define HRM_O MT(MOD_RSFT, KC_O)
#define HRM_I MT(MOD_RCTL, KC_I)
#define HRM_E MT(MOD_LALT, KC_E)
#define HRM_N MT(MOD_RGUI, KC_N)

// Qwerty home row mods
// KC_SCLN
// Left Side
#define HRM_QA MT(MOD_LSFT, KC_A)
#define HRM_QS MT(MOD_LCTL, KC_S)
#define HRM_QD MT(MOD_LALT, KC_D)
#define HRM_QF MT(MOD_LGUI, KC_F)

// Right Side
#define HRM_QSCLN MT(MOD_RSFT, KC_SCLN)
#define HRM_QL MT(MOD_RCTL, KC_L)
#define HRM_QK MT(MOD_LALT, KC_K)
#define HRM_QJ MT(MOD_RGUI, KC_J)


// Default Elora
#define CTL_ESC  MT(MOD_LCTL, KC_ESC)
#define CTL_QUOT MT(MOD_RCTL, KC_QUOTE)
#define CTL_MINS MT(MOD_RCTL, KC_MINUS)
#define ALT_ENT  MT(MOD_LALT, KC_ENT)

// Actual keycodes, compared to macros above
enum custom_keycodes {
RGB_MODE_TOGGLE = SAFE_RANGE, // Turn between rgb_mode values as per custom_functions.c
UD_APO,
UD_GRV,
UD_TLD,
UD_CIRC,

// Accented letters
E_ACUTE,
E_GRV,
E_CIRC,
A_GRV,
U_GRV,
A_CIRC,
I_CIRC,
O_CIRC,
U_CIRC,
C_CED,

// Trema letters
E_TRM,
I_TRM,
U_TRM,
};
