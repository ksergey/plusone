/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_static_vector_base_051117225636
#define KSERGEY_static_vector_base_051117225636

#include <memory>

namespace plusone {
namespace detail {

/** Iterator implementation for static_vector */
template< class T >
class static_vector_iterator;

} /* namespace detail */

template< class T, class Allocator = std::allocator< T > >
class static_vector
{
private:
    Allocator allocator_;

    T* data_{nullptr};
    typename Allocator::size_type size_{0};
    typename Allocator::size_type capacity_{0};

public:
    using allocator_type = Allocator;

    using value_type = typename Allocator::value_type;
    using reference = typename Allocator::reference;
    using const_reference = typename Allocator::const_reference;
    using pointer = typename Allocator::pointer;
    using const_pointer = typename Allocator::const_pointer;
    using difference_type = typename Allocator::difference_type;
    using size_type = typename Allocator::size_type;

    using iterator = detail::static_vector_iterator< T >;
    using const_iterator = detail::static_vector_iterator< const T >;

    static_vector(const static_vector&) = delete;

    /** Construct vector without capacity. */
    explicit static_vector(const Allocator& alloc = Allocator());

    /** Construct vector with capacity. */
    explicit static_vector(size_type capacity, const Allocator& alloc = Allocator());

    static_vector(static_vector&& v) noexcept;
    static_vector& operator=(static_vector&& v) noexcept;

    /** Destructor. */
    virtual ~static_vector();

    /** @return Pointer to data begin. */
    const value_type* data() const noexcept
    {
        return data_;
    }

    /** @overload */
    value_type* data() noexcept
    {
        return data_;
    }

    /** @return Max elements count in vector. */
    size_type capacity() const noexcept
    {
        return capacity_;
    }

    /** @return Number of stored elements. */
    size_type size() const noexcept
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
    const_reference operator[](size_type index) const noexcept;

    /** @overload */
    reference operator[](size_type index) noexcept;

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

    /** Swap two vectors. */
    void swap(static_vector& v) noexcept;
};

} /* namespace plusone */

#include <plusone/impl/static_vector.ipp>

#endif /* KSERGEY_static_vector_base_051117225636 */
