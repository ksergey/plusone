/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <iostream>
#include <exception>
#include <plusone/event_loop.hpp>
#include <plusone/signal.hpp>

int main(int argc, char* argv[])
{
    try {
        plusone::event_loop loop;
        plusone::signal signal(loop, {SIGINT, SIGTERM}, [&loop](int sig) {
            std::cout << "signal catched\n";
            loop.stop();
        });
        loop.add_timer(1000, [&loop](int id) {
            std::cout << "timeout\n";
        });
        loop.run();
        std::cout << "stopped\n";

    } catch (const std::exception& e) {
        std::cout << "ERROR: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
