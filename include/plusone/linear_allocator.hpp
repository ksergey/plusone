/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_linear_allocator_020118111334
#define KSERGEY_linear_allocator_020118111334

#include <cassert>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include "compiler.hpp"

namespace plusone {

class linear_allocator
{
private:
    std::int8_t* begin_{nullptr};
    std::int8_t* end_{nullptr};
    std::int8_t* offset_{nullptr};

public:
    using pointer = void*;
    using size_type = std::size_t;

    linear_allocator(const linear_allocator&) = delete;
    linear_allocator& operator=(const linear_allocator&) = delete;

    linear_allocator(linear_allocator&& v) noexcept
    {
        swap(v);
    }

    linear_allocator& operator=(linear_allocator&& v) noexcept
    {
        swap(v);
        return *this;
    }

    linear_allocator() = default;

    explicit linear_allocator(size_type max_size)
    {
        begin_ = new std::int8_t[max_size];
        end_ = begin_ + max_size;
        offset_ = begin_;
    }

    ~linear_allocator() noexcept
    {
        delete[] begin_;
    }

    pointer allocate(size_type size, std::uint8_t alignment = 4) noexcept
    {
        std::uint8_t adjustment = alignment - (reinterpret_cast< std::uintptr_t >(offset_) & static_cast< std::uintptr_t >(alignment - 1));
        if (adjustment == alignment) {
            adjustment = 0;
        }

        if (__unlikely(offset_ + adjustment + size > end_)) {
            return nullptr;
        }

        auto result = offset_ + adjustment;
        offset_ = result + size;
        return result;
    }

    void deallocate(pointer, size_type) noexcept
    {
        assert( false && "Deallocation not supported by linear allocator" );
    }

    void reset() noexcept
    {
        offset_ = begin_;
    }

    void swap(linear_allocator& v) noexcept
    {
        std::swap(begin_, v.begin_);
        std::swap(end_, v.end_);
        std::swap(offset_, v.offset_);
    }
};

} /* namespace plusone */

#endif /* KSERGEY_linear_allocator_020118111334 */
