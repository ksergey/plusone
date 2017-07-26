/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_ring_buffer_270717003258
#define KSERGEY_ring_buffer_270717003258

#include <type_traits>
#include <plusone/exception.hpp>

namespace plusone {

using ring_buffer_error = plusone::tagged_exception< struct ring_buffer_tag >;

/**
 * Ring buffer implementation
 * Limitation:
 *  no inserts
 *  no delets
 */
template< class T >
class ring_buffer
{
private:
    using storage_type = typename std::aligned_storage< sizeof(T), alignof(T) >::type;

    storage_type* data_{nullptr};
    std::size_t capacity_{0};
    std::size_t size_{0};
    std::size_t head_{0};
    std::size_t tail_{0};

public:
    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    /** Default constructor */
    ring_buffer() = default;

    /** Move constructor */
    ring_buffer(ring_buffer&& v) noexcept;

    /** Move operator */
    ring_buffer& operator=(ring_buffer&& v) noexcept;

    /**
     * Construct ring buffer with capacity
     * capacity should be power of 2
     */
    explicit ring_buffer(std::size_t capacity);

    /** Destructor */
    ~ring_buffer() noexcept;

    /** Swap two ring buffers */
    void swap(ring_buffer& v) noexcept;

    /** Return buffer capacity */
    std::size_t capacity() const noexcept;

    /** Return buffer size */
    std::size_t size() const noexcept;

    /** Return true if buffer empty */
    bool empty() const noexcept;

    /** Return true if buffer full */
    bool full() const noexcept;

    /** Return an element by index */
    const_reference operator[](std::size_t index) const noexcept;

    /** Return an element by index */
    reference operator[](std::size_t index) noexcept;

    /** Return front element of buffer */
    const_reference front() const noexcept;

    /** Return front element of buffer */
    reference front() noexcept;

    /** Return back element of buffer */
    const_reference back() const noexcept;

    /** Return back element of buffer */
    reference back() noexcept;

    /** Clear buffer */
    void clear() noexcept;

    /** Emplace back an new element */
    template< class... Args >
    reference emplace_back(Args&&... args);
};

} /* namespace plusone */

#include <plusone/impl/ring_buffer.ipp>

#endif /* KSERGEY_ring_buffer_270717003258 */
