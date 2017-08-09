/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_stream_010817181427
#define KSERGEY_stream_010817181427

#include <plusone/buffer_base.hpp>
#include <plusone/net/socket.hpp>
#include <plusone/net/ssl/context.hpp>
#include <plusone/net/ssl/result.hpp>

namespace plusone {
namespace net {
namespace ssl {

/** SSL stream */
class stream
{
private:
    plusone::net::socket socket_;
    SSL* handle_{nullptr};

public:
    using native_handle_type = SSL*;
    using socket_type = plusone::net::socket;

    /**
     * Default constructor.
     * create uninitialized stream
     */
    stream() = default;

    /** Construct stream stream */
    stream(context& ctx, socket_type socket);

    /** Destructor */
    ~stream() noexcept;

    /** Move constructor */
    stream(stream&& v) noexcept;

    /** Move operator */
    stream& operator=(stream&& v) noexcept;

    /** Get native handle */
    native_handle_type native_handle() noexcept;

    /** Get underlying socket */
    socket_type& socket() noexcept;

    /** Return true if stream initialized */
    explicit operator bool() const noexcept;

    /** Swap two streams */
    void swap(stream& v) noexcept;

    /** Shutdown socket */
    void close() noexcept;

    /** Do SSL handshake */
    handshake_result handshake() noexcept;

    /** Do write to stream */
    op_result send(const void* buf, std::size_t len) noexcept;

    /** Do write to stream */
    op_result send(const const_buffer& buf);

    /** Do read from stream */
    op_result recv(void* buf, std::size_t len) noexcept;

    /** Do read from stream */
    op_result recv(const mutable_buffer& buf);
};

} /* namespace ssl */
} /* namespace net */
} /* namespace plusone */

#include <plusone/net/ssl/impl/stream.ipp>

#endif /* KSERGEY_stream_010817181427 */
