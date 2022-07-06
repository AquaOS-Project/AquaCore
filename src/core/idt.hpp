#pragma once

#include <cxx/cstdint>
#include <cxx/cstddef>

struct INTFrame {
    uint64_t error_code;
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
};

struct [[gnu::packed]] IDTDescriptor {
    uint16_t limit;
    uint64_t offset;
};

struct [[gnu::packed]] IDTGate {
    uint16_t offset1;
    uint16_t selector;
    uint8_t ist;
    uint8_t type;
    uint16_t offset2;
    uint32_t offset3;
    uint32_t zero;
};

struct [[gnu::packed]] IDTEntry {
    uint16_t Offset1;
    uint16_t Selector;
    uint8_t IST;
    uint8_t TypeAttr;
    uint16_t Offset2;
    uint32_t Offset3;
    uint32_t Zero;
};

struct [[gnu::packed]] IDTPtr {
    uint16_t Limit;
    uint64_t Base;
};

void RegisterINTHandler(size_t index, uint64_t addr, uint8_t gateType, uint8_t ist);
void InitIDT();
void LoadIDT();
