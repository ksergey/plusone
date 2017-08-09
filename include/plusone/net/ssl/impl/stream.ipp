/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_stream_020817011241
#define KSERGEY_stream_020817011241

#include <plusone/compiler.hpp>
#include <plusone/expect.hpp>
#include <plusone/net/common.hpp>

namespace plusone {
namespace net {
namespace ssl {

__force_inline stream::stream(context& ctx, socket_type socket)
    : socket_{std::move(socket)}
{
    if (__unlikely(!socket_)) {
        throw error("SSL stream create error (Invalid socket)");
    }

    handle_ = ::SSL_new(ctx.native_handle());
    if (__unlikely(!handle_)) {
        throw error("SSL stream create error ({})", ::ERR_reason_error_string(::ERR_get_error()));
    }

    auto rc = SSL_set_fd(handle_, socket_.native_handle());
    if (__unlikely(rc == 0)) {
        auto code = ::ERR_get_error();
        ::SSL_free(handle_);
        throw error("SSL stream create error [set fd] ({})", ::ERR_reason_error_string(code));
    }

    /* Imitating the behaviour of write() */
    ::SSL_set_mode(handle_, SSL_MODE_ENABLE_PARTIAL_WRITE);
    ::SSL_set_mode(handle_, SSL_MODE_ACCEPT_MOVING_WRITE_BUFFER);
}

__force_inline stream::~stream() noexcept
{
    close();
}

__force_inline stream::stream(stream&& v) noexcept
{
    swap(v);
}

__force_inline stream& stream::operator=(stream&& v) noexcept
{
    swap(v);
    return *this;
}

__force_inline stream::native_handle_type stream::native_handle() noexcept
{
    return handle_;
}

__force_inline stream::socket_type& stream::socket() noexcept
{
    return socket_;
}

__force_inline stream::operator bool() const noexcept
{
    return handle_ != nullptr;
}

__force_inline void stream::swap(stream& v) noexcept
{
    std::swap(v.socket_, socket_);
    std::swap(v.handle_, handle_);
}

__force_inline void stream::close() noexcept
{
    if (handle_) {
        ::SSL_shutdown(handle_);
        ::SSL_free(handle_);
        handle_ = nullptr;
    }
}

__force_inline handshake_result stream::handshake() noexcept
{
    __expect( native_handle() );
    return ::SSL_connect(native_handle());
}

__force_inline op_result stream::send(const void* buf, std::size_t len) noexcept
{
    __expect( native_handle() );
    return ::SSL_write(native_handle(), buf, len);
}

__force_inline op_result stream::send(const const_buffer& buf)
{
    return send(buffer_cast< const void* >(buf), buffer_size(buf));
}

__force_inline op_result stream::recv(void* buf, std::size_t len) noexcept
{
    __expect( native_handle() );
    return ::SSL_read(native_handle(), buf, len);
}

__force_inline op_result stream::recv(mutable_buffer& buf)
{
    return recv(buffer_cast< void* >(buf), buffer_size(buf));
}

} /* namespace ssl */
} /* namespace net */
} /* namespace plusone */

#endif /* KSERGEY_stream_020817011241 */
