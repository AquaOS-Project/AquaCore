#pragma once

#include <common/fun.hpp>
#include <core/cpu.hpp>
#include <core/gdt.hpp>
#include <cxx/cstdint>
#include <cxx/cstddef>
#include <cxx/utility>

namespace idt {

    struct InterruptFrame {
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
        void set(void *isr, uint8_t typeattr = 0x8E, uint8_t ist = 0) {
            uint64_t israddr = reinterpret_cast<uint64_t>(isr);
            this->Offset1 = static_cast<uint16_t>(israddr);
            this->Selector = GDT_CODE;
            this->IST = ist;
            this->TypeAttr = typeattr;
            this->Offset2 = static_cast<uint16_t>(israddr >> 16);
            this->Offset3 = static_cast<uint32_t>(israddr >> 32);
            this->Zero = 0;
        }
    };


    struct [[gnu::packed]] IDTPtr {
        uint16_t Limit;
        uint64_t Base;
    };

    class InterruptHandler {
        private:
        std::function<void(registers_t*)> handler;

        public:
        template<typename Func, typename ...Args>
        bool set(Func &&func, Args ...args) {
            if (this->handler == true) return false;
            this->handler = [func = std::forward<Func>(func), args...](registers_t *regs) mutable {
                func(regs, std::move(args)...);
            };
            return true;
        }

        bool clear();
        bool get();
        bool operator()(registers_t *regs);
    };

    extern InterruptHandler handlers[256];
    extern IDTEntry idt[256];
    extern IDTPtr idtr;
    void init();
}