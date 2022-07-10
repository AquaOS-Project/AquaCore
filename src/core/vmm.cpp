#include <boot/LimineRequests.hpp>
#include <common/common.hpp>
#include <common/log.hpp>
#include <boot/limine.h>
#include <core/cpu.hpp>
#include <core/mm.hpp>

namespace mm::vmm { 
    Pagemap *KernelPagemap = nullptr;

    static PTable *GetNextLevel(PTable *CurrentLevel, size_t entry, bool allocate = true) {
        PTable *ret = nullptr;
        if (CurrentLevel->entries[entry].GetFlags(Present)) {
            ret = reinterpret_cast<PTable*>(CurrentLevel->entries[entry].GetAddress() << 12);
        }
        else if (allocate == true) {
            ret = pmm::alloc<PTable*>();
            CurrentLevel->entries[entry].SetAddress(reinterpret_cast<uint64_t>(ret) >> 12);
            CurrentLevel->entries[entry].SetFlags(Present | Write | UserSuper, true);
        }
        return ret;
    }

    PDEntry *Pagemap::virt2pte(uint64_t VirtualAddress, bool allocate, bool hugepages) {
        size_t PML5Entry = (VirtualAddress & (static_cast<uint64_t>(0x1FF) << 48)) >> 48;
        size_t PML4Entry = (VirtualAddress & (static_cast<uint64_t>(0x1FF) << 39)) >> 39;
        size_t PML3Entry = (VirtualAddress & (static_cast<uint64_t>(0x1FF) << 30)) >> 30;
        size_t PML2Entry = (VirtualAddress & (static_cast<uint64_t>(0x1FF) << 21)) >> 21;
        size_t PML1Entry = (VirtualAddress & (static_cast<uint64_t>(0x1FF) << 12)) >> 12;

        PTable *pml5, *pml4, *pml3, *pml2, *pml1;

        pml5 = this->TopLevel;
        if (pml5 == nullptr) return nullptr;

        pml4 = GetNextLevel(pml5, PML5Entry, allocate);
        if (pml4 == nullptr) return nullptr;

        pml3 = GetNextLevel(pml4, PML4Entry, allocate);
        if (pml3 == nullptr) return nullptr;

        pml2 = GetNextLevel(pml3, PML3Entry, allocate);
        if (pml2 == nullptr) return nullptr;
        if (hugepages) return &pml2->entries[PML2Entry];

        pml1 = GetNextLevel(pml2, PML2Entry, allocate);
        if (pml1 == nullptr) return nullptr;

        return &pml1->entries[PML1Entry];
    }

    bool Pagemap::mapMem(uint64_t VirtualAddress, uint64_t PhysicalAddress, uint64_t flags, bool hugepages) {
        lockit(this->lock);

        PDEntry *PMLEntry = this->virt2pte(VirtualAddress, true, hugepages);
        if (PMLEntry == nullptr) {
            log("[VMM] Could not get page map entry!");
            return false;
        }

        PMLEntry->SetAddress(PhysicalAddress >> 12);
        PMLEntry->SetFlags(flags | (hugepages ? LargerPages : 0), true);
        return true;
    }

    bool Pagemap::remapMem(uint64_t VirtualAddress_old, uint64_t VirtualAddress_new, uint64_t flags, bool hugepages) {
        this->lock.lock();

        PDEntry *PML1Entry = this->virt2pte(VirtualAddress_old, false, hugepages);
        if (PML1Entry == nullptr) {
            log("[VMM] Could not get page map entry!");
            this->lock.unlock();
            return false;
        }

        uint64_t PhysicalAddress = PML1Entry->GetAddress() << 12;
        PML1Entry->value = 0;
        invlpg(VirtualAddress_old);
        this->lock.unlock();

        this->mapMem(VirtualAddress_new, PhysicalAddress, flags, hugepages);
        return true;
    }

    bool Pagemap::unmapMem(uint64_t VirtualAddress, bool hugepages) {
        lockit(this->lock);

        PDEntry *PMLEntry = this->virt2pte(VirtualAddress, false, hugepages);
        if (PMLEntry == nullptr) {
            log("[VMM] Could not get page map entry!");
            return false;
        }

        PMLEntry->value = 0;
        invlpg(VirtualAddress);
        return true;
    }

    void Pagemap::switchTo() {
        write_cr(3, reinterpret_cast<uint64_t>(this->TopLevel));
    }

    void Pagemap::save() {
        this->TopLevel = reinterpret_cast<PTable*>(read_cr(3));
    }

    Pagemap::Pagemap(bool user) {
        this->large_page_size = 0x200000;
        this->page_size = 0x1000;

        this->TopLevel = pmm::alloc<PTable*>();

        if (user == false) {
            for (uint64_t i = 0; i < 0x100000000; i += this->large_page_size) {
                this->mapMem(i, i, flags::Present | flags::Write, true);
                this->mapMem(tohh(i), i, flags::Present | flags::Write, true);
            }

            for (size_t i = 0; i < MemoryMapRequest.response->entry_count; i++) {
                limine_memmap_entry *mmap = MemoryMapRequest.response->entries[i];

                uint64_t base = align_down(mmap->base, this->page_size);
                uint64_t top = align_up(mmap->base + mmap->length, this->page_size);
                if (top < 0x100000000) continue;

                for (uint64_t t = base; t < top; t += this->page_size) {
                    if (t < 0x100000000) continue;
                    this->mapMem(t, t, flags::Present | flags::Write);
                    this->mapMem(tohh(t), t, flags::Present | flags::Write);
                }
            }
        }

        for (size_t i = 0; i < KernelFileRequest.response->kernel_file->size; i += this->page_size) {
            uint64_t PhysicalAddress = KernelAddressRequest.response->physical_base + i;
            uint64_t VirtualAddress = KernelAddressRequest.response->virtual_base + i;
            this->mapMem(VirtualAddress, PhysicalAddress, flags::Present | flags::Write);
        }
    }

    PTable *GetPagemap() {
        return reinterpret_cast<PTable*>(read_cr(3));
    }

    void init() {
        log("Initialising VMM...");
        KernelPagemap = new Pagemap();
        KernelPagemap->switchTo();
        log("Done!");
    }
}