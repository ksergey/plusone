/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_tcp_010817124434
#define KSERGEY_tcp_010817124434

#include <plusone/net/resolver.hpp>
#include <plusone/net/socket_options.hpp>

namespace plusone {
namespace net {
namespace detail {

/**
 * Connect socket to address from resolver
 * @return socket
 * @throw socket_error in case of socket create error
 * socket will valid if connect was successful
 */
__force_inline socket connect(const resolver& r)
{
    for (auto& ep: r) {
        socket s = socket::create(ep.domain(), ep.type(), ep.proto());
        if (__unlikely(!s)) {
            throw socket_error("Failed to create socket");
        }

        if (s.connect(ep.data(), ep.size())) {
            s.set_cloexec();
            return s;
        }
    }

    return socket{};
}

/**
 * Bind socket to address from resolver
 * @return socket
 * @throw socket_error in case of socket create error
 * @throw socket_error in case of socket failed to listen
 */
__force_inline socket bind(const resolver& r)
{
    for (auto& ep: r) {
        socket s = socket::create(ep.domain(), ep.type(), ep.proto());
        if (__unlikely(!s)) {
            throw socket_error("Failed to create socket");
        }

        s.set_option(socket_options::socket::reuse_address(true));
        if (s.bind(ep.data(), ep.size())) {
            s.set_cloexec();
            if (__unlikely(!s.listen())) {
                throw socket_error("Failed to listen");
            }
            return s;
        }
    }

    return socket{};
}

} /* namespace detail */

__force_inline socket tcp::connect(const std::string& node)
{
    return detail::connect(resolver(tcp_any, node));
}

__force_inline socket tcp::connect(const std::string& node, const std::string& service)
{
    return detail::connect(resolver(tcp_any, node, service));
}

__force_inline socket tcp::bind(const std::string& node)
{
    return detail::bind(resolver(tcp_any, node, AI_PASSIVE));
}

__force_inline socket tcp::bind(const std::string& node, const std::string& service)
{
    return detail::bind(resolver(tcp_any, node, service, AI_PASSIVE));
}

} /* namespace net */
} /* namespace plusone */

#endif /* KSERGEY_tcp_010817124434 */
