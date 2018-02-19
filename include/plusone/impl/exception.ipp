/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_exception_240717100244
#define KSERGEY_exception_240717100244

#include <type_traits>
#include <plusone/compiler.hpp>

namespace plusone {

__force_inline exception::exception(std::string text)
    : what_{text}
{}

__force_inline const char* exception::what() const noexcept
{
    return what_.c_str();
}

template< class Ex >
__force_inline void throw_ex()
{
    static_assert( std::is_convertible< Ex*, std::exception* >::value,
            "Exception must derive from std::exception" );

    throw Ex{};
}

template< class Ex, class... Args >
__force_inline void throw_ex(fmt::CStringRef format, Args&&... args)
{
    static_assert( std::is_convertible< Ex*, std::exception* >::value,
            "Exception must derive from std::exception" );

    throw Ex{fmt::format(format, std::forward< Args >(args)...)};
}

} /* namespace plusone */

#endif /* KSERGEY_exception_240717100244 */
