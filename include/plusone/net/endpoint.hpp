/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_endpoint_061116024156_MADLIFE
#define MADLIFE_endpoint_061116024156_MADLIFE

#include "common.hpp"

namespace plusone {
namespace net {

/** Endpoint (address + port) */
class endpoint final
{
private:
    /* Endpoint data */
    union {
        sockaddr in;
        sockaddr_in in4;
        sockaddr_in6 in6;
    } data_{};

public:
    /** Default constructor */
    endpoint() = default;

    /** Destructor */
    ~endpoint() = default;

    /** Construct endpoint */
    endpoint(const std::string& addr, std::uint16_t port);

    /** Return true if endpoint initialized */
    bool valid() const noexcept;

    /** Return true if endpoint initialized */
    explicit operator bool() const noexcept;

    /** Return true if endpoint not initialized */
    bool operator!() const noexcept;

    /** Endpoint domain */
    int domain() const noexcept;

    /** Endpoint data */
    const sockaddr* data() const noexcept;

    /** Endpoint data */
    sockaddr* data() noexcept;

    /** Endpoint data size */
    std::size_t size() const noexcept;

    /** Return endpoint as string */
    std::string str(char delim = ' ') const noexcept;
};

} /* namespace net */
} /* namespace plusone */

#include "impl/endpoint.ipp"

#endif /* MADLIFE_endpoint_061116024156_MADLIFE */
