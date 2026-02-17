# Link Time Optimization. Can increase build time but reduce binary size.
LTO_ENABLE = yes
VPATH += keyboards/gboards/

# Enable Combo feature, which allows to combo multiple keys together to create a new key
# Note: Vermoot's keymap uses COMBO_ENABLE = yes with extensive combos for French accents and symbols
COMBO_ENABLE = no

# Enable NKRO feature, which allows the keyboard to report any number of keys pressed simultaneously
NKRO_ENABLE = yes

# Enable Mousekey feature, which allows to move the cursor with keyboard keys
MOUSEKEY_ENABLE = no

# Enable Caps Word feature, which allows to capitalize words until the next word
CAPS_WORD_ENABLE = yes

# Enable RGBLIGHT feature, which allows to control the RGB LEDs on the keyboard
RGBLIGHT_ENABLE = no

ENCODER_MAP_ENABLE = no

# This adds module functionality to your keyboard (files found in users/halcyon_modules)
USER_NAME := halcyon_modules

# Additional features used by Vermoot's keymap (can be enabled if needed):
# SWAP_HANDS_ENABLE = yes  # Allows mirroring the keyboard for one-handed typing
# STENO_ENABLE = yes       # For Plover steno support (requires steno protocol)
