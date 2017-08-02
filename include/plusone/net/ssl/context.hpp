/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_context_010817181652
#define KSERGEY_context_010817181652

#include <openssl/ssl.h>
#include <plusone/net/ssl/context_base.hpp>
#include <plusone/net/ssl/init.hpp>

namespace plusone {
namespace net {
namespace ssl {

/** SSL context */
class context
    : public context_base
    , private init
{
private:
    SSL_CTX* ctx_{nullptr};

public:
    /** Native handle type */
    using native_handle_type = SSL_CTX*;

    /** Move constructor */
    context(context&& v) noexcept;

    /** Move operator */
    context& operator=(context&& v) noexcept;

    /**
     * Default constructor.
     * construct uninitialized context
     */
    context() = default;

    /**
     * Construct context
     * @param[in] method is connection method
     * @throw context::error in case of construct context failed
     */
    explicit context(method m);

    /** Destructor */
    ~context() noexcept;

    /** Check context is initialized */
    explicit operator bool() const noexcept;

    /** Get native handle */
    native_handle_type native_handle() noexcept;

    /** Swap two contexts */
    void swap(context& v) noexcept;
};

} /* namespace ssl */
} /* namespace net */
} /* namespace plusone */

#include <plusone/net/ssl/impl/context.ipp>

#endif /* KSERGEY_context_010817181652 */
