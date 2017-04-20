/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <cstdio>
#include <plusone/net/socket.hpp>
#include <plusone/net/socket_option.hpp>
#include <plusone/net/endpoint.hpp>
#include <plusone/net/tcp.hpp>

int main(int argc, char* argv[])
{
    auto sock = plusone::net::socket::create(AF_INET, SOCK_STREAM, 0);
    sock.set_cloexec();
    sock.set_nonblock();

    auto res = sock.set_option(plusone::net::rcvbuf(12000));
    if (res) {
        std::printf("option rcvbuf set\n");
    }

    auto resolver = plusone::net::resolver(plusone::net::tcp_any, "google.ru:http");
    std::printf("google.ru resolved as:\n");
    for (auto& ep: resolver) {
        std::printf("   %s\n", ep.str().c_str());
    }

    return 0;
}
