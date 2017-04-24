/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <cstdlib>
#include <iostream>
#include <csignal>
#include <plusone/net/mmap_rx.hpp>
#include "packet_source_v3.hpp"

static sig_atomic_t sigint = 0;

static void sighandler(int num)
{ sigint = 1; }

int main(int argc, char* argv[])
{
    try {
        signal(SIGINT, sighandler);

        /* TODO: hardcore some of params */
        plusone::net::mmap_rx rx{nullptr, getpagesize(), 64, getpagesize()};

        //sample::packet_source_v3 source{"enp0s31f6"};

        while (__likely(!sigint)) {
            rx.run_once();
        }

    } catch (const std::exception& e) {
        std::cout << "error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
