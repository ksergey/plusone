/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <array>
#include <iostream>
#include <atomic>
#include <plusone/expect.hpp>
#include <plusone/compiler.hpp>
#include <plusone/exception.hpp>

typedef std::array< char, 128 > buffer_t;
typedef plusone::tagged_exception< int > tagged_exception;

int main(int argc, char* argv[])
{
    std::cout << sizeof(bool) << '\n';
    std::cout << sizeof(std::atomic< bool >) << '\n';
    buffer_t buffer0 __aligned(16);

    __expect( argc > 2 );
    __expectm( argc > 3, "not enought args" );

    tagged_exception("msg %s %d %d", "test", 512, 137) << "  hahahaha " << 31337;
    tagged_exception() << "  hahahaha " << 31337;

    return 0;
}
