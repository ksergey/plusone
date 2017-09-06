/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <unistd.h>
#include <iostream>
#include <plusone/signal.hpp>

int main(int argc, char* argv[])
{
    try {
        bool runing{true};

        auto handler = [&runing](int) {
            std::cout << "got signal\n";
            runing = false;
        };

        plusone::signal::set_handler(SIGINT, handler);
        plusone::signal::set_handler(SIGTERM, handler);

        std::cout << "start\n";
        while (runing) {
            ::sleep(1);
        }
        std::cout << "stop\n";

    } catch (const std::exception& e) {
        std::cout << "ERROR: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
