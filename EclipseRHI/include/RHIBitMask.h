#pragma once
#include <type_traits>

template<typename T> struct EnableBitMaskOperators : std::false_type {};

template<typename T>
concept BitMaskEnum = std::is_enum_v<T> && EnableBitMaskOperators<T>::value;

template<BitMaskEnum  E>
constexpr E operator|(E lhs, E rhs)
{
    using U = std::underlying_type_t<E>;
    return static_cast<E>(static_cast<U>(lhs) | static_cast<U>(rhs));
}

template<BitMaskEnum  E>
constexpr E operator&(E lhs, E rhs) 
{
    using U = std::underlying_type_t<E>;
    return static_cast<E>(static_cast<U>(lhs) & static_cast<U>(rhs));
}

template<BitMaskEnum E>
constexpr E operator^(E lhs, E rhs) 
{
    using U = std::underlying_type_t<E>;
    return static_cast<E>(static_cast<U>(lhs) ^ static_cast<U>(rhs));
}

template<BitMaskEnum E>
constexpr E operator~(E lhs)
{
    using U = std::underlying_type_t<E>;
    return static_cast<E>(~static_cast<U>(lhs));
}

template<BitMaskEnum E>
constexpr E operator|=(E& lhs, E rhs) 
{
    using U = std::underlying_type_t<E>;
    lhs = static_cast<E>(static_cast<U>(lhs) | static_cast<U>(rhs));
    return lhs;
}

template<BitMaskEnum E>
constexpr E operator&=(E& lhs, E rhs) 
{
    using U = std::underlying_type_t<E>;
    lhs = static_cast<E>(static_cast<U>(lhs) & static_cast<U>(rhs));
    return lhs;
}

template<BitMaskEnum E>
constexpr E operator^=(E& lhs, E rhs) 
{
    using U = std::underlying_type_t<E>;
    lhs = static_cast<E>(static_cast<U>(lhs) ^ static_cast<U>(rhs));
    return lhs;
}

template<BitMaskEnum E>
constexpr bool HasFlag(E value, E flag)
{
    return (value & flag) != E{};
}

#define ENIGMA_ENABLE_BITMASK_OPERATORS(EnumType) \
    template<> struct EnableBitMaskOperators<EnumType> : std::true_type {};
