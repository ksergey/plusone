/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_static_buffer_131017131639
#define KSERGEY_static_buffer_131017131639

#include <plusone/buffer_base.hpp>

namespace plusone {

/** Staic buffer. */
class static_buffer
{
private:
    char* data_{nullptr};
    std::size_t capacity_{0};
    std::size_t size_{0};

public:
    static_buffer(const static_buffer&) = delete;
    static_buffer& operator=(const static_buffer&) = delete;

    /** Create a buffer using the provided storage area. */
    static_buffer(void* data, std::size_t size);

    /** @return Buffer capacity. */
    std::size_t capacity() const noexcept;

    /** @return Size of ready to consume data. */
    std::size_t size() const noexcept;

    /** @return Consume buffer. */
    const_buffer data() const noexcept;

    /** @overload */
    mutable_buffer data() noexcept;

    /** @return Buffer for filling data. */
    mutable_buffer prepare() noexcept;

    /**
     * @return Buffer for filling data size.
     * @throw std::length_error in case of not enought space.
     */
    mutable_buffer prepare(std::size_t size) noexcept;

    /** Make bytes available for consumtion */
    void commit(std::size_t size) noexcept;

    /**
     * Consume bytes from buffer.
     * @warning The method perform std::memmove in case of size != size()
     */
    void consume(std::size_t size) noexcept;

protected:
    /** Construct uninitialized. */
    static_buffer() = default;

    /** Reset the pointed to buffer. */
    void reset(void* data, std::size_t size) noexcept;
};

} /* namespace plusone */

#include <plusone/impl/static_buffer.ipp>

#endif /* KSERGEY_static_buffer_131017131639 */
