/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_static_vector_base_051117230618
#define KSERGEY_static_vector_base_051117230618

#include <iterator>
#include <plusone/compiler.hpp>
#include <plusone/exception.hpp>

namespace plusone {
namespace detail {

template< class T >
class static_vector_iterator
    : std::iterator< std::random_access_iterator_tag, T >
{
private:
    using iterator = static_vector_iterator< T >;

    T* pos_{nullptr};

public:
    static_vector_iterator() = default;

    explicit static_vector_iterator(T* value)
        : pos_{value}
    {}

    iterator& operator++() noexcept
    {
        ++pos_;
        return *this;
    }

    iterator& operator--() noexcept
    {
        --pos_;
        return *this;
    }

    bool operator==(const iterator& it) const noexcept
    {
        return pos_ == it.pos_;
    }

    bool operator!=(const iterator& it) const noexcept
    {
        return pos_ != it.pos_;
    }

    const T& operator*() const noexcept
    {
        return *reinterpret_cast< const T* >(pos_);
    }

    T& operator*() noexcept
    {
        return *reinterpret_cast< T* >(pos_);
    }

    const T* operator->() const noexcept
    {
        return reinterpret_cast< const T* >(pos_);
    }

    T* operator->() noexcept
    {
        return reinterpret_cast< T* >(pos_);
    }
};

} /* namespace detail */

template< class T, class Allocator >
inline static_vector< T, Allocator >::static_vector(const Allocator& alloc)
    : allocator_{alloc}
{}

template< class T, class Allocator >
inline static_vector< T, Allocator >::static_vector(size_type capacity, const Allocator& alloc)
    : allocator_{alloc}
    , data_{allocator_.allocate(capacity)}
    , size_{0}
    , capacity_{capacity}
{
    if (!data_) {
        throw_ex< std::bad_alloc >();
    }
}

template< class T, class Allocator >
inline static_vector< T, Allocator >::static_vector(static_vector&& v) noexcept
{
    swap(v);
}

template< class T, class Allocator >
inline auto static_vector< T, Allocator >::operator=(static_vector&& v) noexcept
    -> static_vector&
{
    if (__likely(this != &v)) {
        swap(v);
    }
    return *this;
}

template< class T, class Allocator >
inline static_vector< T, Allocator >::~static_vector()
{
    clear();
    allocator_.deallocate(data_, capacity_);
}

template< class T, class Allocator >
inline auto static_vector< T, Allocator >::operator[](size_type index) const noexcept
    -> const_reference
{
    return *(data_ + index);
}

template< class T, class Allocator >
auto static_vector< T, Allocator >::operator[](size_type index) noexcept
    -> reference
{
    return *(data_ + index);
}

template< class T, class Allocator >
inline auto static_vector< T, Allocator >::front() const noexcept
    -> const_reference
{
    return *data_;
}

template< class T, class Allocator >
inline auto static_vector< T, Allocator >::front() noexcept
    -> reference
{
    return *data_;
}

template< class T, class Allocator >
inline auto static_vector< T, Allocator >::back() const noexcept
    -> const_reference
{
    return *(data_ + size_ - 1);
}

template< class T, class Allocator >
inline auto static_vector< T, Allocator >::back() noexcept
    -> reference
{
    return *(data_ + size_ - 1);
}

template< class T, class Allocator >
template< class... Args >
inline auto static_vector< T, Allocator >::emplace_back(Args&&... args)
    -> reference
{
    if (__unlikely(size_ < capacity_)) {
        new (data_ + size_) T(std::forward< Args >(args)...);
        ++size_;
    } else {
        throw_ex< std::bad_alloc >();
    }
    return back();
}

template< class T, class Allocator >
inline void static_vector< T, Allocator >::clear()
{
    for (size_type index = 0; index < size(); ++index) {
        (data_ + index)->~T();
    }
    size_ = 0;
}

template< class T, class Allocator >
inline auto static_vector< T, Allocator >::begin() noexcept
    -> iterator
{
    return iterator{data_};
}

template< class T, class Allocator >
inline auto static_vector< T, Allocator >::end() noexcept
    -> iterator
{
    return iterator{data_ + size_};
}

template< class T, class Allocator >
inline auto static_vector< T, Allocator >::begin() const noexcept
    -> const_iterator
{
    return const_iterator{data_};
}

template< class T, class Allocator >
inline auto static_vector< T, Allocator >::end() const noexcept
    -> const_iterator
{
    return const_iterator{data_ + size_};
}

template< class T, class Allocator >
inline auto static_vector< T, Allocator >::cbegin() const noexcept
    -> const_iterator
{
    return const_iterator{data_};
}

template< class T, class Allocator >
inline auto static_vector< T, Allocator >::cend() const noexcept
    -> const_iterator
{
    return const_iterator{data_ + size_};
}

template< class T, class Allocator >
inline void static_vector< T, Allocator >::swap(static_vector& v) noexcept
{
    std::swap(allocator_, v.allocator_);
    std::swap(data_, v.data_);
    std::swap(size_, v.size_);
    std::swap(capacity_, v.capacity_);
}

} /* namespace plusone */

#endif /* KSERGEY_static_vector_base_051117230618 */
