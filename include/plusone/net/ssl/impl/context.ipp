/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_context_010817183049
#define KSERGEY_context_010817183049

#include <plusone/compiler.hpp>

namespace plusone {
namespace net {
namespace ssl {

__force_inline context::context(context&& v) noexcept
{
    swap(v);
}

__force_inline context& context::operator=(context&& v) noexcept
{
    swap(v);
    return *this;
}

__force_inline context::context(method m)
{
    switch (m) {
        case context::sslv23:
            break;
        case context::sslv23_client:
            break;
        case context::sslv23_server:
            break;
        default:
            break;
    }
}

__force_inline context::~context() noexcept
{
    if (ctx_) {
        ::SSL_CTX_free(ctx_);
    }
}

__force_inline context::operator bool() const noexcept
{
    return ctx_ != nullptr;
}

__force_inline context::native_handle_type context::native_handle() noexcept
{
    return ctx_;
}

__force_inline void context::swap(context& v) noexcept
{
    std::swap(v.ctx_, ctx_);
}

} /* namespace ssl */
} /* namespace net */
} /* namespace plusone */

#endif /* KSERGEY_context_010817183049 */
