/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_exception_130717071552
#define KSERGEY_exception_130717071552

#include <cstdio>
#include <memory>
#include <sstream>
#include "../compiler.hpp"

namespace plusone {
namespace detail {

template< std::size_t STACK_BUFFER_SIZE = 128,  class... Args >
__force_inline std::string string_printf(const char* fmt, const Args&... args)
{
    char stack_buffer[STACK_BUFFER_SIZE];
    auto sz = std::snprintf(stack_buffer, sizeof(stack_buffer), fmt, args...);
    if (__likely(sz < int{sizeof(stack_buffer)})) {
        return std::string(stack_buffer, sz);
    }

    /* allocate buffer in heap (including space for '\0') */
    std::unique_ptr< char[] > heap_buffer(new char[sz + 1]);
    std::snprintf(heap_buffer.get(), sz + 1, fmt, args...);
    return std::string(heap_buffer.get(), sz);
}

} /* namespace detail */

template< class... Args >
__force_inline exception::exception(const char* fmt, const Args&... args)
    : what_(detail::string_printf< 128 >(fmt, args...))
{}

template< class T >
__force_inline exception& exception::operator<<(const T& value)
{
    std::ostringstream os;
    os << value;
    what_ += os.str();
    return *this;
}

__force_inline const char* exception::what() const noexcept
{
    return what_.c_str();
}

} /* namespace plusone */

#endif /* KSERGEY_exception_130717071552 */
