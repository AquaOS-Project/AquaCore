#include <boot/LimineRequests.hpp>
#include <common/lock.hpp>
#include <common/log.hpp>
#include <core/gdt.hpp>

namespace gdt {
    [[gnu::aligned(0x1000)]] GDT DefaultGDT {
        {0x0000, 0, 0, 0x00, 0x00, 0}, 
        {0xFFFF, 0, 0, 0x9A, 0x00, 0}, 
        {0xFFFF, 0, 0, 0x92, 0x00, 0},
        {0xFFFF, 0, 0, 0x9A, 0xCF, 0},
        {0xFFFF, 0, 0, 0x92, 0xCF, 0},
        {0x0000, 0, 0, 0x9A, 0x20, 0},
        {0x0000, 0, 0, 0x92, 0x00, 0},
        {0x0000, 0, 0, 0xFA, 0x20, 0},
        {0x0000, 0, 0, 0xF2, 0x00, 0},
        {0x0000, 0, 0, 0x89, 0x00, 0, 0, 0} 
    };

    NewLock(GDTLock);
    GDTDescriptor gdtDescriptor;
    TSS *tss = new gdt::TSS[SMPRequest.response->cpu_count]();

    void reloadall(size_t cpu) {
        lockit(GDTLock);

        uintptr_t base = reinterpret_cast<uintptr_t>(&tss[cpu]);
        uintptr_t limit = base + sizeof(tss[cpu]);

        DefaultGDT.Tss.Length = limit;

        DefaultGDT.Tss.Base0 = base;
        DefaultGDT.Tss.Base1 = base >> 16;
        DefaultGDT.Tss.Flags1 = 0x89;
        DefaultGDT.Tss.Flags2 = 0x00;
        DefaultGDT.Tss.Base2 = base >> 24;
        DefaultGDT.Tss.Base3 = base >> 32;
        DefaultGDT.Tss.Reserved = 0x00;

        asm volatile ("lgdt %0" : : "m"(gdtDescriptor) : "memory");
        asm volatile ("ltr %0" : : "r"(static_cast<uint16_t>(gdt::GDT_TSS)));
    }

    void init() {
        log("Initialising GDT...");
        gdtDescriptor.Size = sizeof(GDT) - 1;
        gdtDescriptor.Offset = reinterpret_cast<uint64_t>(&DefaultGDT);
        reloadall(0);
        tss[0].RSP[0] = reinterpret_cast<uint64_t>(KernelStack + StackSize);
    }
}