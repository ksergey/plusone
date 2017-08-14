/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <iostream>
#include <plusone/net/ssl/context.hpp>
#include <plusone/net/ssl/stream.hpp>
#include <plusone/net/tcp.hpp>
#include <plusone/exception.hpp>

using namespace std::string_literals;
using plusone::throw_ex;

//#define HOST "api.ipify.org"
#define HOST "api.ipify.org"

const std::string request =
    "GET / HTTP/1.0\r\n"
    "Host: " HOST "\r\n"
    "Accept: */*\r\n"
    "Connection: close\r\n\r\n";

int main(int argc, char* argv[])
{
    try {
        plusone::net::ssl::context context{plusone::net::ssl::context::sslv23_client};

        auto socket = plusone::net::tcp::connect(HOST, "https");
        if (!socket) {
            throw_ex< std::runtime_error >("Failed to connect to {}", HOST);
        }

        plusone::net::ssl::stream stream{context, std::move(socket)};
        auto handshake_rc = stream.handshake();
        if (!handshake_rc) {
            throw_ex< std::runtime_error >("Handshake error ({})", handshake_rc.str());
        }

        auto send_rc = stream.send(request.data(), request.size());
        if (!send_rc) {
            throw_ex< std::runtime_error >("Send request error ({})", send_rc.str());
        }

        stream.socket().set_nonblock();
        stream.socket().set_option(plusone::net::socket_options::ip::tcp::nodelay{true});

        char response_buffer[1024 * 1024];
        std::size_t block = 32;
        std::size_t offset = 0;

        while (true) {
            auto recv_rc = stream.recv(response_buffer + offset, std::min(block, sizeof(response_buffer) - offset));
            if (recv_rc) {
                offset += recv_rc.bytes();
                if (recv_rc.bytes() != block) {
                    break;
                }
            } else {
                if (recv_rc.disconnected()) {
                    break;
                } else if (!recv_rc.again()) {
                    throw_ex< std::runtime_error >("Receive response error ({}, {})", recv_rc.str(), recv_rc.code());
                }
            }
        }

        std::cout << "Result:\n";
        std::cout.write(response_buffer, offset);
        std::cout << '\n';

    } catch (const std::exception& e) {
        std::cout << "ERROR: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
