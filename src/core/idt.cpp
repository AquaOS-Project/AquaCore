#include <core/idt.hpp>
#include <lib/string.hpp>

static IDTGate idt[256];
static IDTDescriptor IdtDescriptor;
IDTPtr idtr;

void RegisterINTHandler(size_t index, uint64_t addr, uint8_t gateType, uint8_t ist) {
    idt[index].offset1 = addr & 0xFFFF;
    idt[index].offset2 = (addr >> 16) & 0xFFFF;
    idt[index].offset3 = addr >> 32;
    idt[index].selector = 0x08;
    idt[index].type = gateType;
    idt[index].zero = 0;
    idt[index].ist = ist;
}

void InitIDT() {
    idtr.Limit = sizeof(IDTEntry) * 256 - 1;
    idtr.Base = reinterpret_cast<uintptr_t>(&idt[0]);
    memset(idt, 0, sizeof(idt));
    IdtDescriptor = {sizeof(idt), (uint64_t)&idt};
}

void LoadIDT() {
    asm volatile("lidt (%0)" :: "r"(&IdtDescriptor));
}
