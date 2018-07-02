/*
Copyright 2012 Jun Wako <wakojun@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CONFIG_H
#define CONFIG_H

#include "config_common.h"

/* USB Device descriptor parameter */
#include "usb_id.h"

/* key matrix size */
#define MATRIX_ROWS 5
#define MATRIX_COLS 14

/* Layer numbers */
#include "layernums.h"

/*
 * Keyboard Matrix Assignments
 *
 * Change this to how you wired your keyboard
 * COLS: AVR pins used for columns, left to right
 * ROWS: AVR pins used for rows, top to bottom
 * DIODE_DIRECTION: COL2ROW = COL = Anode (+), ROW = Cathode (-, marked on diode)
 *         ROW2COL = ROW = Anode (+), COL = Cathode (-, marked on diode)
 *
*/

#define MATRIX_ROW_PINS { F0, F1, F4, F5, F6 }
#define MATRIX_COL_PINS { F7, B6, B5, B4, D7, C7, D6, D3, D1, D0, B7, B3, B2, B1 }
#define UNUSED_PINS
#define C6_AUDIO
#define AUDIO_VOICES

/* COL2ROW or ROW2COL */
#define DIODE_DIRECTION COL2ROW

/* Debounce reduces chatter (unintended double-presses) - set 0 if debouncing is not needed */
#define DEBOUNCING_DELAY 5

/* define if matrix has ghost (lacks anti-ghosting diodes) */
//#define MATRIX_HAS_GHOST

/* number of backlight levels */
//#define BACKLIGHT_LEVELS 3

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
//#define LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
//#define LOCKING_RESYNC_ENABLE

/*
 * Force NKRO
 *
 * Force NKRO (nKey Rollover) to be enabled by default, regardless of the saved
 * state in the bootmagic EEPROM settings. (Note that NKRO must be enabled in the
 * makefile for this to work.)
 *
 * If forced on, NKRO can be disabled via magic key (default = LShift+RShift+N)
 * until the next keyboard reset.
 *
 * NKRO may prevent your keystrokes from being detected in the BIOS, but it is
 * fully operational during normal computer usage.
 *
 * For a less heavy-handed approach, enable NKRO via magic key (LShift+RShift+N)
 * or via bootmagic (hold SPACE+N while plugging in the keyboard). Once set by
 * bootmagic, NKRO mode will always be enabled until it is toggled again during a
 * power-up.
 *
 */
#define FORCE_NKRO

/*
 * Magic Key Options
 *
 * Magic keys are hotkey commands that allow control over firmware functions of
 * the keyboard. They are best used in combination with the HID Listen program,
 * found here: https://www.pjrc.com/teensy/hid_listen.html
 *
 * The options below allow the magic key functionality to be changed. This is
 * useful if your keyboard/keypad is missing keys and you want magic key support.
 *
 */

/* key combination for magic key command */
#define IS_COMMAND() ( \
  keyboard_report->mods == (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT)) \
)

/* period of tapping(ms) for Fn keys */
#define TAPPING_TERM    125
/* tap count needed for toggling a feature */
#define TAPPING_TOGGLE  5
/* send button event when button is released within this value(ms); set 0 to disable  */
#define PS2_MOUSE_SCROLL_BTN_SEND       250
/* divide vertical and horizontal mouse move by this to convert to scroll move */
#define PS2_MOUSE_SCROLL_DIVISOR_V      4
#define PS2_MOUSE_SCROLL_DIVISOR_H      4
/* enable mouse debug */
#define PS2_MOUSE_DEBUG_RAW
#define PS2_MOUSE_DEBUG_HID

/* control how magic key switches layers */
//#define MAGIC_KEY_SWITCH_LAYER_WITH_FKEYS true
//#define MAGIC_KEY_SWITCH_LAYER_WITH_NKEYS true
//#define MAGIC_KEY_SWITCH_LAYER_WITH_CUSTOM false

/* override magic key keymap */
//#define MAGIC_KEY_SWITCH_LAYER_WITH_FKEYS
//#define MAGIC_KEY_SWITCH_LAYER_WITH_NKEYS
//#define MAGIC_KEY_SWITCH_LAYER_WITH_CUSTOM
//#define MAGIC_KEY_HELP1     H
//#define MAGIC_KEY_HELP2     SLASH
//#define MAGIC_KEY_DEBUG     D
//#define MAGIC_KEY_DEBUG_MATRIX  X
//#define MAGIC_KEY_DEBUG_KBD   K
//#define MAGIC_KEY_DEBUG_MOUSE  M
//#define MAGIC_KEY_VERSION    V
//#define MAGIC_KEY_STATUS     S
//#define MAGIC_KEY_CONSOLE    C
//#define MAGIC_KEY_LAYER0_ALT1  ESC
//#define MAGIC_KEY_LAYER0_ALT2  GRAVE
//#define MAGIC_KEY_LAYER0     0
//#define MAGIC_KEY_LAYER1     1
//#define MAGIC_KEY_LAYER2     2
//#define MAGIC_KEY_LAYER3     3
//#define MAGIC_KEY_LAYER4     4
//#define MAGIC_KEY_LAYER5     5
//#define MAGIC_KEY_LAYER6     6
//#define MAGIC_KEY_LAYER7     7
//#define MAGIC_KEY_LAYER8     8
//#define MAGIC_KEY_LAYER9     9
//#define MAGIC_KEY_BOOTLOADER   PAUSE
//#define MAGIC_KEY_LOCK      CAPS
//#define MAGIC_KEY_EEPROM     E
//#define MAGIC_KEY_NKRO      N
//#define MAGIC_KEY_SLEEP_LED   Z

/*
 * Feature disable options
 * These options are also useful to firmware size reduction.
 */

/* disable action features */
//#define NO_ACTION_LAYER
//#define NO_ACTION_TAPPING
#define NO_ACTION_ONESHOT
#define NO_ACTION_MACRO
#define NO_ACTION_FUNCTION

/*
 * PS2 mouse features, required for TrackPoint
 */

/* PS/2 mouse USART version */
#ifdef PS2_USE_USART
#if defined(__AVR_ATmega16U4__) || defined(__AVR_ATmega32U4__)
/* XCK for clock line and RXD for data line */
#define PS2_CLOCK_PORT  PORTD
#define PS2_CLOCK_PIN   PIND
#define PS2_CLOCK_DDR   DDRD
#define PS2_CLOCK_BIT   5
#define PS2_DATA_PORT   PORTD
#define PS2_DATA_PIN    PIND
#define PS2_DATA_DDR    DDRD
#define PS2_DATA_BIT    2
/* synchronous, odd parity, 1-bit stop, 8-bit data, sample at falling edge */
/* set DDR of CLOCK as input to be slave */
#define PS2_USART_INIT() do {   \
    PS2_CLOCK_DDR &= ~(1<<PS2_CLOCK_BIT);   \
    PS2_DATA_DDR &= ~(1<<PS2_DATA_BIT);     \
    UCSR1C = ((1 << UMSEL10) |  \
              (3 << UPM10)   |  \
              (0 << USBS1)   |  \
              (3 << UCSZ10)  |  \
              (0 << UCPOL1));   \
    UCSR1A = 0;                 \
    UBRR1H = 0;                 \
    UBRR1L = 0;                 \
} while (0)
#define PS2_USART_RX_INT_ON() do {  \
    UCSR1B = ((1 << RXCIE1) |       \
              (1 << RXEN1));        \
} while (0)
#define PS2_USART_RX_POLL_ON() do { \
    UCSR1B = (1 << RXEN1);          \
} while (0)
#define PS2_USART_OFF() do {    \
    UCSR1C = 0;                 \
    UCSR1B &= ~((1 << RXEN1) |  \
                (1 << TXEN1));  \
} while (0)
#define PS2_USART_RX_READY      (UCSR1A & (1<<RXC1))
#define PS2_USART_RX_DATA       UDR1
#define PS2_USART_ERROR         (UCSR1A & ((1<<FE1) | (1<<DOR1) | (1<<UPE1)))
#define PS2_USART_RX_VECT       USART1_RX_vect

// Enable switching the axis of TrackPoint middle scrolling by Fn + middle button
// LY_FUNC must be the Fn layer number, defined in layernums.h
// Also used to avoid interference between the Fn layer and music playback
#ifndef NO_ACTION_LAYER
#ifdef LY_FUNC
#define FUNCTION_LAYER_NUMBER LY_FUNC
#endif
#endif

#endif
#endif

#endif

