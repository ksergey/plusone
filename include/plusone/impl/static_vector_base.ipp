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
class static_vector_base_iterator
    : std::iterator< std::random_access_iterator_tag, T >
{
private:
    using iterator = static_vector_base_iterator< T >;

    T* pos_{nullptr};

public:
    static_vector_base_iterator() = default;

    explicit static_vector_base_iterator(T* value)
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

template< class T >
inline static_vector_base< T >::static_vector_base(T* data, std::size_t size)
    : data_{data}
    , capacity_{size}
{}

template< class T >
inline static_vector_base< T >::~static_vector_base()
{
    clear();
}

template< class T >
inline auto static_vector_base< T >::operator[](std::size_t index) const noexcept
    -> const_reference
{
    return *(data_ + index);
}

template< class T >
auto static_vector_base< T >::operator[](std::size_t index) noexcept
    -> reference
{
    return *(data_ + index);
}

template< class T >
inline auto static_vector_base< T >::front() const noexcept
    -> const_reference
{
    return *data_;
}

template< class T >
inline auto static_vector_base< T >::front() noexcept
    -> reference
{
    return *data_;
}

template< class T >
inline auto static_vector_base< T >::back() const noexcept
    -> const_reference
{
    return *(data_ + size_ - 1);
}

template< class T >
inline auto static_vector_base< T >::back() noexcept
    -> reference
{
    return *(data_ + size_ - 1);
}

template< class T >
template< class... Args >
inline auto static_vector_base< T >::emplace_back(Args&&... args)
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

template< class T >
inline void static_vector_base< T >::clear()
{
    for (std::size_t index = 0; index < size_; ++index) {
        (data_ + index)->~T();
    }
    size_ = 0;
}

template< class T >
inline auto static_vector_base< T >::begin() noexcept
    -> iterator
{
    return iterator{data_};
}

template< class T >
inline auto static_vector_base< T >::end() noexcept
    -> iterator
{
    return iterator{data_ + size_};
}

template< class T >
inline auto static_vector_base< T >::begin() const noexcept
    -> const_iterator
{
    return const_iterator{data_};
}

template< class T >
inline auto static_vector_base< T >::end() const noexcept
    -> const_iterator
{
    return const_iterator{data_ + size_};
}

template< class T >
inline auto static_vector_base< T >::cbegin() const noexcept
    -> const_iterator
{
    return const_iterator{data_};
}

template< class T >
inline auto static_vector_base< T >::cend() const noexcept
    -> const_iterator
{

    return const_iterator{data_ + size_};
}

template< class T >
inline void static_vector_base< T >::reset(T* data, std::size_t size)
{
    data_ = data;
    capacity_ = size;
    size = 0;
}

template< class T >
inline void static_vector_base< T >::swap(static_vector_base& other) noexcept
{
    std::swap(data_, other.data_);
    std::swap(capacity_, other.capacity_);
    std::swap(size_, other.size_);
}

} /* namespace plusone */

#endif /* KSERGEY_static_vector_base_051117230618 */
