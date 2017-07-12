/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_static_vector_030217004526
#define KSERGEY_static_vector_030217004526

#include <type_traits>

namespace plusone {
namespace detail {

/** Iterator implementation for static_vector */
template< class T >
class static_vector_iterator;

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
    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;

    /** Iterator type */
    using iterator = detail::static_vector_iterator< T >;

    /** Const iterator type */
    using const_iterator = detail::static_vector_iterator< const T >;

    static_vector(const static_vector&) = delete;
    static_vector& operator=(const static_vector&) = delete;

    /** Default constructor */
    static_vector() = default;

    /** Move constructor */
    static_vector(static_vector&& v) noexcept;

    /** Move operator */
    static_vector& operator=(static_vector&& v) noexcept;

    /** Construct vector of specific capacity */
    explicit static_vector(std::size_t capacity);

    /** Destructor */
    virtual ~static_vector();

    /** Return vector capacity */
    std::size_t capacity() const noexcept;

    /** Return vector size */
    std::size_t size() const noexcept;

    /** Return true if vector empty */
    bool empty() const noexcept;

    /** Return true if vector full */
    bool full() const noexcept;

    /** Return element from vector at index */
    const_reference operator[](std::size_t index) const noexcept;

    /** Return element from vector at index */
    reference operator[](std::size_t index) noexcept;

    /** Return front element of vector */
    const_reference front() const noexcept;

    /** Return front element of vector */
    reference front() noexcept;

    /** Return back element of vector */
    const_reference back() const noexcept;

    /** Return back element of vector */
    reference back() noexcept;

    /** Push back a new element */
    template< class... Args >
    reference emplace_back(Args&&... args);

    /** Clear vector */
    void clear();

    /** Swap content with another vector */
    void swap(static_vector& other) noexcept;

    /** Return iterator to begin element */
    iterator begin() noexcept;

    /** Return iterator to next after last element */
    iterator end() noexcept;

    /** Return const iterator to begin element */
    const_iterator begin() const noexcept;

    /** Return const iterator to next after last element */
    const_iterator end() const noexcept;

    /** Return const iterator to begin element */
    const_iterator cbegin() const noexcept;

    /** Return const iterator to next after last element */
    const_iterator cend() const noexcept;
};

} /* namespace plusone */

#include "impl/static_vector.ipp"

#endif /* KSERGEY_static_vector_030217004526 */
