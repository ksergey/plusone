/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include "websocket_client_base.hpp"
#include <plusone/fmt.hpp>

namespace app {

using namespace plusone::string_view_literals;

std::string websocket_client_base::prepare_upgrade_request(const string_view& resource, const string_view& host,
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
        resource, host, origin
    );
}

websocket_client_base::parse_result websocket_client_base::check_upgrade_response(static_buffer& b) noexcept
{
    static constexpr auto resp_end = "\r\n\r\n"sv;

    auto data = b.data();
    string_view str{buffer_cast< const char* >(data), buffer_size(data)};
    auto found = str.find(resp_end);
    if (__likely(found != string_view::npos)) {
        const auto len = found + resp_end.size();
        b.consume(len);
        return success;
    } else {
        return not_enought_data;
    }
}

} /* namespace app */
