/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_math_310717111020
#define KSERGEY_math_310717111020

#include <type_traits>
#include <array>
#include <plusone/compiler.hpp>

namespace plusone {
namespace detail {

template< class T >
__force_inline constexpr T pow(T base, unsigned exponent) noexcept
{
    static_assert( std::is_integral< T >(), "T must be integral" );

    return exponent == 0 ? 1 : base * pow(base, exponent - 1);
}

constexpr std::array< std::int64_t, 19 > pow10table = {
    pow< std::int64_t >(10, 0),
    pow< std::int64_t >(10, 1),
    pow< std::int64_t >(10, 2),
    pow< std::int64_t >(10, 3),
    pow< std::int64_t >(10, 4),
    pow< std::int64_t >(10, 5),
    pow< std::int64_t >(10, 6),
    pow< std::int64_t >(10, 7),
    pow< std::int64_t >(10, 8),
    pow< std::int64_t >(10, 9),
    pow< std::int64_t >(10, 10),
    pow< std::int64_t >(10, 11),
    pow< std::int64_t >(10, 12),
    pow< std::int64_t >(10, 13),
    pow< std::int64_t >(10, 14),
    pow< std::int64_t >(10, 15),
    pow< std::int64_t >(10, 16),
    pow< std::int64_t >(10, 17),
    pow< std::int64_t >(10, 18)
};

} /* namespace detail */

/** Calculate minimum power-of-two value greater @c num */
__force_inline constexpr std::uint64_t upper_power_of_two(std::uint64_t num) noexcept
{
    num--;
    num |= num >> 1;
    num |= num >> 2;
    num |= num >> 4;
    num |= num >> 8;
    num |= num >> 16;
    num++;
    return num;
}

template< class T >
__force_inline constexpr bool is_power_of_two(T v) noexcept
{
    static_assert( std::is_integral< T >(), "T must be integral" );
    return (v != 0) && ((v & (v - 1)) == 0);
}

__force_inline constexpr std::int64_t pow10(unsigned exponent) noexcept
{
    return exponent < detail::pow10table.size() ? detail::pow10table[exponent] : -1;
}

} /* namespace plusone */

#endif /* KSERGEY_math_310717111020 */
