#include "nph60.h"
#include "action_layer.h"
#include <util/delay.h>
#ifdef PS2_MOUSE_ENABLE
#include "ps2_mouse.h"
#include "ps2.h"
#endif
#include "yk.h"

// For improved Space Cadet mods
static bool shift_interrupted[2] = {0, 0};
static bool ctrl_interrupted[2] = {0, 0};
static bool alt_interrupted[2] = {0, 0};
static uint16_t scs_timer[2] = {0, 0};
static uint16_t scc_timer[2] = {0, 0};
static uint16_t sca_timer[2] = {0, 0};

/* true if the last press of these keys was modded (i.e. RAlt, LShift... were pressed), false otherwise.
 * Used to ensure that the correct keycode is released if the key is released.
 */
bool cm_esc_was_ralted = false;
bool kc_p7_was_lshifted = false;
bool kc_ppls_was_lshifted = false;
bool kc_pmns_was_lshifted = false;

enum nph60_keycodes {
  CM_ESC = SAFE_RANGE, // custom Esc
  CM_LSPO, // custom Space Cadet Left Shift, Parenthesis Open on tap
  CM_RSPC, // custom Space Cadet Right Shift, Parenthesis Close on tap
  CM_LCSO, // custom Left Ctrl, Square bracket Open on tap
  CM_RCSC, // custom Right Ctrl, Square bracket Close on tap
  CM_LACO, // custom Left Alt, Curly bracket Open on tap
  CM_RACC, // custom Right Alt, Curly bracket Close on tap
  CM_WAIT, // custom wait
  CM_CSTR, // custom send_string action
  DYNAMIC_MACRO_RANGE,
};
#include "dynamic_macro.h"

// makes layer arrays easier to read
#define _______ KC_TRNS

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    /* 0: Default layer
     * ,-----------------------------------------------------------.
     * |CEs| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 0 | ' | ¡ | Backsp|
     * |-----------------------------------------------------------|
     * | Tab | q | w | e | r | t | y | u | i | o | p | ` | + |Enter|
     * |------------------------------------------------------.    |
     * |CapCtl| a | s | d | f | g | h | j | k | l | ñ | ' |fn2|    |
     * |--------------------------O--------------------------------|
     * |LSPO|fn1| z | x | c | v | b | n | m | , | . | - |   RSPC   |
     * |-----------------------------------------------------------|
     * |LCSO|LGui|LACO|m1 |Space|m3 | fn0 | m2 |RACC|RGui|Menu|RCSC|
     * `--------------'   '-----'   '-----'    '-------------------'
     * fn0: hold for Function layer. Tap 5 times to toggle
     * fn1: tap for "<" (NUBS), hold for Media layer
     * fn2: tap for "ç" (NUHS), hold for Media layer
     * CapCtl: tap for CapsLock, hold for RCtrl
     * LSPO/RSPC: custom Space Cadet Shift
     * LCSO/RCSC: Like above but for LCtrl/RCtrl
     * LACO/RACC: Like above but for LAlt/RAlt
     * CEs: Custom keycode for Esc <-------------------------------(most used)
     *                         with RAlt: GRAVE ("\") (RAlt+GRV)       ...
     *                         with fn0:  GRAVE ("º","ª") (GRV)        ...
     *                         with fn0+RAlt: "RAlt+Esc" <---------(least used)
     */
    [LY_BASE] = KEYMAP(
        CM_ESC              ,KC_1               ,KC_2   ,KC_3   ,KC_4       ,KC_5   ,KC_6   ,KC_7   ,KC_8   ,KC_9   ,KC_0   ,KC_MINS,KC_EQL             ,KC_BSPC, \
        KC_TAB              ,KC_Q               ,KC_W   ,KC_E   ,KC_R       ,KC_T   ,KC_Y   ,KC_U   ,KC_I   ,KC_O   ,KC_P   ,KC_LBRC,KC_RBRC            ,KC_ENT , \
        MT(MOD_LCTL,KC_CAPS),KC_A               ,KC_S   ,KC_D   ,KC_F       ,KC_G   ,KC_H   ,KC_J   ,KC_K   ,KC_L   ,KC_SCLN,KC_QUOT,LT(LY_MREC,KC_NUHS),         \
        CM_LSPO             ,LT(LY_MREC,KC_NUBS),KC_Z   ,KC_X   ,KC_C       ,KC_V   ,KC_B   ,KC_N   ,KC_M   ,KC_COMM,KC_DOT ,KC_SLSH,CM_RSPC            ,         \
        CM_LCSO             ,KC_LGUI            ,CM_LACO,KC_SPC ,TT(LY_FUNC),CM_RACC,KC_RGUI,KC_APP ,CM_RCSC                                                      ),

    /* 1: Normal mods layer
     * ,-----------------------------------------------------------.
     * |   |   |   |   |   |   |   |   |   |   |   |   |   |       |
     * |-----------------------------------------------------------|
     * |     |   |   |   |   |   |   |   |   |   |   |   |   |     |
     * |------------------------------------------------------.    |
     * |CapsLk|   |   |   |   |   |   |   |   |   |   |   | ç |    |
     * |--------------------------O--------------------------------|
     * |LSft| < |   |   |   |   |   |   |   |   |   |   |  RShift  |
     * |-----------------------------------------------------------|
     * |LCtl|    |LAlt|   |     |   |     |    |RAlt|    |    |RCtl|
     * `--------------'   '-----'   '-----'    '-------------------'
     */
    [LY_NMOD] = KEYMAP(
        _______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______, \
        _______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______, \
        KC_CAPS,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,KC_NUHS,         \
        KC_LSFT,KC_NUBS,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,KC_RSFT,         \
        KC_LCTL,_______,KC_LALT,_______,_______,KC_RALT,_______,_______,KC_RCTL                                          ),

    /* 2: Numpad layer
     * ,-----------------------------------------------------------.
     * |   |   |   |   |   |   |   |Np7|Np8|Np9|   |   |   |       |
     * |-----------------------------------------------------------|
     * |     |   |   |   |   |   |   |Np4|Np5|Np6|   |   |Np+|Enter|
     * |------------------------------------------------------. Np |
     * |NumLck|   |   |   |   |   |   |Np1|Np2|Np3|   |   |   |    |
     * |--------------------------O--------------------------------|
     * |    |   |   |   |   |   |   |Np0|Np0|   |   |Np-|          |
     * |-----------------------------------------------------------|
     * |    |    |    |   |     |   |     |    |    |    |    |    |
     * `--------------'   '-----'   '-----'    '-------------------'
     */
    [LY_NPAD] = KEYMAP(
        _______,_______,_______,_______,_______,_______,_______,KC_P7  ,KC_P8  ,KC_P9  ,_______,_______,_______,_______, \
        _______,_______,_______,_______,_______,_______,_______,KC_P4  ,KC_P5  ,KC_P6  ,_______,_______,KC_PPLS,KC_PENT, \
        KC_NLCK,_______,_______,_______,_______,_______,_______,KC_P1  ,KC_P2  ,KC_P3  ,_______,_______,_______,         \
        _______,_______,_______,_______,_______,_______,_______,KC_P0  ,KC_P0  ,_______,_______,KC_PMNS,_______,         \
        _______,_______,_______,_______,_______,_______,_______,_______,_______                                          ),

    /* 3: Function layer
     * ,-----------------------------------------------------------.
     * | º | F1| F2| F3| F4| F5| F6| F7| F8| F9|F10|F11|F12| music |
     * |-----------------------------------------------------------|
     * |     |   |   |   |   |   |   |PUp|Up |PDn|Prt|SLk|Ins| cstr|
     * |------------------------------------------------------.    |
     * |      |   |   |   |   |   |Hom|Lft|Dwn|Rgt|End|   |Pau|    |
     * |--------------------------O--------------------------------|
     * |    |   |   |   |   |   |   |fn4|PDn|   |   |Del|          |
     * |-----------------------------------------------------------|
     * |    |    |    |   |     |   |     |    |    |    |    |fn3 |
     * `--------------'   '-----'   '-----'    '-------------------'
     * fn3: toggle Normal Mods layer
     * fn4: toggle Numpad layer
     * music: toggle Music mode
     * cstr: send a custom string
     */
    [LY_FUNC] = KEYMAP(
        KC_GRV ,KC_F1  ,KC_F2  ,KC_F3  ,KC_F4  ,KC_F5  ,KC_F6  ,KC_F7      ,KC_F8  ,KC_F9  ,KC_F10 ,KC_F11 ,KC_F12 ,MU_TOG , \
        _______,_______,_______,_______,_______,_______,_______,KC_PGUP    ,KC_UP  ,KC_PGDN,KC_PSCR,KC_SLCK,KC_INS ,CM_CSTR, \
        _______,_______,_______,_______,_______,_______,KC_HOME,KC_LEFT    ,KC_DOWN,KC_RGHT,KC_END ,_______,KC_PAUS,         \
        _______,_______,_______,_______,_______,_______,_______,TG(LY_NPAD),KC_PGDN,_______,_______,KC_DEL ,_______,         \
        _______,_______,_______,_______,_______,_______,_______,_______    ,TG(LY_NMOD)                                      ),

    /* 4: Macro and Media layer
     * ,-----------------------------------------------------------.
     * |srm|rm1|rm2|wait|  |   |   |   |   |   |   |   |   |  pm2  |
     * |-----------------------------------------------------------|
     * |    |Mute|Vl-|Vl+|F20|   |   |   |   |   |   |   |   | pm1 |
     * |------------------------------------------------------.    |
     * |      |   |Prv|Stp|Ply|Nxt|   |   |   |   |   |   |   |    |
     * |--------------------------O--------------------------------|
     * |    |   |   |   |   |   |   |   |   |   |   |   |          |
     * |-----------------------------------------------------------|
     * |    |    |    |   |     |   |     |    |    |    |    |    |
     * `--------------'   '-----'   '-----'    '-------------------'
     * srm: stop recording macros
     * rm1: start recording macro 1
     * rm2: start recording macro 2
     * wait: sleep for TAPPING_TERM (to add delays when recording)
     * pm1: play Macro 1
     * pm2: play Macro 2
     */
    [LY_MREC] = KEYMAP(
        DYN_REC_STOP,DYN_REC_START1,DYN_REC_START2,CM_WAIT,_______,_______,_______,_______,_______,_______,_______,_______,_______,DYN_MACRO_PLAY2, \
        _______     ,KC_MUTE       ,KC_VOLD       ,KC_VOLU,KC_F20 ,_______,_______,_______,_______,_______,_______,_______,_______,DYN_MACRO_PLAY1, \
        _______     ,_______       ,KC_MPRV       ,KC_MSTP,KC_MPLY,KC_MNXT,_______,_______,_______,_______,_______,_______,_______,                 \
        _______     ,_______       ,_______       ,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,                 \
        _______     ,_______       ,_______       ,_______,_______,_______,_______,_______,_______                                                  ),

};

// Aux function, sends mod_hold on hold, mod_tap+key_tap on tap
void custom_space_cadet_mods(bool pressed, uint16_t *timer, bool *interrupted, bool *other_interrupted, uint16_t mod_hold, uint16_t other_mod_hold, uint16_t key_tap, uint16_t mod_tap) {
    if (pressed) {
        *interrupted = false;
        *timer = timer_read();
        register_mods(MOD_BIT(mod_hold));
    } else {
        if (get_mods() & MOD_BIT(other_mod_hold)) {
            *interrupted = true;
            *other_interrupted = true;
        }
        unregister_mods(MOD_BIT(mod_hold));
        if (!*interrupted && timer_elapsed(*timer) < TAPPING_TERM) {
            register_mods(MOD_BIT(mod_tap));
            register_code(key_tap);
            unregister_code(key_tap);
            unregister_mods(MOD_BIT(mod_tap));
        }
    }
}

// Aux function, sends alternate key if mod key is held, otherwise sends default key
// Returns: a bool to keep the state for key release
bool send_alternate_key_if_modded(bool key_was_modded, uint16_t default_key, uint16_t mod_key, uint16_t alternate_key, bool eventPressed, bool unapply_mod) {
    if (eventPressed) {
        key_was_modded = get_mods() & MOD_BIT(mod_key);
        if (key_was_modded & unapply_mod) { unregister_mods(MOD_BIT(mod_key)); }
        register_code(key_was_modded ? alternate_key : default_key);
        if (key_was_modded & unapply_mod) { register_mods(MOD_BIT(mod_key)); }
    } else {
        unregister_code(key_was_modded ? alternate_key : default_key);
    }
    return key_was_modded;
}

#ifdef PS2_MOUSE_ENABLE
void ps2_mouse_init_user(void) {

    // set TrackPoint sensitivity
    PS2_MOUSE_SEND(0xE2, "tpsens: 0xE2");
    PS2_MOUSE_SEND(0x81, "tpsens: 0x81");
    PS2_MOUSE_SEND(0x4A, "tpsens: 0x4A");
    PS2_MOUSE_SEND(0x7F, "tpsens: 0x7F"); // default is 0x80

    // set TrackPoint speed
    // (transfer function upper plateau speed)
    PS2_MOUSE_SEND(0xE2, "tpspd: 0xE2");
    PS2_MOUSE_SEND(0x81, "tpspd: 0x81");
    PS2_MOUSE_SEND(0x60, "tpspd: 0x60");
    PS2_MOUSE_SEND(0x81, "tpspd: 0x81"); // default is 0x61

    // set TrackPoint Negative Inertia factor
    PS2_MOUSE_SEND(0xE2, "tpnegin: 0xE2");
    PS2_MOUSE_SEND(0x81, "tpnegin: 0x81");
    PS2_MOUSE_SEND(0x4D, "tpnegin: 0x4D");
    PS2_MOUSE_SEND(0x03, "tpnegin: 0x03"); // default is 0x06

    // enable TrackPoint Press to Select (PtS)
    // inquire PtS status
    PS2_MOUSE_SEND(0xE2, "tppts: 0xE2");
    PS2_MOUSE_SEND(0x2C, "tppts: 0x2C");
    uint8_t rcv = ps2_host_recv_response();
    if ((rcv & 1) == 0) {
        // if PtS is off, enable it
        PS2_MOUSE_SEND(0xE2, "tppts: 0xE2");
        PS2_MOUSE_SEND(0x47, "tppts: 0x47");
        PS2_MOUSE_SEND(0x2C, "tppts: 0x2C");
        PS2_MOUSE_SEND(0x01, "tppts: 0x01"); // toggle the bit
    }
    // set TrackPoint Press to Select threshold
    PS2_MOUSE_SEND(0xE2, "tpthrs: 0xE2");
    PS2_MOUSE_SEND(0x81, "tpthrs: 0x81");
    PS2_MOUSE_SEND(0x5C, "tpthrs: 0x5C");
    PS2_MOUSE_SEND(0x04, "tpthrs: 0x04"); // default is 0x08
    // set TrackPoint Press to Select time constant (zTc)
    PS2_MOUSE_SEND(0xE2, "tpztc: 0xE2");
    PS2_MOUSE_SEND(0x81, "tpztc: 0x81");
    PS2_MOUSE_SEND(0x5E, "tpztc: 0x5E");
    PS2_MOUSE_SEND(0x50, "tpztc: 0x50"); // default is 0x26
  /*
    // set TrackPoint Press to Select Jenks Curvature (jkcur)
    PS2_MOUSE_SEND(0xE2, "tpjkcur: 0xE2");
    PS2_MOUSE_SEND(0x81, "tpjkcur: 0x81");
    PS2_MOUSE_SEND(0x5D, "tpjkcur: 0x5D");
    PS2_MOUSE_SEND(0x87, "tpjkcur: 0x87"); // default is 0x87
  */
  /*
    // set TrackPoint Minimum Drag (mindrag)
    PS2_MOUSE_SEND(0xE2, "tpmdrag: 0xE2");
    PS2_MOUSE_SEND(0x81, "tpmdrag: 0x81");
    PS2_MOUSE_SEND(0x59, "tpmdrag: 0x59");
    PS2_MOUSE_SEND(0x14, "tpmdrag: 0x14"); // default is 0x14
  */
}
#endif

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    // process dynamic macros
    if (!process_record_dynamic_macro(keycode, record)) {
        return false;
    }

    // on keydown, if Normal Mods layer is not active,
    if ( record->event.pressed
      && !(layer_state & (1<<LY_NMOD)) ) {
        // interrupt any Space Cadet Mods as appropiate
        switch (keycode) {
            case CM_LSPO:
            case CM_RSPC:
                ctrl_interrupted[0] = ctrl_interrupted[1] = true;
                alt_interrupted[0] = alt_interrupted[1] = true;
                break;
            case CM_LCSO:
            case CM_RCSC:
                shift_interrupted[0] = shift_interrupted[1] = true;
                alt_interrupted[0] = alt_interrupted[1] = true;
                break;
            case CM_LACO:
            case CM_RACC:
                shift_interrupted[0] = shift_interrupted[1] = true;
                ctrl_interrupted[0] = ctrl_interrupted[1] = true;
                break;
            default :
                shift_interrupted[0] = shift_interrupted[1] = true;
                ctrl_interrupted[0] = ctrl_interrupted[1] = true;
                alt_interrupted[0] = alt_interrupted[1] = true;
                break;
        }
    }

    switch (keycode) {

        // Improved Grave Escape
        case CM_ESC:
            // This makes it easier to type a backslash (simply RAlt+Esc).
            cm_esc_was_ralted = send_alternate_key_if_modded(
                cm_esc_was_ralted, KC_ESC, KC_RALT, KC_GRV, record->event.pressed, false);
            return false;

        // Numpad tweaks
        case KC_P7:
            // Send Numpad Slash with Left Shift + Numpad 7
            kc_p7_was_lshifted = send_alternate_key_if_modded(
                kc_p7_was_lshifted, KC_P7, KC_LSFT, KC_PSLS, record->event.pressed, true);
            return false;

        case KC_PPLS:
            // Send Numpad Asterisk with Left Shift + Numpad Plus
            kc_ppls_was_lshifted = send_alternate_key_if_modded(
                kc_ppls_was_lshifted, KC_PPLS, KC_LSFT, KC_PAST, record->event.pressed, true);
            return false;

        case KC_PMNS:
            // Send Underscore with Left Shift + Numpad Minus
            kc_pmns_was_lshifted = send_alternate_key_if_modded(
                kc_pmns_was_lshifted, KC_PMNS, KC_LSFT, KC_SLSH, record->event.pressed, false);
            return false;

        // Improved Space Cadet Shift
        case CM_LSPO:
            // sends Left Shift on hold, parenthesis open "(" on tap
            custom_space_cadet_mods(record->event.pressed,
                                    &(scs_timer[0]),
                                    &(shift_interrupted[0]),
                                    &(shift_interrupted[1]),
                                    KC_LSFT,
                                    KC_RSFT,
                                    KC_8,
                                    KC_LSFT);
            return false;

        case CM_RSPC:
            // sends Right Shift on hold, parenthesis close ")" on tap
            custom_space_cadet_mods(record->event.pressed,
                                    &(scs_timer[1]),
                                    &(shift_interrupted[1]),
                                    &(shift_interrupted[0]),
                                    KC_RSFT,
                                    KC_LSFT,
                                    KC_9,
                                    KC_LSFT);
            return false;

        // Like Space Cadet Shift, but with Ctrl and square brackets.
        case CM_LCSO:
            // sends Left Ctrl on hold, bracket open "[" on tap
            custom_space_cadet_mods(record->event.pressed,
                                    &(scc_timer[0]),
                                    &(ctrl_interrupted[0]),
                                    &(ctrl_interrupted[1]),
                                    KC_LCTL,
                                    KC_RCTL,
                                    KC_LBRC,
                                    KC_RALT);
            return false;

        case CM_RCSC:
            // sends Right Ctrl on hold, bracket close "]" on tap
            custom_space_cadet_mods(record->event.pressed,
                                    &(scc_timer[1]),
                                    &(ctrl_interrupted[1]),
                                    &(ctrl_interrupted[0]),
                                    KC_RCTL,
                                    KC_LCTL,
                                    KC_RBRC,
                                    KC_RALT);
            return false;

        // Like Space Cadet Shift, but with Alt and curly brackets.
        case CM_LACO:
            // sends Left Alt on hold, curly bracket open "{" on tap
            custom_space_cadet_mods(record->event.pressed,
                                    &(sca_timer[0]),
                                    &(alt_interrupted[0]),
                                    &(alt_interrupted[1]),
                                    KC_LALT,
                                    KC_RALT,
                                    KC_QUOT,
                                    KC_RALT);
            return false;

        case CM_RACC:
            // sends AltGr on hold, curly bracket close "}" on tap
            custom_space_cadet_mods(record->event.pressed,
                                    &(sca_timer[1]),
                                    &(alt_interrupted[1]),
                                    &(alt_interrupted[0]),
                                    KC_RALT,
                                    KC_LALT,
                                    KC_NUHS,
                                    KC_RALT);
            return false;

        // Wait a few ms (to be used when recording dynamic macros)
        case CM_WAIT:
            if (record->event.pressed) {
                _delay_ms(TAPPING_TERM);
            }
            return false;

        // Send a custom string
        case CM_CSTR:
#ifdef YKPW
            if (record->event.pressed) {
                SEND_STRING(YKPW);
            }
            return false;
#endif
            break;

        // None of the above
        default:
            break;
    }

    return true;
};
