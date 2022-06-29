#include <lib/log.hpp>
#include <core/cpu.hpp>

static const char *exceptions[32] = {
    "Division by zero",
    "Debug",
    "Non-maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved",
    "x87 Floating-Point Exception",
    "Aligment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Security Exception",
    "Reserved"
};

void panic(const char *msg) {
    log("-------KERNEL PANIC-------");
    log(msg);
    log("system halted");
    asm("cli");
    asm("hlt");
    log("Goodbye, World");
}

static void ExceptionHandler() {
    error("-------System exception!-------");
    error("Address: 0x%lX", regs->rip);
    error("Error code: 0x%lX, 0b%b", regs->error_code, regs->error_code);
}
