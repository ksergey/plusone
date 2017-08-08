/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_buffer_base_080817215944
#define KSERGEY_buffer_base_080817215944

#include <cstddef>

namespace plusone {

template< class CharT >
struct buffer
{
    CharT* data;
    std::size_t size;
};

/** Hold a buffer that can be modified */
using mutable_buffer = buffer< char >;

/** Hold a buffer that cannot be modified */
using const_buffer = buffer< const char >;

} /* namespace plusone */

#endif /* KSERGEY_buffer_base_080817215944 */
