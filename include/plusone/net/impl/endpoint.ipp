/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_endpoint_120717235410
#define KSERGEY_endpoint_120717235410

namespace plusone {
namespace net {

__force_inline endpoint::endpoint(const std::string& addr, std::uint16_t port)
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

__force_inline bool endpoint::valid() const noexcept
{
    return domain() == AF_INET || domain() == AF_INET6;
}

__force_inline endpoint::operator bool() const noexcept
{
    return valid();
}

__force_inline bool endpoint::operator!() const noexcept
{
    return !valid();
}

__force_inline int endpoint::domain() const noexcept
{
    return data_.in.sa_family;
}

__force_inline const sockaddr* endpoint::data() const noexcept
{
    return &data_.in;
}

__force_inline sockaddr* endpoint::data() noexcept
{
    return &data_.in;
}

__force_inline std::size_t endpoint::size() const noexcept
{
    if (domain() == AF_INET) {
        return sizeof(data_.in4);
    }
    if (domain() == AF_INET6) {
        return sizeof(data_.in6);
    }
    return 0;
}

__force_inline std::string endpoint::str(char delim) const noexcept
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

} /* namespace net */
} /* namespace plusone */

#endif /* KSERGEY_endpoint_120717235410 */
