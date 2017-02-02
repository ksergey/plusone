/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_static_vector_030217004526
#define KSERGEY_static_vector_030217004526

#include <stdexcept>
#include <type_traits>
#include "compiler.hpp"

namespace plusone {

/* Vector without reallocations and erasings */
template< class T >
class static_vector
{
private:
    using storage = typename std::aligned_storage< sizeof(T), alignof(T) >::type;

    storage* data_{nullptr};
    std::size_t capacity_{0};
    std::size_t size_{0};

public:
    static_vector(const static_vector&) = delete;
    static_vector& operator=(const static_vector&) = delete;

    static_vector() = default;

    /* Move constructor */
    static_vector(static_vector&& v)
    {
        std::swap(data_, v.data_);
        std::swap(capacity_, v.capacity_);
        std::swap(size_, v.size_);
    }

    /* Move operator */
    static_vector& operator=(static_vector&& v)
    {
        if (&v != this) {
            std::swap(data_, v.data_);
            std::swap(capacity_, v.capacity_);
            std::swap(size_, v.size_);
        }
        return *this;
    }

    /* Construct vector of specific capacity */
    explicit static_vector(std::size_t capacity)
        : capacity_{capacity}
        , size_{0}
    {
        data_ = new storage[capacity_];
    }

    ~static_vector()
    {
        if (data_) {
            clear();
            delete[] data_;
        }
    }

    /* Return vector capacity */
    std::size_t capacity() const noexcept
    { return capacity_; }

    /* Return vector size */
    std::size_t size() const noexcept
    { return size_; }

    /* Return element from vector at index */
    const T& operator[](std::size_t index) const
    { return *reinterpret_cast< const T* >(data_ + index); }

    /* Return element from vector at index */
    T& operator[](std::size_t index)
    { return *reinterpret_cast< T* >(data_ + index); }

    /* Return front element of vector */
    const T& front() const noexcept
    { return *reinterpret_cast< const T* >(data_); }

    /* Return front element of vector */
    T& front() noexcept
    { return *reinterpret_cast< T* >(data_); }

    /* Return back element of vector */
    const T& back() const noexcept
    { return *reinterpret_cast< const T* >(data_ + size_ - 1); }

    /* Return back element of vector */
    T& back() noexcept
    { return *reinterpret_cast< T* >(data_ + size_ - 1); }

    /* Push back new element */
    template< class... Args >
    T& emplace_back(Args&&... args)
    {
        if (__unlikely(size_ < capacity_)) {
            new (data_ + size_) T(std::forward< Args >(args)...);
            ++size_;
        } else {
            throw std::bad_alloc{};
        }
        return back();
    }

    /* Clear vector */
    void clear()
    {
        for (std::size_t index = 0; index < size_; ++index) {
            reinterpret_cast< T* >(data_ + index)->~T();
        }
        size_ = 0;
    }
};

} /* namespace plusone */

#endif /* KSERGEY_static_vector_030217004526 */
