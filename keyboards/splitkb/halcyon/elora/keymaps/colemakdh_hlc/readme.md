# Colemak-DH Keymap for Halcyon Elora

This keymap is based on [Vermoot's Kyria keymap](https://github.com/qmk/qmk_firmware/tree/master/keyboards/splitkb/kyria/keymaps/Vermoot), adapted for the Halcyon Elora with a full number row.

## Features

- **Home Row Mods**: Modifier keys on the home row (Shift, Ctrl, Alt, GUI)
- **Combos**: Extensive combo system for French accents, symbols, and common words
- **Caps Word**: Double-tap Shift or both shifts to activate caps word mode
- **Layer-tap keys**: Access layers by holding thumb keys
- **Encoder support**: Volume control on base layers

## Layer Overview

| Layer     | Purpose                                                                  |
| --------- | ------------------------------------------------------------------------ |
| `_BASE`   | Colemak-DH with home row mods                                            |
| `_QWERTY` | QWERTY layout with home row mods                                         |
| `_GAMING` | Gaming layout (no combos, standard keys)                                 |
| `_NAV`    | Navigation: F-keys (color-coded per row), arrows, Home/End, Page Up/Down |
| `_EDIT`   | Editing, terminal line navigation, media, app prefixes                   |
| `_ADJUST` | Layer switches, RGB, brightness                                          |

---

## Base Layer: Colemak-DH

```
,-------------------------------------------.                              ,-------------------------------------------.
|  Esc   |   1  |   2  |   3  |   4  |   5  |                              |   6  |   7  |   8  |   9  |   0  |   `    |
|--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
| Tab/Meh|   Q  |   W  |   F  |   P  |   B  |                              |   J  |   L  |   U  |   Y  |   ;  | Media  |
|--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
|Esc/GUI |A/Shft|R/GUI |S/Alt |T/Ctrl|   G  |                              |   M  |N/Ctrl|E/Alt |I/GUI |O/Shft|  '     |
|--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
| Shift  |   Z  |   X  |   C  | D/Num|   V  |      |      |  |      |      |   K  |   H  |   ,  |   .  |   -  | Shift  |
`----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
                       | Ctrl | Sym  | Num  | Nav  |OS/Shf|  |OS/Shf| Space|Bspc  | Funct| Alt  |
                       |      |      |      |      |      |  |      |      |      |      |      |
                       `----------------------------------'  `----------------------------------'
,-----------------------------------.                                              ,-----------------------------------.
|      |      |       |      |      |                                              | Vol- | Vol+ |       |      |      |
`-----------------------------------'                                              `-----------------------------------'
```

- **HRM (Home Row Mods)**: Hold A/O for Shift, R/I for GUI, S/E for Alt, T/N for Ctrl
- **Thumb cluster**: Ctrl | Symbols | Nums | Nav | One-Shot Shift (left) / One-Shot Shift | Space | Backspace | Function | Alt (right)
- **D/Num**: Hold D for NUMS layer, tap for D
- **Tab/Meh**: Tap for Tab, hold for Meh (Ctrl+Alt+Shift)
- **Esc/GUI**: Tap for Esc, hold for GUI

---

## QWERTY Layer

Standard QWERTY layout with the same home row mods and thumb cluster as the base layer.

```
,-------------------------------------------.                              ,-------------------------------------------.
|  Esc   |   1  |   2  |   3  |   4  |   5  |                              |   6  |   7  |   8  |   9  |   0  |   `    |
|--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
| Tab/Meh|   Q  |   W  |   E  |   R  |   T  |                              |   Y  |   U  |   I  |   O  |   P  | Media  |
|--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
|Esc/GUI |A/Shft|S/GUI |D/Alt |F/Ctrl|   G  |                              |   H  |J/Ctrl|K/Alt |L/GUI |;/Shft|  '     |
|--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
| Shift  |   Z  |   X  |   C  |   V  |   B  |      |      |  |      |      |   N  |   M  |   ,  |   .  |   /  | Shift  |
`----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
                       | Ctrl | Sym  | Num  | Nav  |OS/Shf|  |OS/Shf| Space|Bspc  | Funct| Alt  |
                       `----------------------------------'  `----------------------------------'
```

---

## Gaming Layer

Standard gaming layout without home row mods (so keys register immediately). Combos are disabled on this layer.

```
,-------------------------------------------.                              ,-------------------------------------------.
|  Esc   |   1  |   2  |   3  |   4  |   5  |                              |   6  |   7  |   8  |   9  |   0  |   `    |
|--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
|  Tab   |   Q  |   W  |   E  |   R  |   T  |                              |   Y  |   U  |   I  |   O  |   P  |  Enter |
|--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
|  Ctrl  |   A  |   S  |   D  |   F  |   G  |                              |   H  |   J  |   K  |   L  |   ;  |   '    |
|--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
| Shift  |   Z  |   X  |   C  |   V  |   B  | Ctrl | Alt  |  | AltGr| Ctrl |   N  |   M  |   ,  |   .  |   /  | Shift  |
`----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
                       | GUI  | Num  | Nav  | Space|      |  | Sym  | Space|Bspc  |      | BASE |
                       `----------------------------------'  `----------------------------------'
```

- Switch back to BASE layer with the BASE key on the bottom right

---

## Navigation Layer

```
,-------------------------------------------.                              ,-------------------------------------------.
|  F1    |  F2  |  F3  |  F4  |  F5  |  F6  |                              |  F7  |  F8  |  F9  |  F10 |  F11 |  F12   |
|--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
|        |      |      |      |      |      |                              | Home |PgDown| PgUp | End  |      |        |
|--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
|        | Shift| GUI  | Alt  | Ctrl |      |                              | Left | Down | Up   | Right|      |        |
|--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
|        |      |      |      |      |      |      |      |  |      |      |      |      |      |      | Del  |        |
`----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
                       |      |      |      |      |      |  |      |      |      |      |      |
                       `----------------------------------'  `----------------------------------'
```

---

## Edit Layer (hold right thumb)

Editing cluster (left hand), terminal line navigation, media (right hand) and app prefixes (bottom right).

```
,-------------------------------------------.                              ,-------------------------------------------.
|        |      |      |      |      |      |                              |        |      |      |      |      |        |
|--------+------+------+------+------+------|                              |--------+------+------+------+------+--------|
|        |      |      | Find |      |      |                              |        | Vol↓ |      | Vol↑ |      |        |
|--------+------+------+------+------+------|                              |--------+------+------+------+------+--------|
|        | SelA | Redo | CtrlA| CtrlE|      |                              |  Tmux  | Prev | Play | Next | Mute | GEN3  |
|--------+------+------+------+------+------+-------------.  ,-------------+--------+------+------+------+------+--------|
|        | Undo | Cut  | Copy | Paste|      |      |      |  |      |      |   Del  |  Pal | DevT | GEN1 | GEN2 |        |
`----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
                       |      |      |      |      |      |  |      |      |      |      |      |
                       `----------------------------------'  `----------------------------------'
```

- **Editing**: Undo/Cut/Copy/Paste/Redo/Select-All/Find (QWERTY-position based)
- **Terminal line nav**: Ctrl+A / Ctrl+E (beginning/end of line)
- **Media**: Play/Prev/Next/Vol↑/Vol↓/Mute
- **App commands**: Palette (Ctrl+Shift+P), DevTools (Ctrl+Shift+I)
- **App prefixes**: Tmux (Ctrl+B), GEN1/GEN2/GEN3 (Ctrl+Alt+Shift+] / [ / Tab) — bind these in any app. Tap the prefix, release the thumb, then hit the command key.

---

```
,-------------------------------------------.                              ,-------------------------------------------.
|  F1    |  F2  |  F3  |  F4  |  F5  |  F6  |                              |  F7  |  F8  |  F9  |  F10 |  F11 |  F12   |
|--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
|   `    |   1  |   2  |   3  |   4  |   5  |                              |   6  |   7  |   8  |   9  |   0  |        |
|--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
|        |   !  |   @  |   #  |   $  |   %  |                              |   ^  |   &  |   *  |   (  |   )  |        |
|--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
|        |      |   {  |   [  |   (  |   _  |      |      |  |      |      |   -  |   )  |   ]  |   }  |      |        |
`----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
                       |      |      |      |      |      |  |      |      |      |      |      |
                       `----------------------------------'  `----------------------------------'
```

---

## Numpad Layer

```
,-------------------------------------------.                              ,-------------------------------------------.
|        |      |      |      |      |      |                              |      |      |      |      |      |        |
|--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
|        |      |      |      |      |      |                              |   /  |   7  |   8  |   9  |   -  |  Num   |
|--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
|        | Shift| GUI  | Alt  | Ctrl |      |                              |   *  |   4  |   5  |   6  |   +  |   '    |
|--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
|        |      |      |      |      |      |      |      |  |      |      |   =  |   1  |   2  |   3  | Enter|        |
`----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
                       |      |      |      |      |      |  |      |   0  | Bspc |   .  |      |
                       `----------------------------------'  `----------------------------------'
```

---

## Combos

French accent combos, all gated behind **F15** (hold F15 + chord; the F15
indicator lights the participating keys in their role colors on the RGB).

Uniform dead-key model: every chord is `[accent trigger] + [letter]`.

| Accent        | Trigger       | Combos                                                                        |
| ------------- | ------------- | ----------------------------------------------------------------------------- |
| grave `` ` `` | `L` (orange)  | `L`+`E` → `è` · `L`+`A` → `à` · `L`+`U` → `ù`                                 |
| acute ´       | `N` (gold)    | `N`+`E` → `é`                                                                 |
| circumflex ^  | `H` (cyan)    | `H`+`E` → `ê` · `H`+`A` → `â` · `H`+`I` → `î` · `H`+`O` → `ô` · `H`+`U` → `û` |
| trema ¨       | `M` (magenta) | `M`+`E` → `ë` · `M`+`I` → `ï` · `M`+`U` → `ü`                                 |
| cedilla ¸     | `,` (green)   | `,`+`C` → `ç`                                                                 |

---

## Encoder

The Elora has 4 encoders. On the base layer:

- **Top left**: Volume Up/Down
- **Top right**: Volume Up/Down
- **Bottom left**: Page Up/Down
- **Bottom right**: Page Up/Down

---

## Credits

- Original keymap by [Vermoot](https://github.com/Vermoot)
- Adapted for Halcyon Elora with full number row
- Uses the [QMK Firmware](https://qmk.fm/)
