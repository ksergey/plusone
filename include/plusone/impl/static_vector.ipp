/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_static_vector_120717223841
#define KSERGEY_static_vector_120717223841

#include <stdexcept>
#include <iterator>
#include "../compiler.hpp"

namespace plusone {
namespace detail {

/** Iterator implementation for static_vector */
template< class T >
class static_vector_iterator final
    : std::iterator< std::random_access_iterator_tag, T >
{
private:
    using storage_type = typename std::aligned_storage< sizeof(T), alignof(T) >::type;
    using iterator = static_vector_iterator< T >;

    storage_type* pos_{nullptr};

public:
    static_vector_iterator() = default;
    ~static_vector_iterator() = default;

    explicit static_vector_iterator(storage_type* value)
        : pos_{value}
    {}

    /** Increment operation */
    __force_inline iterator& operator++() noexcept
    { ++pos_; return *this; }

    /** Decrement operation */
    __force_inline iterator& operator--() noexcept
    { --pos_; return *this; }

    /** Equal operator */
    __force_inline bool operator==(const iterator& it) const noexcept
    { return pos_ == it.pos_; }

    /** Not equal operator */
    __force_inline bool operator!=(const iterator& it) const noexcept
    { return pos_ != it.pos_; }

    /** Operator derefernce */
    __force_inline const T& operator*() const noexcept
    { return *reinterpret_cast< const T* >(pos_); }

    /** Operator derefernce */
    __force_inline T& operator*() noexcept
    { return *reinterpret_cast< T* >(pos_); }

    /** Operator access member */
    __force_inline const T* operator->() const noexcept
    { return reinterpret_cast< const T* >(pos_); }

    /** Operator access member */
    __force_inline T* operator->() noexcept
    { return reinterpret_cast< T* >(pos_); }
};

} /* namespace detail */

template< class T >
__force_inline static_vector< T >::static_vector(static_vector&& v) noexcept
{
    swap(v);
}

template< class T >
__force_inline static_vector< T >& static_vector< T >::operator=(static_vector&& v) noexcept
{
    if (__likely(&v != this)) {
        swap(v);
    }
    return *this;
}

template< class T >
__force_inline static_vector< T >::static_vector(std::size_t capacity)
    : capacity_{capacity}
    , size_{0}
{
    data_ = new storage_type[capacity_];
}

template< class T >
__force_inline static_vector< T >::~static_vector()
{
    if (data_) {
        clear();
        delete[] data_;
    }
}

template< class T >
__force_inline std::size_t static_vector< T >::capacity() const noexcept
{
    return capacity_;
}

template< class T >
__force_inline std::size_t static_vector< T >::size() const noexcept
{
    return size_;
}

template< class T >
__force_inline bool static_vector< T >::empty() const noexcept
{
    return size_ == 0;
}

template< class T >
__force_inline bool static_vector< T >::full() const noexcept
{
    return size_ == capacity_;
}

template< class T >
__force_inline
typename static_vector< T >::const_reference
static_vector< T >::operator[](std::size_t index) const noexcept
{
    return *reinterpret_cast< const T* >(data_ + index);
}

template< class T >
__force_inline
typename static_vector< T >::reference
static_vector< T >::operator[](std::size_t index) noexcept
{
    return *reinterpret_cast< T* >(data_ + index);
}

template< class T >
__force_inline
typename static_vector< T >::const_reference
static_vector< T >::front() const noexcept
{
    return *reinterpret_cast< const T* >(data_);
}

template< class T >
__force_inline
typename static_vector< T >::reference
static_vector< T >::front() noexcept
{
    return *reinterpret_cast< T* >(data_);
}

template< class T >
__force_inline
typename static_vector< T >::const_reference
static_vector< T >::back() const noexcept
{
    return *reinterpret_cast< const T* >(data_ + size_ - 1);
}

template< class T >
__force_inline
typename static_vector< T >::reference
static_vector< T >::back() noexcept
{
    return *reinterpret_cast< T* >(data_ + size_ - 1);
}

template< class T >
template< class... Args >
__force_inline
typename static_vector< T >::reference
static_vector< T >::emplace_back(Args&&... args)
{
    if (__unlikely(size_ < capacity_)) {
        new (data_ + size_) T(std::forward< Args >(args)...);
        ++size_;
    } else {
        throw std::bad_alloc();
    }
    return back();
}

template< class T >
__force_inline void static_vector< T >::clear()
{
    for (std::size_t index = 0; index < size_; ++index) {
        reinterpret_cast< T* >(data_ + index)->~T();
    }
    size_ = 0;
}

template< class T >
__force_inline void static_vector< T >::swap(static_vector& other) noexcept
{
    std::swap(data_, other.data_);
    std::swap(capacity_, other.capacity_);
    std::swap(size_, other.size_);
}

template< class T >
__force_inline
typename static_vector< T >::iterator
static_vector< T >::begin() noexcept
{
    return iterator{data_};
}

template< class T >
__force_inline
typename static_vector< T >::iterator
static_vector< T >::end() noexcept
{
    return iterator{data_ + size_};
}

template< class T >
__force_inline
typename static_vector< T >::const_iterator
static_vector< T >::begin() const noexcept
{
    return const_iterator{data_};
}

template< class T >
__force_inline
typename static_vector< T >::const_iterator
static_vector< T >::end() const noexcept
{
    return const_iterator{data_ + size_};
}

template< class T >
__force_inline
typename static_vector< T >::const_iterator
static_vector< T >::cbegin() const noexcept
{
    return const_iterator{data_};
}

template< class T >
__force_inline
typename static_vector< T >::const_iterator
static_vector< T >::cend() const noexcept
{
    return const_iterator{data_ + size_};
}

} /* namespace plusone */

#endif /* KSERGEY_static_vector_120717223841 */
