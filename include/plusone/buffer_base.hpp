/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_buffer_base_080817215944
#define KSERGEY_buffer_base_080817215944

#include <cstddef>

namespace plusone {

class mutable_buffer;
class const_buffer;

namespace detail {

void* buffer_cast_helper(const mutable_buffer& v);
const void* buffer_cast_helper(const const_buffer& v);
std::size_t buffer_size_helper(const mutable_buffer& v);
std::size_t buffer_size_helper(const const_buffer& v);

} /* namespace detail */

class mutable_buffer
{
private:
    void* data_{nullptr};
    std::size_t size_{0};

    friend void* plusone::detail::buffer_cast_helper(const mutable_buffer& v);
    friend std::size_t plusone::detail::buffer_size_helper(const mutable_buffer& v);

public:
    /** Construct an empty buffer */
    mutable_buffer() = default;

    /** Construct a buffer to represent a given memory range */
    mutable_buffer(void* data, std::size_t size);
};

class const_buffer
{
private:
    const void* data_{nullptr};
    std::size_t size_{0};

    friend const void* plusone::detail::buffer_cast_helper(const const_buffer& v);
    friend std::size_t plusone::detail::buffer_size_helper(const const_buffer& v);

public:
    /** Construct an empty buffer */
    const_buffer() = default;

    /** Construct a buffer to represent a given memory range */
    const_buffer(const void* data, std::size_t size);
};

template< class PointerT >
PointerT buffer_cast(const mutable_buffer& v);

template< class PointerT >
PointerT buffer_cast(const const_buffer& v);

template< class BufferT >
std::size_t buffer_size(const BufferT& v);

} /* namespace plusone */

#include <plusone/impl/buffer_base.ipp>

#endif /* KSERGEY_buffer_base_080817215944 */
