#pragma once

#include <boot/limine.h>

uint8_t *KernelStack;
uint64_t inline HHDMOffset = 0;
static constexpr uint64_t StackSize = 0x4000;

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
    .stack_size = StackSize
};

volatile inline limine_memmap_request MemoryMapRequest {    
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0,
    .response = nullptr
};

volatile inline limine_kernel_file_request KernelFileRequest {
    .id = LIMINE_KERNEL_FILE_REQUEST,
    .revision = 0,
    .response = nullptr
};

volatile inline limine_kernel_address_request KernelAddressRequest {
    .id = LIMINE_KERNEL_ADDRESS_REQUEST,
    .revision = 0,
    .response = nullptr
};

volatile limine_smp_request SMPRequest {
    .id = LIMINE_SMP_REQUEST,
    .revision = 0,
    .response = nullptr,
    .flags = (1 << 0)
};