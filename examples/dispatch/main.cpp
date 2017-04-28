/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <netinet/ip.h>
#include <cstdlib>
#include <csignal>
#include <iostream>
#include <plusone/net/mmap_rx.hpp>
#include "multicast_channel_group.hpp"
#include "channel_group.hpp"

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

        plusone::net::mmap_rx rx_queue{iface, 1024 * 1024 * 128};
        dispatch::channel_group<> group{rx_queue};

        while (__likely(!sigint)) {
            group.run_once();
        }

    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
