#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <stdint.h>
#define KEY_UP 0x80
#define KEY_DOWN 0x81
#define KEY_RIGHT 0x83
#define KEY_LEFT 0x82

//char ps2_read_key(void);
uint8_t ps2_read_key(void);

#endif // KEYBOARD_H

