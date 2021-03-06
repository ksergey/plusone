/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_result_020817020559
#define KSERGEY_result_020817020559

#include <plusone/compiler.hpp>

namespace plusone {
namespace net {
namespace ssl {

__force_inline error_code::error_code(unsigned long code) noexcept
    : code_{code}
{}

__force_inline unsigned long error_code::code() const noexcept
{
    return code_;
}

__force_inline const char* error_code::str() const noexcept
{
    const char* s = ::ERR_reason_error_string(code_);
    return s ? s : "plusone.net.ssl";
}

__force_inline bool error_code::again() const noexcept
{
    return code() == SSL_ERROR_WANT_READ || code() == SSL_ERROR_WANT_WRITE || code() == SSL_ERROR_NONE;
}

__force_inline bool error_code::disconnected() const noexcept
{
    return code() == SSL_ERROR_ZERO_RETURN;
}

__force_inline handshake_result::handshake_result(int value)
    : error_code{value == 1 ? 0ul : ::ERR_get_error()}
    , value_{value}
{}

__force_inline handshake_result::operator bool() const noexcept
{
    return value_ == 1;
}

__force_inline op_result::op_result(int value)
    : error_code{value > 0 ? 0ul : ::ERR_get_error()}
    , value_{value}
{}

__force_inline op_result::operator bool() const noexcept
{
    return value_ > 0;
}

__force_inline std::size_t op_result::bytes() const noexcept
{
    return static_cast< std::size_t >(value_);
}

} /* namespace ssl */
} /* namespace net */
} /* namespace plusone */

#endif /* KSERGEY_result_020817020559 */
