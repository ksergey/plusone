/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_consuming_buffer_090817105123
#define KSERGEY_consuming_buffer_090817105123

#include <plusone/compiler.hpp>
#include <plusone/expect.hpp>

namespace plusone {

__force_inline consuming_buffer::consuming_buffer(const void* data, std::size_t size) noexcept
    : data_{static_cast< const char* >(data)}
    , size_{size}
{}

__force_inline std::size_t consuming_buffer::size() const noexcept
{
    return size_;
}

__force_inline const_buffer consuming_buffer::data() const noexcept
{
    return {data_, size_};
}

__force_inline void consuming_buffer::consume(std::size_t size) noexcept
{
    __expect( size <= size_ );
    data_ += size;
    size_ -= size;
}

} /* namespace plusone */

#endif /* KSERGEY_consuming_buffer_090817105123 */
