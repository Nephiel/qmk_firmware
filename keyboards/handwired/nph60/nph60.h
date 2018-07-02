#ifndef NPH60_H
#define NPH60_H

#include "quantum.h"
#include "led.h"
#include "layernums.h"

/* NPH60 LEDs
 *   E6 Esc key LED
 *   D4 Capslock LED
 *   B0 N key LED
 */

/* Other
 *   C6 Piezo speaker
 *   D6 built-in Teensy LED, removed to use the pin for matrix column 6
 */

inline void led_escKey(bool enable) {
    if (enable) {
        // output low
        DDRE |= (1<<6);
        PORTE &= ~(1<<6);
    } else {
        // Hi-Z
        DDRE &= ~(1<<6);
        PORTE &= ~(1<<6);
    }
}

inline void led_escKeyToggle(void) {
    if (DDRE & (1<<6)) {
        led_escKey(false);
    } else {
        led_escKey(true);
    }
}

inline void led_capsLockKey(bool enable) {
    if (enable) {
        // output low
        DDRD |= (1<<4);
        PORTD &= ~(1<<4);
    } else {
        // Hi-Z
        DDRD &= ~(1<<4);
        PORTD &= ~(1<<4);
    }
}

inline void led_nKey(bool enable)
{
    if (enable) {
        // output low
        DDRB |= (1<<0);
        PORTB &= ~(1<<0);
    } else {
        // Hi-Z
        DDRB &= ~(1<<0);
        PORTB &= ~(1<<0);
    }
}

/*
 * ISO keymap definition macro
 */
#define KEYMAP( \
    K00, K01, K02, K03, K04, K05, K06, K07, K08, K09, K0A, K0B, K0C,  K0D, \
     K10, K11, K12, K13, K14, K15, K16, K17, K18, K19, K1A, K1B, K1C, K1D, \
     K20,  K21, K22, K23, K24, K25, K26, K27, K28, K29, K2A, K2B, K2C,     \
     K30, K31, K32, K33, K34, K35, K36, K37, K38, K39, K3A, K3B,    K3C,   \
     K40, K41, K42,          K43,        K44,          K45, K46, K47, K48  \
) { \
    { K00, K01, K02, K03, K04, K05, K06, K07, K08, K09, K0A, K0B, K0C, K0D },                          \
    { K10, K11, K12, K13, K14, K15, K16, K17, K18, K19, K1A, K1B, K1C, K1D },                          \
    { K20, K21, K22, K23, K24, K25, K26, K27, K28, K29, K2A, K2B, K2C, KC_NO    },                     \
    { K30, K31, K32, K33, K34, K35, K36, K37, K38, K39, K3A, K3B, K3C, KC_NO    },                     \
    { K40, K41, KC_NO,    K42, KC_NO,    K43, KC_NO,    K44, KC_NO,    KC_NO,    K45, K46, K47, K48 }  \
}
#endif
