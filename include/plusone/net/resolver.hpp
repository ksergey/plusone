/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_resolver_061116025430_MADLIFE
#define MADLIFE_resolver_061116025430_MADLIFE

#include "protocol.hpp"
#include "detail/resolver_iterator.hpp"

namespace plusone {
namespace net {

/** hostname resolver */
class resolver
{
private:
    /* resolver data */
    addrinfo* data_{nullptr};

public:
    /** resolver iterator type */
    using const_iterator = detail::resolver_iterator;

    resolver(const resolver&) = delete;
    resolver& operator=(const resolver&) = delete;

    /** construct resolver from rvalue reference */
    __force_inline resolver(resolver&& r) noexcept
    { std::swap(data_, r.data_); }

    /** move operator */
    __force_inline resolver& operator=(resolver&& r) noexcept
    {
        std::swap(data_, r.data_);
        return *this;
    }

    /** default constructor */
    resolver() = default;

    /** construct resolver */
    resolver(const protocol& p, const std::string& node, const std::string& service, int flags = 0)
    { resolv(p, node.c_str(), service.c_str(), flags); }

    /** construct resolver */
    resolver(const protocol& p, const std::string& node, int flags = 0)
    {
        size_t found = node.rfind(':');
        if (found != std::string::npos) {
            resolv(p, node.substr(0, found).c_str(), node.c_str() + found + 1, flags);
        } else {
            resolv(p, node.c_str(), nullptr, flags);
        }
    }

    /** destructor */
    ~resolver()
    {
        if (data_) {
            freeaddrinfo(data_);
        }
    }

    /** return true if data resolver */
    __force_inline bool valid() const noexcept
    { return data_ != nullptr; }

    /** return true if data resolver */
    __force_inline explicit operator bool() const noexcept
    { return valid(); }

    /** return true if data not resolver */
    __force_inline bool operator!() const noexcept
    { return !valid(); }

    /** return iterator to begin */
    __force_inline const_iterator begin() const noexcept
    { return const_iterator(data_); }

    /** return iterator to past the end */
    __force_inline const_iterator end() const noexcept
    { return const_iterator(); }

private:
    void resolv(const protocol& p, const char* node, const char* service, int flags)
    {
        addrinfo hints = {};
        hints.ai_family = p.domain;
        hints.ai_socktype = p.type;
        hints.ai_flags = flags;
        hints.ai_protocol = p.proto;
        hints.ai_canonname = nullptr;
        hints.ai_addr = nullptr;
        hints.ai_next = nullptr;
        getaddrinfo(node, service, &hints, &data_);
    }
};

} /* namespace net */
} /* namespace plusone */

#endif /* MADLIFE_resolver_061116025430_MADLIFE */
