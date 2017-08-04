/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <netinet/ip.h>
#include <cstdlib>
#include <csignal>
#include <iostream>
#include <plusone/net/mmap_rx.hpp>
#include <plusone/process.hpp>
#include "multicast_channel_group.hpp"
#include "packet_dispatcher.hpp"

template< class Tag >
class generic_handler
{
private:
    const char* tag_;
    const std::uint16_t port_;

public:
    explicit generic_handler(const char* tag, std::uint16_t port)
        : tag_{tag}
        , port_{port}
    {}

    bool contain(const dispatch::endpoint_v4& ep) const
    { return ep.port() >= port_; }

    void process_packet(const dispatch::endpoint_v4& address, std::uint64_t timestamp,
            const std::uint8_t* data, std::size_t size)
    { std::cout << tag_ << " message received to address " << address << "\n"; }
};

int main(int argc, char* argv[])
{
    char* iface = nullptr;
    if (argc == 2) {
        iface = argv[1];
    }

    try {
        plusone::process::install_signal_handlers();

        if (iface) {
            std::cout << "interface " << iface << '\n';
        }

        generic_handler< struct X > hx{"X", 50000};
        generic_handler< struct Y > hy{"Y", 40000};
        generic_handler< struct Z > hz{"Z", 1};

        plusone::net::mmap_rx rx_queue{iface, 1024 * 1024 * 128};

        dispatch::packet_dispatcher<
            decltype(hx), decltype(hy), decltype(hz)
        > group{rx_queue, hx, hy, hz};

        while (__likely(plusone::process::running())) {
            group.dispatch();
        }

    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
