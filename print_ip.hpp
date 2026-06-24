//
// Created by Eugene on 22.06.2026.
//

#ifndef OTUS_LAB4_PRINT_IP_HPP
#define OTUS_LAB4_PRINT_IP_HPP

#include <iostream>
#include <tuple>
#include <type_traits>
#include <utility>

/// @brief Определяет, является ли тип контейнером (имеет вложенный тип iterator).
/// @tparam T Проверяемый тип.
template <typename T, typename = void>
struct is_container : std::false_type {};

/// @brief Специализация для типов с вложенным iterator.
/// @tparam T Тип контейнера.
template <typename T>
struct is_container<T, std::void_t<typename T::iterator>> : std::true_type {};

/// @brief Определяет, является ли тип экземпляром std::tuple.
/// @tparam T Проверяемый тип.
template <typename T>
struct is_tuple : std::false_type {};

/// @brief Специализация для std::tuple с произвольными типами элементов.
/// @tparam Args Типы элементов кортежа.
template <typename... Args>
struct is_tuple<std::tuple<Args...>> : std::true_type {};

/// @brief Определяет, является ли тип std::tuple с одинаковыми типами элементов.
/// Для не-tuple типов всегда false.
/// @tparam T Проверяемый тип.
template <typename T, typename = void>
struct is_homogeneous_tuple : std::false_type {};

/// @brief Специализация для std::tuple.
/// Наследует true_type если все типы элементов совпадают с первым, иначе false_type.
/// @tparam T Тип первого элемента (эталон для сравнения).
/// @tparam Rest Типы остальных элементов.
template <typename T, typename... Rest>
struct is_homogeneous_tuple<std::tuple<T, Rest...>>
    : std::bool_constant<(std::is_same_v<T, Rest> && ...)> {};

/// @brief Вспомогательная функция вывода элементов кортежа через точку.
/// Использует fold expression для разворачивания индексов на этапе компиляции.
/// @tparam T Тип кортежа.
/// @tparam I Пакет индексов элементов.
/// @param value Кортеж для вывода.
template <typename T, std::size_t... I>
void print_tuple_impl(const T& value, std::index_sequence<I...>) {
    ((std::cout << (I == 0 ? "" : ".") << std::get<I>(value)), ...);
}

/// @brief Выводит условный IP-адрес для целочисленного типа.
/// Байты выводятся в беззнаковом виде начиная со старшего, разделённые точкой.
/// Выводятся все байты числа вне зависимости от размера типа.
/// @tparam T Целочисленный тип (int8_t, int16_t, int32_t, int64_t и др.).
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
/// Строка выводится как есть, вне зависимости от содержимого.
/// @tparam T Тип std::string.
/// @param value Строка для вывода.
template <typename T>
std::enable_if_t<std::is_same_v<T, std::string>, void> print_ip(const T& value) {
    std::cout << value << std::endl;
}

/// @brief Выводит условный IP-адрес для контейнеров std::list и std::vector.
/// Элементы выводятся через точку в порядке итерации.
/// std::string явно исключён, несмотря на наличие iterator.
/// @tparam T Тип контейнера (std::vector, std::list и др.).
/// @param value Контейнер для вывода.
template <typename T>
std::enable_if_t<is_container<T>::value && !std::is_same_v<T, std::string>, void> print_ip(const T& value) {
    for (auto it = value.begin(); it != value.end(); ++it) {
        if (it != value.begin()) std::cout << '.';
        std::cout << *it;
    }
    std::cout << std::endl;
}

/// @brief Выводит условный IP-адрес для однородного std::tuple.
/// Элементы выводятся через точку. Если типы элементов различаются —
/// функция не участвует в разрешении перегрузок, что приводит к ошибке компиляции.
/// @tparam T Тип кортежа с одинаковыми типами элементов.
/// @param value Кортеж для вывода.
template <typename T>
std::enable_if_t<is_homogeneous_tuple<T>::value, void> print_ip(const T& value) {
    print_tuple_impl(value, std::make_index_sequence<std::tuple_size_v<T>>{});
    std::cout << std::endl;
}

#endif // OTUS_LAB4_PRINT_IP_HPP