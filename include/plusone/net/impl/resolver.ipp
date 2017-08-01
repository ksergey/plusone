/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_resolver_130717115116
#define KSERGEY_resolver_130717115116

namespace plusone {
namespace net {

inline resolver::resolver(resolver&& r) noexcept
{
    std::swap(data_, r.data_);
}

inline resolver& resolver::operator=(resolver&& r) noexcept
{
    std::swap(data_, r.data_);
    return *this;
}

inline resolver::resolver(const protocol& p, const std::string& node, const std::string& service, int flags)
{
    resolv(p, node.c_str(), service.c_str(), flags);
}

inline resolver::resolver(const protocol& p, const std::string& node, int flags)
{
    std::size_t found = node.rfind(':');
    if (found != std::string::npos) {
        resolv(p, node.substr(0, found).c_str(), node.c_str() + found + 1, flags);
    } else {
        resolv(p, node.c_str(), nullptr, flags);
    }
}

inline resolver::~resolver() noexcept
{
    if (data_) {
        freeaddrinfo(data_);
    }
}

inline resolver::operator bool() const noexcept
{
    return data_ != nullptr;
}

inline resolver::const_iterator resolver::begin() const noexcept
{
    return const_iterator{data_};
}

inline resolver::const_iterator resolver::end() const noexcept
{
    return const_iterator{};
}

inline void resolver::resolv(const protocol& p, const char* node, const char* service, int flags)
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

} /* namespace net */
} /* namespace plusone */

#endif /* KSERGEY_resolver_130717115116 */
