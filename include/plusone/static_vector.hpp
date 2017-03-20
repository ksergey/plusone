/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_static_vector_030217004526
#define KSERGEY_static_vector_030217004526

#include <stdexcept>
#include <type_traits>
#include <iterator>
#include "compiler.hpp"

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

/** Vector without reallocations and erasings */
template< class T >
class static_vector
{
private:
    using storage_type = typename std::aligned_storage< sizeof(T), alignof(T) >::type;

    storage_type* data_{nullptr};
    std::size_t capacity_{0};
    std::size_t size_{0};

public:
    /** Iterator type */
    using iterator = detail::static_vector_iterator< T >;
    /** Const iterator type */
    using const_iterator = detail::static_vector_iterator< const T >;

    static_vector(const static_vector&) = delete;
    static_vector& operator=(const static_vector&) = delete;

    /** Default constructor */
    static_vector() = default;

    /** Move constructor */
    __force_inline static_vector(static_vector&& v)
    { swap(v); }

    /** Move operator */
    __force_inline static_vector& operator=(static_vector&& v)
    {
        if (__likely(&v != this)) {
            swap(v);
        }
        return *this;
    }

    /** Construct vector of specific capacity */
    explicit static_vector(std::size_t capacity)
        : capacity_{capacity}
        , size_{0}
    { data_ = new storage_type[capacity_]; }

    /** Destructor */
    __force_inline ~static_vector()
    {
        if (data_) {
            clear();
            delete[] data_;
        }
    }

    /** Return vector capacity */
    __force_inline std::size_t capacity() const noexcept
    { return capacity_; }

    /** Return vector size */
    __force_inline std::size_t size() const noexcept
    { return size_; }

    /** Return element from vector at index */
    __force_inline const T& operator[](std::size_t index) const noexcept
    { return *reinterpret_cast< const T* >(data_ + index); }

    /** Return element from vector at index */
    __force_inline T& operator[](std::size_t index) noexcept
    { return *reinterpret_cast< T* >(data_ + index); }

    /** Return front element of vector */
    __force_inline const T& front() const noexcept
    { return *reinterpret_cast< const T* >(data_); }

    /** Return front element of vector */
    __force_inline T& front() noexcept
    { return *reinterpret_cast< T* >(data_); }

    /** Return back element of vector */
    __force_inline const T& back() const noexcept
    { return *reinterpret_cast< const T* >(data_ + size_ - 1); }

    /** Return back element of vector */
    __force_inline T& back() noexcept
    { return *reinterpret_cast< T* >(data_ + size_ - 1); }

    /** Push back a new element */
    template< class... Args >
    __force_inline T& emplace_back(Args&&... args)
    {
        if (__unlikely(size_ < capacity_)) {
            new (data_ + size_) T(std::forward< Args >(args)...);
            ++size_;
        } else {
            throw std::bad_alloc{};
        }
        return back();
    }

    /** Clear vector */
    __force_inline void clear()
    {
        for (std::size_t index = 0; index < size_; ++index) {
            reinterpret_cast< T* >(data_ + index)->~T();
        }
        size_ = 0;
    }

    /** Swap content with another vector */
    __force_inline void swap(static_vector& other)
    {
        std::swap(data_, other.data_);
        std::swap(capacity_, other.capacity_);
        std::swap(size_, other.size_);
    }

    /** Return iterator to begin element */
    __force_inline iterator begin() noexcept
    { return iterator{data_}; }

    /** Return iterator to next after last element */
    __force_inline iterator end() noexcept
    { return iterator{data_ + size_}; }

    /** Return const iterator to begin element */
    __force_inline const_iterator begin() const noexcept
    { return const_iterator{data_}; }

    /** Return const iterator to next after last element */
    __force_inline const_iterator end() const noexcept
    { return const_iterator{data_ + size_}; }

    /** Return const iterator to begin element */
    __force_inline const_iterator cbegin() const noexcept
    { return const_iterator{data_}; }

    /** Return const iterator to next after last element */
    __force_inline const_iterator cend() const noexcept
    { return const_iterator{data_ + size_}; }
};

} /* namespace plusone */

#endif /* KSERGEY_static_vector_030217004526 */
