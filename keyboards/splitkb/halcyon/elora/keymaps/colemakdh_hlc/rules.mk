# Link Time Optimization. Can increase build time but reduce binary size.
LTO_ENABLE = yes
VPATH += keyboards/gboards/

COMBO_ENABLE = yes
SRC += custom_functions.c

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

