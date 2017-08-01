/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_error_010817231800
#define KSERGEY_error_010817231800

#include <openssl/err.h>
#include <plusone/exception.hpp>
#include <plusone/compiler.hpp>

namespace plusone {
namespace net {
namespace ssl {
namespace error {
namespace detail {

struct ssl_category
    : public std::error_category
{
    const char* name() const noexcept override
    {
        return "plusone.net.ssl";
    }

    std::string message(int condition) const override
    {
        const char* s = ::ERR_reason_error_string(condition);
        return s ? s : "plusone.net.ssl error";
    }
};

} /* namespace detail */


__force_inline const std::error_category& get_ssl_category()
{
    static detail::ssl_category instance;
    return instance;
}

} /* namespace error */

__force_inline void throw_error(int code)
{
    std::error_code ec{code, error::get_ssl_category()};
    throw_exception(ec);
}

} /* namespace ssl */
} /* namespace net */
} /* namespace plusone */

#endif /* KSERGEY_error_010817231800 */
