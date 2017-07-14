/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_resolver_061116025430_MADLIFE
#define MADLIFE_resolver_061116025430_MADLIFE

#include <string>
#include "protocol.hpp"
#include "detail/resolver_iterator.hpp"

namespace plusone {
namespace net {

/** Hostname resolver */
class resolver
{
private:
    /* Resolver data */
    addrinfo* data_{nullptr};

public:
    /** Resolver iterator type */
    using const_iterator = detail::resolver_iterator;

    /** Construct resolver from rvalue reference */
    resolver(resolver&& r) noexcept;

    /** Move operator */
    resolver& operator=(resolver&& r) noexcept;

    /** Default constructor */
    resolver() = default;

    /** Construct resolver */
    resolver(const protocol& p, const std::string& node, const std::string& service, int flags = 0);

    /** Construct resolver */
    resolver(const protocol& p, const std::string& node, int flags = 0);

    /** Destructor */
    virtual ~resolver() noexcept;

    /** Return true if data resolver */
    bool valid() const noexcept;

    /** Return true if data resolver */
    explicit operator bool() const noexcept;

    /** Return true if data not resolver */
    bool operator!() const noexcept;

    /** Return iterator to begin */
    const_iterator begin() const noexcept;

    /** Return iterator to past the end */
    const_iterator end() const noexcept;

private:
    void resolv(const protocol& p, const char* node, const char* service, int flags);
};

} /* namespace net */
} /* namespace plusone */

#include "impl/resolver.ipp"

#endif /* MADLIFE_resolver_061116025430_MADLIFE */
