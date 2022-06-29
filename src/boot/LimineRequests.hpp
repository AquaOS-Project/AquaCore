#pragma once

#include <boot/limine.h>

volatile inline limine_terminal_request TerminalRequest = {
    .id = LIMINE_TERMINAL_REQUEST,
    .revision = 0
};

volatile inline limine_framebuffer_request FrameBufferRequest {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0,
    .response = nullptr
};
