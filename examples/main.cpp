/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <netinet/ip.h>
#include <cstdlib>
#include <csignal>
#include <iostream>
#include <plusone/net/mmap_rx.hpp>

static sig_atomic_t sigint = 0;

static void sighandler(int num)
{ sigint = 1; }

int main(int argc, char* argv[])
{
    char* iface = nullptr;
    if (argc == 2) {
        iface = argv[1];
    }

    try {
        signal(SIGINT, sighandler);

        if (iface) {
            std::cout << "interface " << iface << '\n';
        }

        plusone::net::mmap_rx rx{iface, 1024 * 1024};

        while (__likely(!sigint)) {
            auto packet = rx.get();
            if (__likely(packet)) {
                auto& ip = packet.as< iphdr >();
                if (ip.protocol == IPPROTO_ICMP) {
                    std::cout << packet.sec() << '.' << packet.nsec() << ' ' << "Packet received {size=" << packet.size() << "}\n";
                }
                packet.commit();
            }
        }

    } catch (const std::exception& e) {
        std::cout << "error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
