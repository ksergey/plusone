/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include "websocket.hpp"
#include <plusone/fmt.hpp>

namespace app {
namespace {

__force_inline std::string prepare_upgrade_request(const string_view& path, const string_view& host, const string_view& origin)
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

template< class Stream >
bool send_all(Stream& s, const void* data, std::size_t size)
{
    plusone::consuming_buffer buffer{data, size};
    while (buffer.size() > 0) {
        auto rc = s.send(buffer.data());
        if (__likely(rc)) {
            buffer.consume(rc.bytes());
        } else if (__unlikely(!rc.again())) {
            break;
        }
    }
    return buffer.size() == 0;
}

} /* namespace */

using namespace plusone;
using namespace plusone::string_view_literals;

websocket::websocket(const string_view& host, const string_view& service, const string_view& origin)
    : host_{host}
    , service_{service}
    , origin_{origin}
{}

bool websocket::open()
{
    socket_ = plusone::net::tcp::connect(host_, service_);
    if (!socket_) {
        return false;
    }
    socket_.set_option(plusone::net::socket_options::ip::tcp::nodelay{true});
    socket_.set_nonblock();
    wait_upgrade_ = true;
    return true;
}

bool websocket::send_upgrade(const string_view& path)
{
    __expect( wait_upgrade_ );
    if (__unlikely(!socket_)) {
        return false;
    }
    auto upgrade = prepare_upgrade_request(path, host_, origin_);
    return send_all(socket_, upgrade.data(), upgrade.size());
}

bool websocket::poll()
{
    auto rc = socket_.recv(buffer_.prepare());
    if (__likely(rc)) {
        buffer_.commit(rc.bytes());
        parse_buffer();
        return true;
    } else if (__unlikely(!rc.again())) {
        throw_ex< std::runtime_error >("Receive error, {}", rc.str());
    }
    return false;
}

void websocket::parse_buffer()
{
    std::cout << "parsing\n";
    if (__unlikely(wait_upgrade_)) {
        parse_upgrade();
    } else {
        parse_frame();
    }
}

__force_inline void websocket::parse_upgrade()
{
    static constexpr auto resp_end = "\r\n\r\n"sv;

    auto data = buffer_.data();
    string_view str{buffer_cast< const char* >(data), buffer_size(data)};
    auto found = str.find(resp_end);
    if (__likely(found != string_view::npos)) {
        wait_upgrade_ = false;
        const auto len = found + resp_end.size();
        std::cout << "response:\n" << str.substr(0, len);
        buffer_.consume(len);
    }
}

__force_inline void websocket::parse_frame()
{
    auto size = buffer_size(buffer_.data());
    if (__unlikely(size < 2)) {
        std::cout << "no data" << size << "\n";
        return ;
    }

    auto data = buffer_cast< std::uint8_t* >(buffer_.data());

    std::uint8_t opcode = data[0] & 0x0F;
    bool fin = bool(data[0] >> 7);
    bool masked = bool(data[1] >> 7);
    std::size_t length = data[1] & ~0x80;
    std::size_t pos = 2;
    std::size_t payload_length = length;
    if (length == 126) {
        payload_length = data[2] + (data[3] << 8);
        pos += 2;
    } else if (length == 127) {
        payload_length = data[2] + (data[3] << 8);
        pos += 8;
    }

    if (size < payload_length + pos) {
        std::cout << size << " " << payload_length << " " << pos << '\n';
        return ;
    }

    std::cout << plusone::fmt::format("frame [ opcode={} fin={} masked={} payload_size={} ]", opcode, fin, masked, payload_length) << '\n';

    buffer_.consume(payload_length + pos);
}

} /* namespace app */
