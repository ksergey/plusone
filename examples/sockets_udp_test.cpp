/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <iostream>
#include <plusone/net/socket.hpp>
#include <plusone/net/socket_option.hpp>
#include <plusone/net/endpoint.hpp>
#include <plusone/net/tcp.hpp>

int main(int argc, char* argv[])
{
    auto sock = plusone::net::socket::create(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    sock.set_cloexec();
    sock.set_nonblock();

    char buffer[2000];
    while (true) {
        auto res = sock.recv(buffer, sizeof(buffer));
        if (!res) {
            if (res.again()) {
                continue;
            } else {
                break;
            }
        }

        std::cout << "Packet received\n";
    }

    return 0;
}
