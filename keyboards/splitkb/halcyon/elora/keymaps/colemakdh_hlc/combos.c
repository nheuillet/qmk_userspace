#include QMK_KEYBOARD_H
#include "custom_keycodes.h"

// French Accent Combos - F15 gated
// All combos only trigger when F15 is held (see combo_should_trigger in custom_functions.c)
//
// Uniform dead-key model: every chord is [accent trigger] + [letter].
//
// Grave:      L + E → è  |  L + A → à  |  L + U → ù
// Acute:      N + E → é
// Circumflex: H + E → ê  |  H + A → â  |  H + I → î  |  H + O → ô  |  H + U → û
// Trema:      M + E → ë  |  M + I → ï  |  M + U → ü
// Cedilla:    , + C → ç
//
// L = grave (`) trigger
// N = acute (´) partner
// H = circumflex (^) trigger for all hat accents
// M = trema (¨) trigger for all diaeresis accents
// COMM = cedilla (¸) trigger

enum combo_events {
    LE_E_GRAVE,        // L + E → è
    NE_E_ACUTE,        // N + E → é
    HE_E_CIRC,         // H + E → ê
    ME_E_TRM,          // M + E → ë
    LA_A_GRAVE,        // L + A → à
    HA_A_CIRC,         // H + A → â
    HI_I_CIRC,         // H + I → î
    MI_I_TRM,          // M + I → ï
    HO_O_CIRC,         // H + O → ô
    LU_U_GRAVE,        // L + U → ù
    HU_U_CIRC,         // H + U → û
    MU_U_TRM,          // M + U → ü
    CC_C_CED,          // , + C → ç
    COMBO_LENGTH
};

const uint16_t PROGMEM le_e_grave_combo[]  = {KC_L, KC_E, COMBO_END};
const uint16_t PROGMEM ne_e_acute_combo[]  = {KC_N, KC_E, COMBO_END};
const uint16_t PROGMEM he_e_circ_combo[]   = {KC_H, KC_E, COMBO_END};
const uint16_t PROGMEM me_e_trm_combo[]    = {KC_M, KC_E, COMBO_END};
const uint16_t PROGMEM la_a_grave_combo[]  = {KC_L, KC_A, COMBO_END};
const uint16_t PROGMEM ha_a_circ_combo[]   = {KC_H, KC_A, COMBO_END};
const uint16_t PROGMEM hi_i_circ_combo[]   = {KC_H, KC_I, COMBO_END};
const uint16_t PROGMEM mi_i_trm_combo[]    = {KC_M, KC_I, COMBO_END};
const uint16_t PROGMEM ho_o_circ_combo[]   = {KC_H, KC_O, COMBO_END};
const uint16_t PROGMEM lu_u_grave_combo[]  = {KC_L, KC_U, COMBO_END};
const uint16_t PROGMEM hu_u_circ_combo[]   = {KC_H, KC_U, COMBO_END};
const uint16_t PROGMEM mu_u_trm_combo[]    = {KC_M, KC_U, COMBO_END};
const uint16_t PROGMEM cc_c_ced_combo[]    = {KC_COMM, KC_C, COMBO_END};

combo_t key_combos[] = {
    [LE_E_GRAVE]  = COMBO(le_e_grave_combo,  E_GRV),
    [NE_E_ACUTE]  = COMBO(ne_e_acute_combo,  E_ACUTE),
    [HE_E_CIRC]   = COMBO(he_e_circ_combo,   E_CIRC),
    [ME_E_TRM]    = COMBO(me_e_trm_combo,    E_TRM),
    [LA_A_GRAVE]  = COMBO(la_a_grave_combo,  A_GRV),
    [HA_A_CIRC]   = COMBO(ha_a_circ_combo,   A_CIRC),
    [HI_I_CIRC]   = COMBO(hi_i_circ_combo,   I_CIRC),
    [MI_I_TRM]    = COMBO(mi_i_trm_combo,    I_TRM),
    [HO_O_CIRC]   = COMBO(ho_o_circ_combo,   O_CIRC),
    [LU_U_GRAVE]  = COMBO(lu_u_grave_combo,  U_GRV),
    [HU_U_CIRC]   = COMBO(hu_u_circ_combo,   U_CIRC),
    [MU_U_TRM]    = COMBO(mu_u_trm_combo,    U_TRM),
    [CC_C_CED]    = COMBO(cc_c_ced_combo,    C_CED),
};
