#include <stdint.h>
#include <stddef.h>
#include <stivale2.h>
//#include <vga.h>

__attribute__((aligned(16)))
static uint8_t stack[8192];

static struct stivale2_header_tag_terminal terminal_hdr_tag = {
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_TERMINAL_ID,
        .next = 0
    },
    .flags = 0
};

static struct stivale2_header_tag_framebuffer framebuffer_hdr_tag = {
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_FRAMEBUFFER_ID,
        .next = (uint64_t)&terminal_hdr_tag
    },
    .framebuffer_width  = 0,
    .framebuffer_height = 0,
    .framebuffer_bpp    = 0
};

__attribute__((section(".stivale2hdr"), used))
static struct stivale2_header stivale_hdr = {
    .entry_point = 0,
    .stack = (uintptr_t)stack + sizeof(stack),
    .flags = (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4),
    .tags = (uintptr_t)&framebuffer_hdr_tag
};

void *stivale2_get_tag(struct stivale2_struct *stivale2_struct, uint64_t id) {
    struct stivale2_tag *current_tag = (void *)stivale2_struct->tags;
    for (;;) {
        if (current_tag == NULL) {
            return NULL;
        }

        if (current_tag->identifier == id) {
            return current_tag;
        }

        current_tag = (stivale2_tag*)current_tag->next;
    }
}

extern "C" void _start(struct stivale2_struct *stivale2_struct) {
    stivale2_struct_tag_terminal *term_str_tag;
    term_str_tag = (struct stivale2_struct_tag_terminal *)stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_TERMINAL_ID);
    if (term_str_tag == NULL) {
        for (;;) {
            asm ("hlt");
        }
}
    void *term_write_ptr = (void *)term_str_tag->term_write;
    void (*term_write)(const char *string, size_t length) = term_write_ptr;
    term_write("Hello, World!\n", 13);
    for (;;) {
        asm ("hlt");
    }
}
