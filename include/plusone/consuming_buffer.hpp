/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_consuming_buffer_090817105048
#define KSERGEY_consuming_buffer_090817105048

#include <plusone/buffer_base.hpp>

namespace plusone {

/** Consuming only buffer */
class consuming_buffer
{
private:
    const char* data_;
    std::size_t size_;

public:
    consuming_buffer(const consuming_buffer&) = delete;
    consuming_buffer& operator=(const consuming_buffer&) = delete;

    consuming_buffer(const void* data, std::size_t size) noexcept;

    /** Get size of elapsed to consume data */
    std::size_t size() const noexcept;

    /** Return consume buffer */
    const_buffer data() const noexcept;

    /** Consume bytes from buffer */
    void consume(std::size_t size) noexcept;
};

} /* namespace plusone */

#include <plusone/impl/consuming_buffer.ipp>

#endif /* KSERGEY_consuming_buffer_090817105048 */
