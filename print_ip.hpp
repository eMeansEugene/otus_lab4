//
// Created by Eugene on 22.06.2026.
//

#ifndef OTUS_LAB4_PRINT_IP_HPP
#define OTUS_LAB4_PRINT_IP_HPP

#include <iostream>
#include <type_traits>

template <typename T>
std::enable_if_t<std::is_integral_v<T>, void> print_ip(T value) {
    for (std::size_t i = sizeof(T); i > 0; --i) {
        auto shift = (i - 1) * 8; //
        auto byte = (value >> shift) & 0xFF;
        if (i != sizeof(T)) std::cout << '.';
        std::cout << static_cast<int>(byte);
    }
    std::cout << std::endl;
}
#endif // OTUS_LAB4_PRINT_IP_HPP