/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_endpoint_061116024156_MADLIFE
#define MADLIFE_endpoint_061116024156_MADLIFE

#include "common.hpp"

namespace plusone {
namespace net {

/** endpoint (address + port) */
class endpoint final
{
private:
    /* endpoint data */
    union {
        sockaddr in;
        sockaddr_in in4;
        sockaddr_in6 in6;
    } data_{};

public:
    /** default constructor */
    endpoint() = default;

    /** destructor */
    ~endpoint() = default;

    /** construct endpoint */
    endpoint(const std::string& addr, uint16_t port)
    {
        if (::inet_pton(AF_INET6, addr.c_str(), &data_.in6.sin6_addr) == 1) {
            data_.in6.sin6_family = AF_INET6;
            data_.in6.sin6_port = htons(port);
        } else if (::inet_pton(AF_INET, addr.c_str(), &data_.in4.sin_addr)) {
            data_.in4.sin_family = AF_INET;
            data_.in4.sin_port = htons(port);
        } else {
            data_.in.sa_family = AF_UNSPEC;
        }
    }

    /** return true if endpoint initialized */
    __force_inline bool valid() const noexcept
    { return domain() == AF_INET || domain() == AF_INET6; }

    /** return true if endpoint initialized */
    __force_inline explicit operator bool() const noexcept
    { return valid(); }

    /** return true if endpoint not initialized */
    __force_inline bool operator!() const noexcept
    { return !valid(); }

    /** endpoint domain */
    __force_inline int domain() const noexcept
    { return data_.in.sa_family; }

    /** endpoint data */
    __force_inline const sockaddr* data() const noexcept
    { return &data_.in; }

    /** endpoint data */
    __force_inline sockaddr* data() noexcept
    { return &data_.in; }

    /** endpoint data size */
    __force_inline size_t size() const noexcept
    {
        if (domain() == AF_INET) {
            return sizeof(data_.in4);
        }
        if (domain() == AF_INET6) {
            return sizeof(data_.in6);
        }
        return 0;
    }

    /** return endpoint as string */
    __force_inline std::string str(char delim = ' ') const noexcept
    {
        if (data_.in.sa_family == AF_INET) {
            char buffer[INET_ADDRSTRLEN]{};
            inet_ntop(AF_INET, &data_.in4.sin_addr, buffer, sizeof(buffer));
            return std::string(buffer) + delim + std::to_string(ntohs(data_.in4.sin_port));
        } else if (data_.in.sa_family == AF_INET6) {
            char buffer[INET6_ADDRSTRLEN]{};
            inet_ntop(AF_INET6, &data_.in6.sin6_addr, buffer, sizeof(buffer));
            return std::string(buffer) + delim + std::to_string(ntohs(data_.in6.sin6_port));
        } else {
            return std::string();
        }
    }
};

} /* namespace net */
} /* namespace plusone */

#endif /* MADLIFE_endpoint_061116024156_MADLIFE */
