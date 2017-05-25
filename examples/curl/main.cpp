/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <netinet/ip.h>
#include <cstdlib>
#include <csignal>
#include <iostream>
#include <plusone/net/curl/service.hpp>
#include <plusone/signal.hpp>

static sig_atomic_t sigint = 0;

int main(int argc, char* argv[])
{
    try {
        plusone::signal::setup([](int sig) {
            sigint = 1;
        });

        plusone::net::curl::cleanup curl_cleanup;
        plusone::net::curl::service service;

        while (__likely(!sigint)) {
            service.run_once(1000);
        }

    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
