/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_math_310717111020
#define KSERGEY_math_310717111020

#include <type_traits>
#include <array>
#include <plusone/expect.hpp>
#include <plusone/compiler.hpp>

namespace plusone {
namespace detail {

template< class T >
constexpr T pow(T base, unsigned exponent) noexcept
{
    static_assert( std::is_integral< T >(), "T must be integral" );
    return exponent == 0 ? 1 : base * pow(base, exponent - 1);
}

template< class T, std::size_t... Is >
constexpr auto make_pow_array(T base, std::index_sequence< Is... >) noexcept
{
    return std::array< T, sizeof...(Is) >{{pow< T >(base, Is)...}};
}

template< class T, std::size_t N >
constexpr std::array< T, N > make_pow_array(T base) noexcept
{
    return make_pow_array(base, std::make_index_sequence< N >{});
}

constexpr auto pow10table = make_pow_array< std::uint64_t, pow10::table_size >(10);

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

__force_inline constexpr std::uint64_t pow10::get(unsigned exponent) noexcept
{
    return detail::pow10table[exponent];
}

} /* namespace plusone */

#endif /* KSERGEY_math_310717111020 */
