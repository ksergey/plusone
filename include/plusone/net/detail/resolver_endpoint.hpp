/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_resolver_endpoint_061116025513_MADLIFE
#define MADLIFE_resolver_endpoint_061116025513_MADLIFE

#include <string>
#include <cstdint>
#include "../common.hpp"

namespace plusone {
namespace net {
namespace detail {

class resolver_endpoint final
{
private:
    /* resolver endpoint data */
    const addrinfo* data_{nullptr};

    friend class resolver_iterator;

public:
    /** default constructor */
    resolver_endpoint() = default;

    /** destructor */
    ~resolver_endpoint() = default;

    /** construct resolver endpoint */
    explicit resolver_endpoint(const addrinfo* data)
        : data_(data)
    {}

    /** return true if endpoint is initialized */
    __force_inline bool valid() const noexcept
    { return data_ != nullptr; }

    /** return true if endpoint is initialized */
    __force_inline explicit operator bool() const noexcept
    { return valid(); }

    /** return true if endpoint is not initialized */
    __force_inline bool operator!() const noexcept
    { return !valid(); }

    /** return socket protocol family */
    __force_inline int domain() const noexcept
    { return data_->ai_family; }

    /** return socket type */
    __force_inline int type() const noexcept
    { return data_->ai_socktype; }

    /** return socket protocol */
    __force_inline int proto() const noexcept
    { return data_->ai_protocol; }

    /** return endpoint data */
    __force_inline const sockaddr* data() const noexcept
    { return data_->ai_addr; }

    /** return endpoint data size */
    __force_inline size_t size() const noexcept
    { return data_->ai_addrlen; }

    /** return endpoint as string */
    __force_inline std::string str(char delim = ' ') const noexcept
    {
        const struct in_addr* address;
        uint16_t port;
        if (data_->ai_family == AF_INET) {
            const sockaddr_in* ipv4 = reinterpret_cast< const sockaddr_in* >(data());
            address = &(ipv4->sin_addr);
            port = ipv4->sin_port;
        } else if (data_->ai_family == AF_INET6) {
            const sockaddr_in6* ipv6 = reinterpret_cast< const sockaddr_in6* >(data());
            address = reinterpret_cast< const in_addr* >(&(ipv6->sin6_addr));
            port = ipv6->sin6_port;
        } else {
            return std::string();
        }

        char buffer[INET6_ADDRSTRLEN]{};

        inet_ntop(data_->ai_family, address, buffer, sizeof(buffer));
        if (port > 0) {
            return std::string(buffer) + delim + std::to_string(ntohs(port));
        } else {
            return buffer;
        }
    }
};

} /* namespace detail */
} /* namespace net */
} /* namespace plusone */

#endif /* MADLIFE_resolver_endpoint_061116025513_MADLIFE */
