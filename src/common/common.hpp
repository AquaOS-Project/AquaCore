#pragma once

#include <boot/LimineRequests.hpp>

constexpr uint64_t align_down(uint64_t n, uint64_t a) {
    return (n & ~(a - 1));
}

constexpr uint64_t align_up(uint64_t n, uint64_t a) {
    return align_down(n + a - 1, a);
}

constexpr uint64_t div_roundup(uint64_t n, uint64_t a) {
    return align_down(n + a - 1, a) / a;
}

constexpr uint64_t jdn(uint8_t days, uint8_t months, uint16_t years) {
    return (1461 * (years + 4800 + (months - 14) / 12)) / 4 + (367 * (months - 2 - 12 * ((months - 14) / 12))) / 12 - (3 * ((years + 4900 + (months - 14) / 12) / 100)) / 4 + days - 32075;
}

template<typename type>
constexpr type tohh(type a) {
    return reinterpret_cast<type>(reinterpret_cast<uint64_t>(a) + HHDMOffset);
}

template<typename type>
constexpr type fromhh(type a) {
    return reinterpret_cast<type>(reinterpret_cast<uint64_t>(a) - HHDMOffset);
}