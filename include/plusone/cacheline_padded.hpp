/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_cacheline_padded_190717003226
#define KSERGEY_cacheline_padded_190717003226

#include <type_traits>
#include <utility>
#include <plusone/compiler.hpp>

namespace plusone {

template< class T >
class cacheline_padded
{
private:
    static_assert( alignof(T) < false_sharing_range, "" );

    static constexpr std::size_t padding_size() noexcept
    {
        return false_sharing_range - (alignof(T) % false_sharing_range);
    }

    char padding0_[padding_size()];
    T data_;
    char padding1_[padding_size()];

public:
    template< class... Args >
    explicit cacheline_padded(Args&&... args)
        : data_(std::forward< Args >(args)...)
    {}

    const T* get() const noexcept
    {
        return &data_;
    }

    T* get() noexcept
    {
        return &data_;
    }

    const T* operator->() const noexcept
    {
        return get();
    }

    T* operator->() noexcept
    {
        return get();
    }

    const T& operator*() const noexcept
    {
        return *get();
    }

    T& operator*() noexcept
    {
        return *get();
    }
};

} /* namespace plusone */

#endif /* KSERGEY_cacheline_padded_190717003226 */
