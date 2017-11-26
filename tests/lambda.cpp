/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <iostream>
#include <string>
#include <plusone/compiler.hpp>
#include <plusone/visitor.hpp>

int main(int argc, char* argv[])
{
    plusone::ignore_unused(argc, argv);

    int int_value = 5;
    std::string string_value{"Test"};

    try {
        auto v = plusone::make_visitor(
            [](int v) {
                std::cout << "int value " << v << '\n';
            },
            [](const std::string& v) {
                std::cout << "string value " << v << '\n';
            },
            [](int x, int y) {
                std::cout << "x=" << x << ",y=" << y << "\n";
            }
        );

        v(string_value);
        v(int_value);
        v(7);
        v("hello");
        v(3, 2);
    } catch (const std::exception& e) {
        std::cout << "ERROR: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
