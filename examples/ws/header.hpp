/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_header_141017145143
#define KSERGEY_header_141017145143

#include <cstdint>
#include <plusone/buffer_base.hpp>
#include <plusone/expect.hpp>

namespace app {

using namespace plusone;
static constexpr std::size_t max_header_size = 14;

constexpr std::size_t calc_buffer_size(std::size_t max_data_size) noexcept
{
    return max_header_size + max_header_size;
}

__force_inline std::uint8_t* websocket_buffer_cast(const mutable_buffer& b) noexcept
{
    return buffer_cast< std::uint8_t* >(b) + max_header_size;
}

__force_inline std::size_t websocket_buffer_size(const mutable_buffer& b) noexcept
{
    auto size = buffer_size(b);
    __expect( size >= max_header_size );
    return size - max_header_size;
}

} /* namespace app */

#endif /* KSERGEY_header_141017145143 */
