/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_math_250417152506
#define KSERGEY_math_250417152506

#include <cstdint>

namespace plusone {

/** Calculate minimum power-of-two value greater @c num */
constexpr std::uint64_t upper_power_of_two(std::uint64_t num) noexcept;

/** Check value is power of two */
template< class T >
constexpr bool is_power_of_two(T v) noexcept;

/**
 * Get power of 10 (pre-calculated)
 * @return power of 10 for exponent or -1 in case of exponent is too big
 */
constexpr std::int64_t pow10(unsigned exponent) noexcept;

} /* namespace plusone */

#include <plusone/impl/math.ipp>

#endif /* KSERGEY_math_250417152506 */
