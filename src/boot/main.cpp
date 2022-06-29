#include <boot/limine.h>
#include <boot/LimineRequests.hpp>
#include <lib/log.hpp>
#include <core/pic.hpp>
#include <core/cpu.hpp>
#include <dev/fb.hpp>
#include <core/idt.hpp>

extern "C" void main(void) {
      if (TerminalRequest.response == NULL || TerminalRequest.response->terminal_count < 1) {
        asm("hlt");
      }
      struct limine_terminal *terminal = TerminalRequest.response->terminals[0];
      log("\e[1;1H\e[2J");
      log("Hello, world!\nstarting aqua 0.0.1...");
      log("starting PIC...");
      InitPIC();
      log("done!");
      log("starting CPU features..");
      InitCPUfeat();
      log("done!");
      log("starting IDT...");
      InitIDT();
      log("done!");
      log("starting framebuffer...");
      InitFB();
      log("done!");
      TerminalRequest.response->write(terminal, "check debug console if you want to interact with aqua", 54);
      for (;;) {
         asm ("hlt");
  }
}
