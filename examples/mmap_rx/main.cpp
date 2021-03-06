/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <netinet/ip.h>
#include <cstdlib>
#include <csignal>
#include <iostream>
#include <plusone/net/mmap_rx.hpp>
#include <plusone/signal.hpp>

static void process(const plusone::net::mmap_rx::frame& frame)
{
    assert( frame );

    auto& ip = frame.as< iphdr >();
    if (ip.protocol == IPPROTO_ICMP) {
        std::cout << frame.sec() << '.' << frame.nsec() << ' ' << "Packet received {size=" << frame.size() << "}\n";
    }
}

int main(int argc, char* argv[])
{
    char* iface = nullptr;
    if (argc == 2) {
        iface = argv[1];
    }

    try {
        sig_atomic_t sigint = 0;
        plusone::signal::set_handler({SIGINT, SIGTERM}, [&sigint](int){ sigint = 1; });

        if (iface) {
            std::cout << "interface " << iface << '\n';
        }

        plusone::net::mmap_rx rx{iface, 1024 * 1024};

        while (__likely(!sigint)) {
            auto frame = rx.get_next_frame();

            /* Check if frame available */
            if (__unlikely(!frame)) {
                continue;
            }

            /* Process frame */
            process(frame);
        }
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
