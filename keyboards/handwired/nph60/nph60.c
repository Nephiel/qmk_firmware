#include "nph60.h"
#include "action_layer.h"

void led_set_kb(uint8_t usb_led) {

    // put your keyboard LED indicator (ex: Caps Lock LED) toggling code here

    // N key LED, used as Function layer indicator
    led_nKey(layer_state & (1<<LY_FUNC));    // turn LED on if layer LY_FUNC is active
    //led_nKey(biton32(layer_state) == LY_FUNC); // turn LED on if layer LY_FUNC is the highest active layer

    // Esc key LED, used as Numpad layer indicator
    led_escKey(layer_state & (1<<LY_NPAD));

    // Caps Lock key LED
    if (layer_state & (1<<LY_NPAD)) {
        // Numpad layer active
        // used as Num Lock indicator
        led_capsLockKey(usb_led & (1<<USB_LED_NUM_LOCK));
    } else {
        // Numpad layer not active
        // used as Caps Lock indicator
        led_capsLockKey(usb_led & (1<<USB_LED_CAPS_LOCK));
    }

    // Do not forget to call the user function
    led_set_user(usb_led);
}
