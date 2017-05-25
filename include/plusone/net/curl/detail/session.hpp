/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_session_250517191958
#define KSERGEY_session_250517191958

#include <cstddef>
#include "../../../expect.hpp"
#include "../common.hpp"

namespace plusone {
namespace net {
namespace curl {
namespace detail {

class easy_session
{
private:
    CURL* session_;

public:
    easy_session(const easy_session&) = delete;
    easy_session& operator=(const easy_session&) = delete;

    easy_session(easy_session&& s) noexcept
    { std::swap(s.session_, session_); }

    easy_session& operator=(easy_session&& s)
    { std::swap(s.session_, session_); return *this; }

    easy_session(std::nullptr_t)
        : session_{nullptr}
    {}

    easy_session()
        : session_{curl_easy_init()}
    {
        if (__unlikely(!session_)) {
            throw curl::error{"Failed to init curl easy session"};
        }
    }

    ~easy_session() noexcept
    {
        if (session_) {
            curl_easy_cleanup(session_);
        }
    }

    __force_inline operator CURL*() noexcept
    { return session_; }

    template< class ValueT >
    __force_inline void set_option(CURLMoption option, ValueT value)
    {
        __expect( session_ );
        curl_easy_setopt(session_, option, value);
    }
};

class multi_session
{
private:
    CURLM* session_;

public:
    multi_session(const multi_session&) = delete;
    multi_session& operator=(const multi_session&) = delete;

    multi_session(multi_session&& s) noexcept
    { std::swap(s.session_, session_); }

    multi_session& operator=(multi_session&& s)
    { std::swap(s.session_, session_); return *this; }

    multi_session(std::nullptr_t)
        : session_{nullptr}
    {}

    multi_session()
        : session_{curl_multi_init()}
    {
        if (__unlikely(!session_)) {
            throw curl::error{"Failed to init curl multi session"};
        }
    }

    ~multi_session() noexcept
    {
        if (session_) {
            curl_multi_cleanup(session_);
        }
    }

    __force_inline operator CURLM*() noexcept
    { return session_; }

    template< class ValueT >
    __force_inline void set_option(CURLMoption option, ValueT value)
    {
        __expect( session_ );
        curl_multi_setopt(session_, option, value);
    }
};

} /* namespace detail */
} /* namespace curl */
} /* namespace net */
} /* namespace plusone */

#endif /* KSERGEY_session_250517191958 */
