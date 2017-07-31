/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_ring_buffer_270717003431
#define KSERGEY_ring_buffer_270717003431

#include <plusone/math.hpp>
#include <plusone/compiler.hpp>

namespace plusone {

template< class T >
__force_inline ring_buffer< T >::ring_buffer(ring_buffer&& v) noexcept
{
    swap(v);
}

template< class T >
__force_inline ring_buffer< T >& ring_buffer< T >::operator=(ring_buffer&& v) noexcept
{
    if (__likely(this != &v)) {
        swap(v);
    }
    return *this;
}

template< class T >
__force_inline ring_buffer< T >::ring_buffer(std::size_t capacity)
    : capacity_{capacity}
{
    if (__unlikely(!is_power_of_two(capacity_))) {
        throw ring_buffer_error("Capacity must be power of two");
    }

    data_ = new storage_type[capacity_];
}

template< class T >
ring_buffer< T >::~ring_buffer() noexcept
{
    if (data_) {
        clear();
        delete[] data_;
    }
}

template< class T >
__force_inline void ring_buffer< T >::swap(ring_buffer& v) noexcept
{
    std::swap(v.data_, data_);
    std::swap(v.capacity_, capacity_);
    std::swap(v.size_, size_);
    std::swap(v.head_, head_);
    std::swap(v.tail_, tail_);
}

template< class T >
__force_inline std::size_t ring_buffer< T >::capacity() const noexcept
{
    return capacity_;
}

template< class T >
__force_inline std::size_t ring_buffer< T >::size() const noexcept
{
    return size_;
}

template< class T >
__force_inline bool ring_buffer< T >::empty() const noexcept
{
    return size_ == 0;
}

template< class T >
__force_inline bool ring_buffer< T >::full() const noexcept
{
    return size() == capacity();
}

template< class T >
__force_inline typename ring_buffer< T >::const_reference
ring_buffer< T >::operator[](std::size_t index) const noexcept
{
    return *reinterpret_cast< const_pointer >(data_ + ((head_ + index) & (capacity_ - 1)));
}

template< class T >
__force_inline typename ring_buffer< T >::reference
ring_buffer< T >::operator[](std::size_t index) noexcept
{
    return *reinterpret_cast< pointer >(data_ + ((head_ + index) & (capacity_ - 1)));
}

template< class T >
__force_inline typename ring_buffer< T >::const_reference
ring_buffer< T >::front() const noexcept
{
    return *reinterpret_cast< const_pointer >(data_ + (head_ & (capacity_ - 1)));
}

template< class T >
__force_inline typename ring_buffer< T >::reference
ring_buffer< T >::front() noexcept
{
    return *reinterpret_cast< pointer >(data_ + (head_ & (capacity_ - 1)));
}

template< class T >
__force_inline typename ring_buffer< T >::const_reference
ring_buffer< T >::back() const noexcept
{
    return *reinterpret_cast< const_pointer >(data_ + ((tail_ - 1) & (capacity_ - 1)));
}

template< class T >
__force_inline typename ring_buffer< T >::reference
ring_buffer< T >::back() noexcept
{
    return *reinterpret_cast< pointer >(data_ + ((tail_ - 1) & (capacity_ - 1)));
}

template< class T >
__force_inline void ring_buffer< T >::clear() noexcept
{
    for (std::size_t index = 0; index < size_; ++index) {
        reinterpret_cast< T* >(data_ + index)->~T();
    }
    size_ = head_ = tail_ = 0;
}

template< class T >
template< class... Args >
__force_inline typename ring_buffer< T >::reference
ring_buffer< T >::emplace_back(Args&&... args)
{
    if (size_ < capacity_) {
        new (data_ + size_) T(std::forward< Args >(args)...);
        ++size_;
        ++tail_;
    } else {
        const std::size_t idx = head_ & (capacity_ - 1);
        reinterpret_cast< T* >(data_ + idx)->~T();
        new (data_ + idx) T(std::forward< Args >(args)...);
        ++tail_;
        ++head_;
    }
    return back();
}

} /* namespace plusone */

#endif /* KSERGEY_ring_buffer_270717003431 */
