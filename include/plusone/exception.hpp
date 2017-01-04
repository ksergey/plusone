/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_exception_051216171739
#define KSERGEY_exception_051216171739

#include <cstdio>
#include <string>
#include <sstream>
#include <memory>
#include <exception>
#include "compiler.hpp"

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

/** exception class */
class exception
    : public std::exception
{
private:
    std::string what_;

public:
    /** empty string constructor */
    exception() = default;

    /** printf-like message formatter */
    template< class... Args >
    explicit exception(const char* fmt, const Args&... args)
        : what_(detail::string_printf< 10 >(fmt, args...))
    {}

    /** destructor */
    virtual ~exception() noexcept = default;

    /** stream like message formatter */
    template< class T >
    __force_inline exception& operator<<(const T& value)
    {
        std::ostringstream os;
        os << value;
        what_ += os.str();
        return *this;
    }

    /** return exception description */
    virtual const char* what() const noexcept override
    { return what_.c_str(); }
};

/** make unique exception class */
template< class... Tags >
struct tagged_exception
    : exception
{
    using exception::exception;
};

} /* namespace plusone */

#endif /* KSERGEY_exception_051216171739 */
