/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_tcp_071216195251
#define KSERGEY_tcp_071216195251

#include <string>
#include <stdexcept>
#include "../resolver.hpp"
#include "../socket.hpp"
#include "../socket_option.hpp"

namespace plusone {
namespace net {
namespace detail {

__force_inline static socket connect(const resolver& r)
{
    for (auto& ep: r) {
        socket s = socket::create(ep.domain(), ep.type(), ep.proto());
        if (__unlikely(!s)) {
            throw socket_error{"failed to create socket"};
        }

        if (s.connect(ep.data(), ep.size())) {
            s.set_nonblock();
            s.set_cloexec();
            s.set_option(tcp_nodelay(true));
            return s;
        }
    }

    return socket{};
}

__force_inline static socket bind(const resolver& r)
{
    for (auto& ep: r) {
        socket s = socket::create(ep.domain(), ep.type(), ep.proto());
        if (__unlikely(!s)) {
            throw socket_error{"failed to create socket"};
        }

        s.set_option(reuseaddr(true));
        if (s.bind(ep.data(), ep.size())) {
            s.set_nonblock();
            s.set_cloexec();
            if (__unlikely(!s.listen())) {
                throw socket_error{"failed to listen"};
            }
            return s;
        }
    }

    return socket{};
}

} /* namespace detail */
} /* namespace net */
} /* namespace plusone */

#endif /* KSERGEY_tcp_071216195251 */
