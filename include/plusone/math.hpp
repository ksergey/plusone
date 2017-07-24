/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_math_250417152506
#define KSERGEY_math_250417152506

#include <cstdint>
#include <plusone/compiler.hpp>

namespace plusone {

/** Calculate minimum power-of-two value greater @c num */
__force_inline std::uint64_t upper_power_of_two(std::uint64_t num)
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

} /* namespace plusone */

#endif /* KSERGEY_math_250417152506 */
