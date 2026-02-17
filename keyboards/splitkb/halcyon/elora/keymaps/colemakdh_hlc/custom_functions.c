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

bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case HRM_A:
        case HRM_R:
        case HRM_O:
        case HRM_I:
            return true;
        default:
            return false;
    }
}
