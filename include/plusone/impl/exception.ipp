/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_exception_130717071552
#define KSERGEY_exception_130717071552

#include <cstdio>
#include <memory>
#include <sstream>
#include "../fmt.hpp"
#include "../compiler.hpp"

namespace plusone {

__force_inline exception::exception(std::string text)
    : what_{text}
{}

template< class... Args >
exception::exception(const char* format, const Args&... args)
    : what_{fmt::format(format, args...)}
{}

__force_inline const char* exception::what() const noexcept
{
    return what_.c_str();
}

} /* namespace plusone */

#endif /* KSERGEY_exception_130717071552 */
