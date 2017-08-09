/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_buffer_base_090817110342
#define KSERGEY_buffer_base_090817110342

#include <plusone/compiler.hpp>

namespace plusone {
namespace detail {

__force_inline void* buffer_cast_helper(const mutable_buffer& v)
{
    return v.data_;
}

__force_inline const void* buffer_cast_helper(const const_buffer& v)
{
    return v.data_;
}

__force_inline std::size_t buffer_size_helper(const mutable_buffer& v)
{
    return v.size_;
}

__force_inline std::size_t buffer_size_helper(const const_buffer& v)
{
    return v.size_;
}

} /* namespace detail */

__force_inline mutable_buffer::mutable_buffer(void* data, std::size_t size)
    : data_{data}
    , size_{size}
{}

__force_inline const_buffer::const_buffer(const void* data, std::size_t size)
    : data_{data}
    , size_{size}
{}

template< class PointerT >
__force_inline PointerT buffer_cast(const mutable_buffer& v)
{
    return static_cast< PointerT >(detail::buffer_cast_helper(v));
}

template< class PointerT >
__force_inline PointerT buffer_cast(const const_buffer& v)
{
    return static_cast< PointerT >(detail::buffer_cast_helper(v));
}

template< class BufferT >
__force_inline std::size_t buffer_size(const BufferT& v)
{
    return detail::buffer_size_helper(v);
}

} /* namespace plusone */

#endif /* KSERGEY_buffer_base_090817110342 */
