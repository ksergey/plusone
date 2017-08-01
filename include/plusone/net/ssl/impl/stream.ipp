/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_stream_020817011241
#define KSERGEY_stream_020817011241

#include <plusone/compiler.hpp>
#include <plusone/net/common.hpp>

namespace plusone {
namespace net {
namespace ssl {

__force_inline stream::stream(context& ctx, socket_type socket)
    : socket_{std::move(socket)}
{
    if (__unlikely(!socket)) {
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

} /* namespace ssl */
} /* namespace net */
} /* namespace plusone */

#endif /* KSERGEY_stream_020817011241 */
