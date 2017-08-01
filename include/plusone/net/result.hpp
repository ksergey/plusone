/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_result_051116235729_MADLIFE
#define MADLIFE_result_051116235729_MADLIFE

#include <plusone/net/socket.hpp>

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
    socket_result(int code = 0);

    /** Return error code */
    int code() const noexcept;

    /** Return error code description */
    const char* str() const noexcept;

    /** Return true if error is EINTR */
    bool interrupted() const noexcept;

    /** Return true if error is EAGAIN or EWOULDBLOCK */
    bool again() const noexcept;
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
    op_result(int value = 0);

    /** Return true if operation successfully executed */
    explicit operator bool() const noexcept;
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
    io_result(ssize_t value = 0);

    /** Return true if operation successfully executed */
    explicit operator bool() const noexcept;

    /** Return read/write bytes count */
    std::size_t bytes() const noexcept;

    /** Return true if disconnect happend */
    bool is_disconnected() const noexcept;
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
    accept_result(int s = invalid_socket);

    /** Return true if operation successfully executed */
    explicit operator bool() const noexcept;

    /** Return accepted socket */
    socket&& get() noexcept;
};

} /* namespace net */
} /* namespace plusone */

#include <plusone/net/impl/result.ipp>

#endif /* MADLIFE_result_051116235729_MADLIFE */
