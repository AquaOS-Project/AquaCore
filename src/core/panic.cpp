#include <common/log.hpp>
#include <core/cpu.hpp>

registers_t *regs;

[[noreturn]] void panic(const char *msg) {
    log("-------KERNEL PANIC-------");
    log(msg);
    log("system halted");
    asm("cli");
    asm("hlt");
    log("Goodbye, World");
}
