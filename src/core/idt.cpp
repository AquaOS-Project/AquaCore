#include <common/string.hpp>
#include <common/log.hpp>
#include <core/cpu.hpp>
#include <core/gdt.hpp>
#include <core/idt.hpp>

namespace idt {
    InterruptHandler handlers[256];
    static IDTGate idt[256];
    static IDTDescriptor IdtDescriptor;
    IDTPtr idtr;

    bool InterruptHandler::clear() {
        bool ret = (this->handler == true);
        this->handler.clear();
        return ret;
    }

    bool InterruptHandler::get() {
        return this->handler == true;
    }

    bool InterruptHandler::operator()(registers_t *regs) {
        if (this->handler == false) return false;
        this->handler(regs);
        return true;
    }

    void init() {
        log("starting IDT...");
        idtr.Limit = sizeof(IDTEntry) * 256 - 1;
        idtr.Base = reinterpret_cast<uintptr_t>(&idt[0]);
        memset(idt, 0, sizeof(idt));
        IdtDescriptor = {sizeof(idt), (uint64_t)&idt};
        log("Done!");
    }
}