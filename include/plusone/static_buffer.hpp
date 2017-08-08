/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_static_buffer_080817230852
#define KSERGEY_static_buffer_080817230852

#include "buffer_base.hpp"

namespace plusone {

class static_buffer
{
private:
    char* data_{nullptr};
    std::size_t capacity_{0};
    std::size_t size_{0};

public:
    static_buffer(const static_buffer&) = delete;
    static_buffer& operator=(const static_buffer&) = delete;

    /** Create a buffer using the provided storage area */
    static_buffer(void* data, std::size_t size);

    /** Return maximum size of input/output sequence */
    std::size_t capacity() const noexcept;

    /** Return size of input sequnce */
    std::size_t size() const noexcept;

    /** Return input sequnce buffer */
    const_buffer data() const noexcept;

    /** Return output sequence buffer */
    mutable_buffer prepare() noexcept;

    /**
     * Return output sequence buffer
     * @throw std::length_error in case of not enought space
     */
    mutable_buffer prepare(std::size_t size) noexcept;

    /** Move bytes from the output sequence to the input */
    void commit(std::size_t size) noexcept;

    /** Remove bytes from the input sequence */
    void consume(std::size_t size) noexcept;
};

} /* namespace plusone */

#include <plusone/impl/static_buffer.ipp>

#endif /* KSERGEY_static_buffer_080817230852 */
