#include <lib/port.hpp>
#include <lib/log.hpp>
#include <dev/ps2.hpp>

static bool canread() {
    return (inb(PS2_PORT_STATUS) & PS2_OUTPUT_FULL) != 0;
}

static bool canwrite() {
    return (inb(PS2_PORT_STATUS) & PS2_INPUT_FULL) == 0;
}

static uint8_t read() {
    for (size_t i = 0; i < PS2_MAX_TIMEOUT; i++) {
        if (canread()) return inb(PS2_PORT_DATA);
    }
    return 0;
}

static uint8_t read(bool &error) {
    for (size_t i = 0; i < PS2_MAX_TIMEOUT; i++) {
        if (canread()) {
            error = false;
            return inb(PS2_PORT_DATA);
        }
    }
    error = true;
    return 0;
}

static void write(uint16_t port, uint8_t value) {
    for (size_t i = 0; i < PS2_MAX_TIMEOUT; i++) {
        if (canwrite()) return outb(port, value);
    }
}

static void drainbuffer() {
    while (!canwrite()) inb(PS2_PORT_DATA);
}

static void disabledev(devices device) {
    switch (device) {
        case PS2_DEVICE_FIRST:
            write(PS2_PORT_COMMAND, PS2_DISABLE_FIRST);
            break;
        case PS2_DEVICE_SECOND:
            write(PS2_PORT_COMMAND, PS2_DISABLE_SECOND);
            break;
        default:
            error("[PS/2] Unknown device!");
            break;
    }
}
