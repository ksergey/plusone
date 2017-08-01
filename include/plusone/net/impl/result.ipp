/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_result_240717100816
#define KSERGEY_result_240717100816

namespace plusone {
namespace net {

__force_inline socket_result::socket_result(int code)
    : code_(code)
{}

__force_inline int socket_result::code() const noexcept
{
    return code_;
}

__force_inline const char* socket_result::str() const noexcept
{
    return ::strerror(code_);
}

__force_inline bool socket_result::interrupted() const noexcept
{
    return code_ == EINTR;
}

__force_inline bool socket_result::again() const noexcept
{
    return code_ == EAGAIN || code_ == EWOULDBLOCK;
}

__force_inline op_result::op_result(int value)
    : socket_result(value != 0 ? errno : 0)
    , value_(value)
{}

__force_inline op_result::operator bool() const noexcept
{
    return value_ == 0;
}

__force_inline io_result::io_result(ssize_t value)
    : socket_result(value != 0 ? errno : 0)
    , value_(value)
{}

__force_inline io_result::operator bool() const noexcept
{
    return value_ > 0;
}

__force_inline std::size_t io_result::bytes() const noexcept
{
    return value_;
}

__force_inline bool io_result::is_disconnected() const noexcept
{
    return value_ == 0;
}

__force_inline accept_result::accept_result(int s)
    : socket_result(s == invalid_socket ? errno : 0)
    , sock_(s)
{}

__force_inline accept_result::operator bool() const noexcept
{
    return sock_.operator bool();
}

__force_inline socket&& accept_result::get() noexcept
{
    return std::move(sock_);
}

} /* namespace net */
} /* namespace plusone */

#endif /* KSERGEY_result_240717100816 */
