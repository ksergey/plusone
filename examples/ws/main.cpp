/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <iostream>
#include <plusone/exception.hpp>
#include "websocket.hpp"

using namespace plusone;
using namespace app;

static const std::string host = "echo.websocket.org";
static const std::string service = "http";

int main(int argc, char* argv[])
{
    try {
#if 0
        const string_view data_to_encode = "test";
        base64_encode(data_to_encode.begin(), data_to_encode.end(),
                std::ostream_iterator< char >{std::cout});
        std::cout << '\n';
#endif

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

    } catch (const std::exception& e) {
        std::cout << "ERROR: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
