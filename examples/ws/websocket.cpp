/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include "websocket.hpp"
#include <plusone/fmt.hpp>

namespace app {

std::string websocket::prepare_upgrade_request(const string_view& path, const string_view& host,
        const string_view& origin)
{
    return plusone::fmt::format(
        "GET {} HTTP/1.1\r\n"
        "Host: {}\r\n"
        "Upgrade: websocket\r\n"
        "Connection: Upgrade\r\n"
        "Origin: {}\r\n"
        "Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\n"
        "Sec-WebSocket-Version: 13\r\n"
        "\r\n",
        path, host, origin
    );
}

} /* namespace app */
