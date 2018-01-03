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
    std::int8_t* own_{nullptr};
    std::int8_t* begin_{nullptr};
    std::int8_t* end_{nullptr};
    std::int8_t* ptr_{nullptr};

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
        own_ = new std::int8_t[max_size];
        begin_ = own_;
        end_ = begin_ + max_size;
        ptr_ = begin_;
    }

    /// Construct allocator with non-owning memory
    linear_allocator(void* buf, size_type size)
    {
        begin_ = static_cast< std::int8_t* >(buf);
        end_ = begin_ + size;
        ptr_ = begin_;
    }

    ~linear_allocator() noexcept
    {
        delete[] own_;
    }

    size_type allocated_size() const noexcept
    {
        return static_cast< size_type >(ptr_ - begin_);
    }

    size_type free_size() const noexcept
    {
        return static_cast< size_type >(end_ - ptr_);
    }

    size_type total_size() const noexcept
    {
        return static_cast< size_type >(end_ - begin_);
    }

    pointer allocate(size_type size) noexcept
    {
        if (__unlikely(ptr_ + size > end_)) {
            return nullptr;
        }

        auto result = ptr_;
        ptr_ += size;
        return result;
    }

    pointer allocate(size_type size, std::uint8_t alignment) noexcept
    {
        std::uint8_t adjustment = alignment - (reinterpret_cast< std::uintptr_t >(ptr_) & static_cast< std::uintptr_t >(alignment - 1));
        if (adjustment == alignment) {
            adjustment = 0;
        }

        if (__unlikely(ptr_ + adjustment + size > end_)) {
            return nullptr;
        }

        auto result = ptr_ + adjustment;
        ptr_ = result + size;
        return result;
    }

    void deallocate(pointer, size_type) noexcept
    {
        assert( false && "Deallocation not supported by linear allocator" );
    }

    void reset() noexcept
    {
        ptr_ = begin_;
    }

    void swap(linear_allocator& v) noexcept
    {
        std::swap(own_, v.own_);
        std::swap(begin_, v.begin_);
        std::swap(end_, v.end_);
        std::swap(ptr_, v.ptr_);
    }
};

} /* namespace plusone */

#endif /* KSERGEY_linear_allocator_020118111334 */
