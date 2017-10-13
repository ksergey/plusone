/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_static_buffer_131017131727
#define KSERGEY_static_buffer_131017131727

#include <cstring>
#include <plusone/compiler.hpp>
#include <plusone/expect.hpp>
#include <plusone/exception.hpp>

namespace plusone {

__force_inline static_buffer::static_buffer(void* data, std::size_t size)
    : data_{reinterpret_cast< char* >(data)}
    , capacity_{size}
{}

__force_inline std::size_t static_buffer::capacity() const noexcept
{
    return capacity_;
}

__force_inline std::size_t static_buffer::size() const noexcept
{
    return size_;
}

__force_inline const_buffer static_buffer::data() const noexcept
{
    return {data_, size_};
}

__force_inline mutable_buffer static_buffer::data() noexcept
{
    return {data_, size_};
}

__force_inline mutable_buffer static_buffer::prepare() noexcept
{
    return {data_ + size_, capacity_ - size_};
}

__force_inline mutable_buffer static_buffer::prepare(std::size_t size) noexcept
{
    if (__unlikely(capacity_ - size_ < size)) {
        throw_ex< std::length_error >("Not enought size for output sequence");
    }
    return {data_ + size_, size};
}

__force_inline void static_buffer::commit(std::size_t size) noexcept
{
    __expect( size_ + size <= capacity_ );
    size_ += size;
}

__force_inline void static_buffer::consume(std::size_t size) noexcept
{
    __expect( size <= size_ );

    if (__likely(size_ == size)) {
        size_ = 0;
    } else {
        /* Performance issue */
        std::size_t count = size_ - size;
        std::memmove(data_, data_ + size, count);
        size_ = count;
    }
}

__force_inline void static_buffer::reset(void* data, std::size_t size) noexcept
{
    data_ = reinterpret_cast< char* >(data);
    capacity_ = size;
    size_ = 0;
}

} /* namespace plusone */

#endif /* KSERGEY_static_buffer_131017131727 */
