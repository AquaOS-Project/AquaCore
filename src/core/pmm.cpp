#include <boot/LimineRequests.hpp>
#include <common/common.hpp>
#include <common/string.hpp>
#include <common/bitmap.hpp>
#include <common/lock.hpp>
#include <core/panic.hpp>
#include <common/log.hpp>
#include <cxx/algorithm>
#include <core/pmm.hpp>

namespace mm::pmm {

    static uint64_t highest_addr = 0;
    static size_t lastindex = 0;
    static Bitmap bitmap;
    static lock_t lock;

    static size_t freeram = 0;
    static size_t usedram = 0;

    size_t freemem() {
        return freeram;
    }
    size_t usedmem() {
        return usedram;
    }

    void *alloc(size_t count) {
        lockit(lock);

        auto inner_alloc = [count](size_t limit) -> void* {
            size_t p = 0;
            while (lastindex < limit) {
                if (!bitmap[lastindex++]) {
                    if (++p == count) {
                        size_t page = lastindex - count;
                        for (size_t i = page; i < lastindex; i++) bitmap.set(i, true);
                        return reinterpret_cast<void*>(page * 0x1000);
                    }
                }
                else p = 0;
            }
            return nullptr;
        };

        size_t i = lastindex;
        void *ret = inner_alloc(highest_addr / 0x1000);
        if (ret == nullptr)
        {
            lastindex = 0;
            ret = inner_alloc(i);
            if (ret == nullptr) panic("Out of memory!");
        }
        memset(reinterpret_cast<void*>(reinterpret_cast<uint64_t>(ret) + HHDMOffset), 0, count * 0x1000);

        usedram += count * 0x1000;
        freeram -= count * 0x1000;

        return ret;
    }

    void free(void *ptr, size_t count) {
        if (ptr == nullptr) return;
        lockit(lock);

        size_t page = reinterpret_cast<size_t>(ptr) / 0x1000;
        for (size_t i = page; i < page + count; i++) bitmap.set(i, false);
        lastindex = std::max(lastindex, page);

        usedram -= count * 0x1000;
        freeram += count * 0x1000;
    }

    void init() {
        log("Initialising PMM");

        limine_memmap_entry **memmaps = MemoryMapRequest.response->entries;
        uint64_t memmap_count = MemoryMapRequest.response->entry_count;

        for (size_t i = 0; i < memmap_count; i++) {
            if (memmaps[i]->type != LIMINE_MEMMAP_USABLE) continue;

            uintptr_t top = memmaps[i]->base + memmaps[i]->length;
            freeram += memmaps[i]->length;

            highest_addr = std::max(highest_addr, top);
        }

        size_t bitmapSize = align_up((highest_addr / 0x1000) / 8, 0x1000);

        for (size_t i = 0; i < memmap_count; i++) {
            if (memmaps[i]->type != LIMINE_MEMMAP_USABLE) continue;

            if (memmaps[i]->length >= bitmapSize) {
                bitmap.setbuffer(reinterpret_cast<uint8_t*>(memmaps[i]->base), bitmapSize);
                bitmap(0xFF);

                memmaps[i]->length -= bitmapSize;
                memmaps[i]->base += bitmapSize;
                freeram -= bitmapSize;
                usedram += bitmapSize;
                break;
            }
        }

        for (size_t i = 0; i < memmap_count; i++) {
            if (memmaps[i]->type != LIMINE_MEMMAP_USABLE) continue;

            for (uintptr_t t = 0; t < memmaps[i]->length; t += 0x1000) {
                bitmap.set((memmaps[i]->base + t) / 0x1000, false);
            }
        }

        log("Done!");
    }
}