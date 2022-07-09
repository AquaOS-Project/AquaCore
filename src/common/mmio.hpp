#pragma once

#include <cxx/type_traits>
#include <cxx/cstdint>

template<typename type>
concept valuetype = (std::is_same<type, uint8_t>() || std::is_same<type, uint16_t>() || std::is_same<type, uint32_t>() || std::is_same<type, uint64_t>());

template<valuetype type>
static inline type mmin(void *address) {
    volatile auto ptr = reinterpret_cast<volatile type*>(address);
    return *ptr;
}

template<valuetype type>
static inline void mmout(void *address, type value) {
    volatile auto ptr = reinterpret_cast<volatile type*>(address);
    *ptr = value;
}
