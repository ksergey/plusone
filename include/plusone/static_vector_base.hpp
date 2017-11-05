/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_static_vector_base_051117225636
#define KSERGEY_static_vector_base_051117225636

#include <cstddef>

namespace plusone {
namespace detail {

/** Iterator implementation for static_vector */
template< class T >
class static_vector_base_iterator;

} /* namespace detail */

template< class T >
class static_vector_base
{
private:
    T* data_{nullptr};
    std::size_t size_{0};
    std::size_t capacity_{0};

public:
    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;
    using iterator = detail::static_vector_base_iterator< T >;
    using const_iterator = detail::static_vector_base_iterator< const T >;

    static_vector_base(const static_vector_base&) = delete;
    static_vector_base& operator=(const static_vector_base&) = delete;

    /** Construct vector using provided storage area. */
    static_vector_base(T* data, std::size_t size);

    /** Destructor. */
    virtual ~static_vector_base();

    /** @return Pointer to data begin. */
    const T* data() const noexcept
    {
        return data_;
    }

    /** @overload */
    T* data() noexcept
    {
        return data_;
    }

    /** @return Max elements count in vector. */
    std::size_t capacity() const noexcept
    {
        return capacity_;
    }

    /** @return Number of stored elements. */
    std::size_t size() const noexcept
    {
        return size_;
    }

    /** @return True if vector empty. */
    bool empty() const noexcept
    {
        return size_ == 0;
    }

    /** @return True if vector full. */
    bool full() const noexcept
    {
        return size_ == capacity_;
    }

    /** @return Element at index. */
    const_reference operator[](std::size_t index) const noexcept;

    /** @overload */
    reference operator[](std::size_t index) noexcept;

    /** @return Front element. */
    const_reference front() const noexcept;

    /** @overload */
    reference front() noexcept;

    /** @return Back element. */
    const_reference back() const noexcept;

    /** @overload */
    reference back() noexcept;

    /** Push back a new element. */
    template< class... Args >
    reference emplace_back(Args&&... args);

    /**
     * Clear vector.
     * Call destructor for each element.
     */
    void clear();

    /** @return Iterator to begin element. */
    iterator begin() noexcept;

    /** @return Iterator to next after last element. */
    iterator end() noexcept;

    /** @overload */
    const_iterator begin() const noexcept;

    /** @overload */
    const_iterator end() const noexcept;

    /** @return Iterator to begin element. */
    const_iterator cbegin() const noexcept;

    /** @return Iterator to next after last element. */
    const_iterator cend() const noexcept;

protected:
    /** Construct uninitialized vector. */
    static_vector_base() = default;

    /** Reset the pointed-to buffer. */
    void reset(T* data, std::size_t size);

    /** Swap two vectors. */
    void swap(static_vector_base& other) noexcept;
};

} /* namespace plusone */

#include <plusone/impl/static_vector_base.ipp>

#endif /* KSERGEY_static_vector_base_051117225636 */
