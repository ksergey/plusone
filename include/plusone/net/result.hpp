/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_result_051116235729_MADLIFE
#define MADLIFE_result_051116235729_MADLIFE

#include "socket.hpp"

namespace plusone {
namespace net {

/** socket result object */
class socket_result
{
private:
    /* error code */
    int code_;

public:
    /** construct object with error code */
    socket_result(int code = 0)
        : code_(code)
    {}

    /** return error code */
    int code() const noexcept
    { return code_; }

    /** return error code description */
    const char* str() const noexcept
    { return ::strerror(code_); }

    /** return true if error is EINTR */
    bool is_interrupted() const noexcept
    { return code_ == EINTR; }

    /** return true if error is EAGAIN or EWOULDBLOCK */
    bool is_again() const noexcept
    { return code_ == EAGAIN || code_ == EWOULDBLOCK; }
};

/** socket operation result */
class op_result final
    : public socket_result
{
private:
    /* result holder */
    int value_;

public:
    /** construct op_result */
    op_result(int value = 0)
        : socket_result(value != 0 ? errno : 0)
        , value_(value)
    {}

    /** return true if operation finished successful */
    __force_inline bool success() const noexcept
    { return value_ == 0; }

    /** return success() */
    __force_inline explicit operator bool() const noexcept
    { return success(); }

    /** return !success() */
    __force_inline bool operator!() const noexcept
    { return !success(); }
};


/** socket input/output operation result */
class io_result final
    : public socket_result
{
private:
    /* result holder */
    ssize_t value_;

public:
    /** construct io_result */
    io_result(ssize_t value = 0)
        : socket_result(value != 0 ? errno : 0)
        , value_(value)
    {}

    /** return true if operation finished successful */
    __force_inline bool success() const noexcept
    { return value_ > 0; }

    /** return success() */
    __force_inline explicit operator bool() const noexcept
    { return success(); }

    /** return !success() */
    __force_inline bool operator!() const noexcept
    { return !success(); }

    /** return read/write bytes count */
    __force_inline size_t bytes() const noexcept
    { return value_; }

    /** return true if disconnect happend */
    __force_inline bool is_disconnected() const noexcept
    { return value_ == 0; }
};

/** socket accept operation result */
class accept_result final
    : public socket_result
{
private:
    /* accepted socket */
    socket sock_;

public:
    /** construct accept_result */
    accept_result(sock_t s = invalid_socket)
        : socket_result(s == invalid_socket ? errno : 0)
        , sock_(s)
    {}

    /** return true if operation finished successful */
    __force_inline bool success() const noexcept
    { return sock_.valid(); }

    /** return success() */
    __force_inline explicit operator bool() const noexcept
    { return success(); }

    /** return !success() */
    __force_inline bool operator!() const noexcept
    { return !success(); }

    /** return accepted socket */
    __force_inline socket&& get() noexcept
    { return std::move(sock_); }
};

} /* namespace net */
} /* namespace plusone */

#endif /* MADLIFE_result_051116235729_MADLIFE */
