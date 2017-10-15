/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <iostream>
#include <plusone/exception.hpp>
#include "websocket.hpp"
#include "websocket_client.hpp"

using namespace plusone;
using namespace app;

static const std::string host = "echo.websocket.org";
static const std::string service = "http";

struct websocket_event_handler
{
    void operator()(const app::open&) noexcept
    {
        std::cout << "open\n";
    }

    void operator()(const app::close&) noexcept
    {
        std::cout << "close\n";
    }
};

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
            throw_ex< std::runtime_error >("Can't connect");
        }
        socket.set_option(plusone::net::socket_options::ip::tcp::nodelay{true});
        socket.set_nonblock();

        app::websocket_client< plusone::net::socket > s;

        if (!s.start(std::move(socket), host)) {
            throw_ex< std::runtime_error >("Failed to start websocket");
        }

        websocket_event_handler handler;

        while (true) {
            s.poll(handler);
        }

        /*
        app::websocket ws{host, service, host};
        if (!ws.open()) {
            throw_ex< std::runtime_error >("Failed to open websocket");
        }
        if (!ws.send_upgrade()) {
            throw_ex< std::runtime_error >("Failed to send upgrade request");
        }

        while (true) {
            ws.poll();
        }
        */

    } catch (const std::exception& e) {
        std::cout << "ERROR: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
