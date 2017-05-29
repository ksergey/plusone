/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_result_051116235729_MADLIFE
#define MADLIFE_result_051116235729_MADLIFE

#include "socket.hpp"

namespace plusone {
namespace net {

/** Socket result object */
class socket_result
{
private:
    /* Error code */
    int code_;

public:
    /** Construct object with error code */
    __force_inline socket_result(int code = 0)
        : code_(code)
    {}

    /** Return error code */
    __force_inline int code() const noexcept
    { return code_; }

    /** Return error code description */
    __force_inline const char* str() const noexcept
    { return ::strerror(code_); }

    /** Return true if error is EINTR */
    __force_inline bool interrupted() const noexcept
    { return code_ == EINTR; }

    /** Return true if error is EAGAIN or EWOULDBLOCK */
    __force_inline bool again() const noexcept
    { return code_ == EAGAIN || code_ == EWOULDBLOCK; }
};

/** Socket operation result */
class op_result final
    : public socket_result
{
private:
    /* Result holder */
    int value_;

public:
    /** Construct op_result */
    op_result(int value = 0)
        : socket_result(value != 0 ? errno : 0)
        , value_(value)
    {}

    /** Return true if operation finished successful */
    __force_inline bool success() const noexcept
    { return value_ == 0; }

    /** Return success() */
    __force_inline explicit operator bool() const noexcept
    { return success(); }

    /** Return !success() */
    __force_inline bool operator!() const noexcept
    { return !success(); }
};

/** Socket input/output operation result */
class io_result final
    : public socket_result
{
private:
    /* Result holder */
    ssize_t value_;

public:
    /** Construct io_result */
    io_result(ssize_t value = 0)
        : socket_result(value != 0 ? errno : 0)
        , value_(value)
    {}

    /** Return true if operation finished successful */
    __force_inline bool success() const noexcept
    { return value_ > 0; }

    /** Return success() */
    __force_inline explicit operator bool() const noexcept
    { return success(); }

    /** Return !success() */
    __force_inline bool operator!() const noexcept
    { return !success(); }

    /** Return read/write bytes count */
    __force_inline size_t bytes() const noexcept
    { return value_; }

    /** Return true if disconnect happend */
    __force_inline bool is_disconnected() const noexcept
    { return value_ == 0; }
};

/** Socket accept operation result */
class accept_result final
    : public socket_result
{
private:
    /* Accepted socket */
    socket sock_;

public:
    /** Construct accept_result */
    accept_result(int s = invalid_socket)
        : socket_result(s == invalid_socket ? errno : 0)
        , sock_(s)
    {}

    /** Return true if operation finished successful */
    __force_inline bool success() const noexcept
    { return sock_.valid(); }

    /** Return success() */
    __force_inline explicit operator bool() const noexcept
    { return success(); }

    /** Return !success() */
    __force_inline bool operator!() const noexcept
    { return !success(); }

    /** Return accepted socket */
    __force_inline socket&& get() noexcept
    { return std::move(sock_); }
};

} /* namespace net */
} /* namespace plusone */

#endif /* MADLIFE_result_051116235729_MADLIFE */
