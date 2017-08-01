/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_stream_010817181427
#define KSERGEY_stream_010817181427

#include <plusone/net/socket.hpp>
#include <plusone/net/ssl/context.hpp>

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
};

} /* namespace ssl */
} /* namespace net */
} /* namespace plusone */

#include <plusone/net/ssl/impl/stream.ipp>

#endif /* KSERGEY_stream_010817181427 */
