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

template< class... Args >
__force_inline exception::exception(const char* fmt, const Args&... args)
{
    fmt::MemoryWriter writer;
    writer.write(fmt, args...);
    what_ = writer.str();
}

__force_inline const char* exception::what() const noexcept
{
    return what_.c_str();
}

} /* namespace plusone */

#endif /* KSERGEY_exception_130717071552 */
