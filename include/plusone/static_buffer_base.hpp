/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_static_buffer_base_080817231547
#define KSERGEY_static_buffer_base_080817231547

#include "buffer_base.hpp"

namespace plusone {

/** Staic buffer */
class static_buffer_base
{
private:
    char* data_{nullptr};
    std::size_t capacity_{0};
    std::size_t size_{0};

public:
    static_buffer_base(const static_buffer_base&) = delete;
    static_buffer_base& operator=(const static_buffer_base&) = delete;

    /** Create a buffer using the provided storage area */
    static_buffer_base(void* data, std::size_t size);

    /** Return buffer capacity */
    std::size_t capacity() const noexcept;

    /** Return size of ready to consume data */
    std::size_t size() const noexcept;

    /** Return consume buffer */
    const_buffer data() const noexcept;

    /** Return buffer for filling data */
    mutable_buffer prepare() noexcept;

    /**
     * Return buffer for filling data size
     * @throw std::length_error in case of not enought space
     */
    mutable_buffer prepare(std::size_t size) noexcept;

    /** Make bytes available for consumtion */
    void commit(std::size_t size) noexcept;

    /**
     * Consume bytes from buffer
     * @warning The method perform std::memmove in case of size != size()
     */
    void consume(std::size_t size) noexcept;

protected:
    /** Construct uninitialized */
    static_buffer_base() = default;

    /** Reset the pointed to buffer */
    void reset(void* data, std::size_t size) noexcept;
};

} /* namespace plusone */

#include <plusone/impl/static_buffer_base.ipp>

#endif /* KSERGEY_static_buffer_base_080817231547 */
