/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_fixed_size_memory_pool_241017171155
#define KSERGEY_fixed_size_memory_pool_241017171155

#include <cstddef>
#include <cstdint>
#include <algorithm>
#include <type_traits>
#include <plusone/detail/queue.hpp>
#include <plusone/expect.hpp>

namespace plusone {
namespace detail {

} /* namespace detail */

template< std::size_t Size >
class fixed_size_memory_pool
{
private:
    struct entry
    {
        entry* link{nullptr};
        std::uint8_t data[Size];
    };

    static_assert( std::is_standard_layout< entry >(), "Something goes wrong" );

    using storage_type = typename std::aligned_storage< sizeof(entry), alignof(entry) >::type;

    std::size_t capacity_{0};
    storage_type* storage_{nullptr};
    detail::queue< entry > queue_;

public:
    using pointer = void*;

    fixed_size_memory_pool(const fixed_size_memory_pool&) = delete;
    fixed_size_memory_pool& operator=(const fixed_size_memory_pool&) = delete;

    fixed_size_memory_pool() = default;

    /** Construct memory pool. */
    __force_inline explicit fixed_size_memory_pool(std::size_t pool_size)
        : capacity_{pool_size}
        , storage_{new storage_type[pool_size]}
    {
        __expect( pool_size > 0 );

        deallocate_all();
    }

    /** Destructor. */
    __force_inline ~fixed_size_memory_pool() noexcept
    {
        delete[] storage_;
    }

    /** @return Pool size. */
    __force_inline std::size_t pool_size() const noexcept
    {
        return capacity_;
    }

    /** Release all allocated objects. */
    __force_inline void deallocate_all() noexcept
    {
        while (queue_.pop()) {
            /* Do nothing. */
        }
        for (std::size_t i = 0; i < capacity_; ++i) {
            queue_.push(reinterpret_cast< entry* >(storage_ + i));
        }
    }

    /** Allocate block of size `Size`. */
    __force_inline pointer allocate() noexcept
    {
        auto e = queue_.pop();
        if (__likely(e)) {
            return e->data;
        } else {
            return nullptr;
        }
    }

    /** Deallocate early allocated data. */
    __force_inline void deallocate(pointer v) noexcept
    {
        auto e = reinterpret_cast< entry* >(reinterpret_cast< std::uint8_t* >(v) - offsetof(entry, data));
        queue_.push(e);
    }
};

} /* namespace plusone */

#endif /* KSERGEY_fixed_size_memory_pool_241017171155 */
