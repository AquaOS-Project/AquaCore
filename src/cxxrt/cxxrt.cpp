#include <core/panic.hpp>
#include <cxx/cstdint>

extern "C" int __cxa_atexit([[maybe_unused]] void (*func)(void *), [[maybe_unused]] void *arg, [[maybe_unused]] void *dso_handle) {
        return 0;
    }