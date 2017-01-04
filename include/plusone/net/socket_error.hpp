/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_socket_error_051116230809_MADLIFE
#define MADLIFE_socket_error_051116230809_MADLIFE

#include "common.hpp"

namespace plusone {
namespace net {

/** socket error object */
class socket_error
{
private:
    /* error code */
    int code_;

public:
    /** construct object with error code */
    socket_error(int code = 0)
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

} /* namespace net */
} /* namespace plusone */

#endif /* MADLIFE_socket_error_051116230809_MADLIFE */
