#pragma once

#include <cstdint>
#include <lib/string.hpp>

#define pack __attribute__((__packed__))
#define toMB(x) ((x) / 1024 / 1024)
#define toKB(x) ((x) / 1024)
#define align(val, alg) (val + (alg - (val % alg)))
#define alignD(val, alg) (align(val, alg) - alg)
#define unsafe_cast(val, type) (*(type *)&val)
#define inlf static inline __attribute__((always_inline))
#define doptimize __attribute__((optimize("O0")))
#define between(a, b, c) (((uint64_t)(a) >= (uint64_t)(b)) && ((uint64_t)(a) <= (uint64_t)(c)))
#define VMM_ENTRY_PRESENT 0
#define VMM_ENTRY_RW 1
#define VMM_ENTRY_USER 2
#define VMM_ENTRY_WRITE_THROUGH 3
#define VMM_ENTRY_CACHE 4
#define VMM_ENTRY_ACCESSED 5
#define VMM_ENTRY_DIRTY 6
#define VMM_ENTRY_HUGE_PAGES 7
#define VMM_ENTRY_NO_EXECUTE 63
#define VMM_PAGE 4096

struct mm_pool {
    uint64_t total;
    uint64_t available;
    uint64_t used;
    bool full;
    void *base;
    void *allocableBase;
    size_t pageIndex;
    size_t bitmapReserved;
    size_t bitmapByteIndex;
    uint8_t bitmapBitIndex;
    uint8_t *bitmapBase;
};

inlf void controlLoadCR4(uint64_t value) {
    asm volatile("mov %0, %%cr4" ::"r"(value));
}

inlf void controlLoadCR3(uint64_t value) {
    asm volatile("mov %0, %%cr3" ::"r"(value));
}

struct pack vmm_index {
    uint64_t PML4;
    uint64_t PDP;
    uint64_t PD;
    uint64_t PT;
    uint64_t P;
};

struct pack vmm_page_table {
    uint64_t entries[512];
};

inlf bool vmmGetFlag(uint64_t *entry, uint8_t flag) {
    return *entry & (1 << flag);
}

inlf void vmmSetFlag(uint64_t *entry, uint8_t flag, bool value) {
    if (value) {
        *entry |= (1 << flag);
        return;
    }

    *entry &= ~(1 << flag); // unset flag
}

inlf uint64_t vmmGetAddress(uint64_t *entry) {
    return (*entry & 0x000FFFFFFFFFF000) >> 12; // get the address from entry
}

inlf void vmmSetAddress(uint64_t *entry, uint64_t address) {
    *entry &= 0xfff0000000000fff;             // clear address field
    *entry |= (address & 0xFFFFFFFFFF) << 12; // set the address field
}
