/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_static_buffer_base_080817221636
#define KSERGEY_static_buffer_base_080817221636

#include <plusone/compiler.hpp>
#include <plusone/expect.hpp>

namespace plusone {

__force_inline static_buffer_base::static_buffer_base(void* data, std::size_t size)
    : data_{reinterpret_cast< char* >(data)}
    , capacity_{size}
{}

__force_inline std::size_t static_buffer_base::capacity() const noexcept
{
    return capacity_;
}

__force_inline std::size_t static_buffer_base::size() const noexcept
{
    return size_;
}

__force_inline const_buffer static_buffer_base::data() const noexcept
{
    return {data_, size_};
}

__force_inline mutable_buffer static_buffer_base::prepare() noexcept
{
    return {data_ + size_, capacity_ - size_};
}

__force_inline mutable_buffer static_buffer_base::prepare(std::size_t size) noexcept
{
    if (__unlikely(capacity_ - size_ < size)) {
        throw std::length_error("Not enought size for output sequence");
    }
    return {data_ + size_, size};
}

__force_inline void static_buffer_base::commit(std::size_t size) noexcept
{
    __expect( size_ + size <= capacity_ );
    size_ += size;
}

__force_inline void static_buffer_base::consume(std::size_t size) noexcept
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

} /* namespace plusone */

#endif /* KSERGEY_static_buffer_base_080817221636 */
