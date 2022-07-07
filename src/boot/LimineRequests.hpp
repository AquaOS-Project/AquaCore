#pragma once

#include <boot/limine.h>

uint64_t inline HHDMOffset = 0;
static constexpr uint64_t STACK_SIZE = 0x4000;

volatile inline limine_terminal_request TerminalRequest {
    .id = LIMINE_TERMINAL_REQUEST,
    .revision = 0
};

volatile inline limine_framebuffer_request FrameBufferRequest {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0,
    .response = nullptr
};

volatile inline limine_hhdm_request HHDMRequest {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0,
    .response = nullptr
};

volatile inline limine_stack_size_request StackSizeRequest {
    .id = LIMINE_STACK_SIZE_REQUEST,
    .revision = 0,
    .response = nullptr,
    .stack_size = STACK_SIZE
};

volatile inline limine_memmap_request MemoryMapRequest {    
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0,
    .response = nullptr
};