/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_endpoint_v4_270417231611
#define KSERGEY_endpoint_v4_270417231611

#include <cstdint>
#include <plusone/net/address_v4.hpp>

namespace dispatch {

/** IPv4 endpoint */
class endpoint_v4 final
{
private:
    std::uint32_t ip_{0};
    std::uint16_t port_{0};

public:
    /** Default constructor */
    endpoint_v4() = default;

    /** Construct endpoint from native ip:port */
    __force_inline endpoint_v4(std::uint32_t ip, std::uint16_t port)
        : ip_{ip}
        , port_{port}
    {}

    /** Construct endpoint from string ip address and port */
    __force_inline endpoint_v4(const std::string& ip, std::uint16_t port)
        : endpoint_v4(plusone::net::address_v4::from_string(ip).to_ulong(), port)
    {}

    /** Return endpoint ip address */
    __force_inline std::uint32_t ip() const noexcept
    { return ip_; }

    /** Change ip address */
    __force_inline endpoint_v4& ip(std::uint32_t value) noexcept
    { ip_ = value; return *this; }

    /** Return endpoint port */
    __force_inline std::uint16_t port() const noexcept
    { return port_; }

    /** Change port */
    __force_inline endpoint_v4& port(std::uint16_t value) noexcept
    { port_ = value; return *this; }

    /** Compare operator */
    __force_inline bool operator==(const endpoint_v4& ep) const noexcept
    { return ep.port_ == port_ && ep.ip_ == ip_; }

    /** Format to string */
    __force_inline std::string to_string() const
    { return plusone::net::address_v4{ip_}.to_string() + ':' + std::to_string(port_); }
};

/** Formatting operator */
__force_inline std::ostream& operator<<(std::ostream& os, const endpoint_v4& value)
{ return os << plusone::net::address_v4{value.ip()}.to_string() << ':' << value.port(); }

} /* namespace dispatch */

#endif /* KSERGEY_endpoint_v4_270417231611 */
