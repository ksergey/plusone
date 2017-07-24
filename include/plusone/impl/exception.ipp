/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_exception_240717100244
#define KSERGEY_exception_240717100244

#include <plusone/fmt.hpp>
#include <plusone/compiler.hpp>

namespace plusone {

__force_inline exception::exception(std::string text)
    : what_{text}
{}

template< class... Args >
__force_inline exception::exception(const char* format, const Args&... args)
    : what_{fmt::format(format, args...)}
{}

__force_inline const char* exception::what() const noexcept
{
    return what_.c_str();
}

} /* namespace plusone */

#endif /* KSERGEY_exception_240717100244 */
