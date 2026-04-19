#include QMK_KEYBOARD_H
#include "custom_keycodes.h"

// French Accent Combos - F15 gated
// All combos only trigger when F15 is held (see combo_should_trigger in custom_functions.c)
//
// Pattern for E:  E+I → è  |  E+N → é  |  H+E → ê  |  M+E → ë
// Pattern for A:  A+S → à  |  H+A → â
// Pattern for I:  H+I → î  |  M+I → ï
// Pattern for O:  I+O → ô
// Pattern for U:  U+Y → ù  |  H+U → û  |  M+U → ü
// Other:          X+C → ç
//
// H = circumflex (^) trigger for all hat accents
// M = trema (¨) trigger for all diaeresis accents

enum combo_events {
    EI_E_GRAVE,        // E + I → è
    EN_E_ACUTE,        // E + N → é
    HE_E_CIRC,         // H + E → ê
    ME_E_TRM,          // M + E → ë
    AS_A_GRAVE,        // A + S → à
    HA_A_CIRC,         // H + A → â
    IO_O_CIRC,         // I + O → ô
    HI_I_CIRC,         // H + I → î
    MI_I_TRM,          // M + I → ï
    HU_U_CIRC,         // H + U → û
    UY_U_GRAVE,        // U + Y → ù
    MU_U_TRM,          // M + U → ü
    XC_C_CED,          // X + C → ç
    COMBO_LENGTH
};

const uint16_t PROGMEM ei_e_grave_combo[]  = {KC_E, KC_I, COMBO_END};
const uint16_t PROGMEM en_e_acute_combo[]  = {KC_E, KC_N, COMBO_END};
const uint16_t PROGMEM he_e_circ_combo[]   = {KC_H, KC_E, COMBO_END};
const uint16_t PROGMEM me_e_trm_combo[]    = {KC_M, KC_E, COMBO_END};
const uint16_t PROGMEM as_a_grave_combo[]  = {KC_A, KC_S, COMBO_END};
const uint16_t PROGMEM ha_a_circ_combo[]   = {KC_H, KC_A, COMBO_END};
const uint16_t PROGMEM io_o_circ_combo[]   = {KC_I, KC_O, COMBO_END};
const uint16_t PROGMEM hi_i_circ_combo[]   = {KC_H, KC_I, COMBO_END};
const uint16_t PROGMEM mi_i_trm_combo[]    = {KC_M, KC_I, COMBO_END};
const uint16_t PROGMEM hu_u_circ_combo[]   = {KC_H, KC_U, COMBO_END};
const uint16_t PROGMEM uy_u_grave_combo[]  = {KC_U, KC_Y, COMBO_END};
const uint16_t PROGMEM mu_u_trm_combo[]    = {KC_M, KC_U, COMBO_END};
const uint16_t PROGMEM xc_c_ced_combo[]    = {KC_X, KC_C, COMBO_END};

combo_t key_combos[] = {
    [EI_E_GRAVE]  = COMBO(ei_e_grave_combo,  E_GRV),
    [EN_E_ACUTE]  = COMBO(en_e_acute_combo,  E_ACUTE),
    [HE_E_CIRC]   = COMBO(he_e_circ_combo,   E_CIRC),
    [ME_E_TRM]    = COMBO(me_e_trm_combo,    E_TRM),
    [AS_A_GRAVE]  = COMBO(as_a_grave_combo,  A_GRV),
    [HA_A_CIRC]   = COMBO(ha_a_circ_combo,   A_CIRC),
    [IO_O_CIRC]   = COMBO(io_o_circ_combo,   O_CIRC),
    [HI_I_CIRC]   = COMBO(hi_i_circ_combo,   I_CIRC),
    [MI_I_TRM]    = COMBO(mi_i_trm_combo,    I_TRM),
    [HU_U_CIRC]   = COMBO(hu_u_circ_combo,   U_CIRC),
    [UY_U_GRAVE]  = COMBO(uy_u_grave_combo,  U_GRV),
    [MU_U_TRM]    = COMBO(mu_u_trm_combo,    U_TRM),
    [XC_C_CED]    = COMBO(xc_c_ced_combo,    C_CED),
};
