/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <iostream>
#include <plusone/consuming_buffer.hpp>
#include <plusone/exception.hpp>
#include <plusone/static_buffer_base.hpp>
#include <plusone/net/tcp.hpp>
#include "websocket.hpp"

using namespace plusone;
using namespace app;

static const std::string host = "echo.websocket.org";
static const std::string service = "http";

template< class Stream >
__force_inline bool send_all(Stream& s, const void* data, std::size_t size)
{
    consuming_buffer buffer{data, size};
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

int main(int argc, char* argv[])
{
    try {
#if 0
        const string_view data_to_encode = "test";
        base64_encode(data_to_encode.begin(), data_to_encode.end(),
                std::ostream_iterator< char >{std::cout});
        std::cout << '\n';
#endif
        auto socket = plusone::net::tcp::connect(host, service);
        if (!socket) {
            throw_ex< std::runtime_error >("Failed to connect to {}:{}", host, service);
        }

        auto upgrade = websocket::prepare_upgrade_request("/", host, host);
        if (__unlikely(!send_all(socket, upgrade.data(), upgrade.size()))) {
            throw_ex< std::runtime_error >("Failed to upgrade connection");
        }

        std::array< char, 1024 * 1024 * 4 > storage;
        static_buffer_base buffer{storage.data(), storage.size()};

        while (true) {
            auto rc = socket.recv(buffer.prepare());
            if (__likely(rc)) {
                buffer.commit(rc.bytes());
                auto data = buffer.data();
                std::cout << string_view{buffer_cast< const char* >(data), buffer_size(data)};
            } else if (__unlikely(!rc.again())) {
                throw_ex< std::runtime_error >("Receive error, {}", rc.str());
            }
        }

    } catch (const std::exception& e) {
        std::cout << "ERROR: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
