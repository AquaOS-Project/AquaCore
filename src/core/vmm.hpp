#include <common/lock.hpp>
#include <cxx/cstdint>

namespace mm::vmm {
    enum flags {
        Present = (1 << 0),
        Write = (1 << 1),
        UserSuper = (1 << 2),
        WriteThrough = (1 << 3),
        CacheDisable = (1 << 4),
        Accessed = (1 << 5),
        LargerPages = (1 << 7),
        PAT = (1 << 7),
        Custom0 = (1 << 9),
        Custom1 = (1 << 10),
        Custom2 = (1 << 11),
        NoExec = (1UL << 63)
    };

    struct PDEntry {
        uint64_t value = 0;

        void SetFlags(uint64_t flags, bool enabled) {
            uint64_t bitSel = flags;
            this->value &= ~bitSel;
            if (enabled) this->value |= bitSel;
        }

        bool GetFlags(uint64_t flags) {
            return (this->value & flags) ? true : false;
        }

        uint64_t GetAddress() {
            return (this->value & 0x000FFFFFFFFFF000) >> 12;
        }

        void SetAddress(uint64_t address) {
            address &= 0x000000FFFFFFFFFF;
            this->value &= 0xFFF0000000000FFF;
            this->value |= (address << 12);
        }
    };

    struct [[gnu::aligned(0x1000)]] PTable {
        PDEntry entries[512];
    };

    struct Pagemap {
        PTable *TopLevel = nullptr;
        uint64_t large_page_size = 0x200000;
        uint64_t page_size = 0x1000;
        lock_t lock;

        bool mapMem(uint64_t VirtualAddress, uint64_t PhysicalAddress, uint64_t flags = (Present | Write), bool hugepages = false);
        bool remapMem(uint64_t VirtualAddress_old, uint64_t VirtualAddress_new, uint64_t flags = (Present | Write), bool hugepages = false);
        bool unmapMem(uint64_t VirtualAddress, bool hugepages = false);

        PDEntry *virt2pte(uint64_t VirtualAddress, bool allocate = true, bool hugepages = false);
        uint64_t VirtualToPhysical(uint64_t VirtualAddress, bool hugepages = false) {
            PDEntry *PMLEntry = this->virt2pte(VirtualAddress, false, hugepages);
            if (PMLEntry == nullptr || !PMLEntry->GetFlags(Present)) return 0;
            return PMLEntry->GetAddress() << 12;
        }
        void switchTo();
        void save();
        Pagemap(bool user = false);
    };

    extern Pagemap *KernelPagemap;
    PTable *GetPagemap();
    void init();
}