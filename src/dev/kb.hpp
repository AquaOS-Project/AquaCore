#pragma once

#define KBSTATP 0x64
#define KBS_DIB 0x01
#define KBDATAP 0x60

#define SHIFT (1 << 0)
#define CTL (1 << 1)
#define ALT (1 << 2)

enum KeyLocks {
    CAPSLOCK   = (1 << 3),
    NUMLOCK    = (1 << 4),
    SCROLLLOCK = (1 << 5),
}

#define E0ESC (1 << 6)

// Misc keycodes
enum MiscKeys {
    KEY_HOME =  0xE0,
    KEY_END  =  0xE1,
    KEY_UP   =  0xE2,
    KEY_DN   =  0xE3,
    KEY_LF   =  0xE4,
    KEY_RT   =  0xE5,
    KEY_PGUP =  0xE6,
    KEY_PGDN =  0xE7,
    KEY_INS  =  0xE8,
    KEY_DEL  =  0xE9,
}

#define C(x) (x - '@')
