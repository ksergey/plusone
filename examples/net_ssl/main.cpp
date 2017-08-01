/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <iostream>
#include <plusone/net/ssl/context.hpp>
#include <plusone/net/ssl/stream.hpp>
#include <plusone/net/resolver.hpp>

int main(int argc, char* argv[])
{
    try {
        //plusone::net::resolver resolver{plusone::net::tcp_any, argv[1]};
        //if (resolver) {
        //    std::cout << "Resolved:\n";
        //    for (auto& entry: resolver) {
        //        std::cout << entry.str() << "\n";
        //    }
        //}

    } catch (const std::exception& e) {
        std::cout << "ERROR: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
