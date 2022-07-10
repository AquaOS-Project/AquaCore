#include <common/port.hpp>
#include <common/log.hpp>
#include <core/pic.hpp>

void eoi(uint64_t int_no) {
    if (int_no >= 40) outb(PIC2_COMMAND, PIC_EOI);
    outb(PIC1_COMMAND, PIC_EOI);
}

void DisablePIC() {
    outb(0xA1, 0xFF);
    outb(0x21, 0xFF);
}

void InitPIC() {
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x00);
    outb(0xA1, 0x00);
}
