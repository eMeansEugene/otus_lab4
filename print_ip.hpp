//
// Created by Eugene on 22.06.2026.
//

#ifndef OTUS_LAB4_PRINT_IP_HPP
#define OTUS_LAB4_PRINT_IP_HPP

#include <iostream>
#include <tuple>
#include <type_traits>
#include <utility>

/// @brief Определяет, является ли тип контейнером (имеет iterator).
/// @tparam T Проверяемый тип.
template <typename T, typename = void>
struct is_container : std::false_type {};

/// @brief Специализация для типов с вложенным iterator.
/// @tparam T Тип контейнера.
template <typename T>
struct is_container<T, std::void_t<typename T::iterator>> : std::true_type {};

/// @brief Определяет, является ли тип std::tuple.
/// @tparam T Проверяемый тип.
template <typename T>
struct is_tuple : std::false_type {};

/// @brief Специализация для std::tuple.
/// @tparam Args Типы элементов кортежа.
template <typename... Args>
struct is_tuple<std::tuple<Args...>> : std::true_type {};

/// @brief Вспомогательная функция проверки одинаковости типов элементов кортежа.
/// @tparam T Тип кортежа.
/// @tparam I Индексы элементов.
template <typename T, std::size_t... I>
constexpr bool all_same_impl(std::index_sequence<I...>) {
    return (std::is_same_v <
        std::tuple_element_t<0, T>,
        std::tuple_element_t<I, T>
    > && ...);
}

/// @brief Проверяет, что все элементы кортежа имеют одинаковый тип.
/// @tparam T Тип кортежа.
/// @return true если все типы одинаковы, false если T не является кортежем.
template <typename T>
constexpr bool all_same() {
    if constexpr (is_tuple<T>::value) {
        return all_same_impl<T>(std::make_index_sequence<std::tuple_size_v<T>>{});
    } else {
        return false;
    }
}

/// @brief Вспомогательная функция вывода элементов кортежа через точку.
/// @tparam T Тип кортежа.
/// @tparam I Индексы элементов.
/// @param value Кортеж для вывода.
template <typename T, std::size_t... I>
void print_tuple_impl(const T& value, std::index_sequence<I...>) {
    ((std::cout << (I == 0 ? "" : ".") << std::get<I>(value)), ...);
}

/// @brief Выводит условный IP-адрес для целочисленного типа.
/// Байты выводятся в беззнаковом виде, начиная со старшего, через точку.
/// @tparam T Целочисленный тип.
/// @param value Значение для вывода.
template <typename T>
std::enable_if_t<std::is_integral_v<T>, void> print_ip(const T& value) {
    for (std::size_t i = sizeof(T); i > 0; --i) {
        auto shift = (i - 1) * 8;
        auto byte = (value >> shift) & 0xFF;
        if (i != sizeof(T)) std::cout << '.';
        std::cout << static_cast<int>(byte);
    }
    std::cout << std::endl;
}

/// @brief Выводит условный IP-адрес для std::string.
/// Строка выводится как есть.
/// @tparam T Тип std::string.
/// @param value Строка для вывода.
template <typename T>
std::enable_if_t<std::is_same_v<T, std::string>, void> print_ip(const T& value) {
    std::cout << value << std::endl;
}

/// @brief Выводит условный IP-адрес для контейнеров std::list и std::vector.
/// Элементы выводятся через точку.
/// @tparam T Тип контейнера.
/// @param value Контейнер для вывода.
template <typename T>
std::enable_if_t<is_container<T>::value && !std::is_same_v<T, std::string>, void> print_ip(const T& value) {
    for (auto it = value.begin(); it != value.end(); ++it) {
        if (it != value.begin()) std::cout << '.';
        std::cout << *it;
    }
    std::cout << std::endl;
}

/// @brief Выводит условный IP-адрес для std::tuple с одинаковыми типами элементов.
/// Если типы элементов различаются — ошибка компиляции.
/// @tparam T Тип кортежа.
/// @param value Кортеж для вывода.
template <typename T>
std::enable_if_t<all_same<T>(), void> print_ip(const T& value) {
    print_tuple_impl(value, std::make_index_sequence<std::tuple_size_v<T>>{});
    std::cout << std::endl;
}

#endif // OTUS_LAB4_PRINT_IP_HPP