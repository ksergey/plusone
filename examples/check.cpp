/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <array>
#include <iostream>
#include <atomic>
#include <plusone/expect.hpp>
#include <plusone/compiler.hpp>
#include <plusone/exception.hpp>
#include <plusone/file.hpp>
#include <plusone/mapped_region.hpp>
#include <plusone/static_vector.hpp>
#include <plusone/object_pool.hpp>
#include <plusone/tagged_tuple.hpp>
#include <plusone/ns_alias.hpp>
#include <plusone/clock.hpp>
#include <plusone/clock_io.hpp>

typedef std::array< char, 128 > buffer_t;
typedef p1::tagged_exception< int > tagged_exception;

int main(int argc, char* argv[])
{
    p1::file file{p1::open_or_create, "./test_file"};
    if (file.size() == 0) {
        file.truncate(1024);
        std::cout << "truncated\n";
    }

    p1::mapped_region region{file};

    std::cout << "region size: " << region.size() << '\n';

    std::cout << sizeof(bool) << '\n';
    std::cout << sizeof(std::atomic< bool >) << '\n';
    buffer_t buffer0 __aligned(16);

    __expect( argc > 2 );
    __expectm( argc > 3, "not enought args" );

    tagged_exception("msg %s %d %d", "test", 512, 137) << "  hahahaha " << 31337;
    tagged_exception() << "  hahahaha " << 31337;

    p1::static_vector< int > vec{5};
    for (int i: {77, 1, 2, 3, 4}) {
        std::cout << vec.emplace_back(i) << '\n';
    }
    for (auto i: vec) {
        std::cout << " " << i << '\n';
    }

    using xtuple = p1::tagged_tuple<
        p1::pair< struct tag3, int >,
        p1::pair< struct tag4, std::string >,
        p1::pair< struct tag5, std::string >
    >;
    xtuple test{999, "hello", "zzzz"};
    p1::get< tag3 >(test) = 64;
    std::cout << p1::get< tag3 >(test) << ' ' << p1::get< tag4 >(test) << ' ' << p1::get< tag5 >(test) << '\n';
    std::cout << std::get< 0 >(test) << '\n';
    std::cout << std::get< 1 >(test) << '\n';
    std::cout << std::get< 2 >(test) << '\n';

    p1::object_pool< xtuple > pool{128};

    std::cout << p1::clock_fmt<>(p1::clock_time< p1::ns >()) << '\n';
    std::cout << p1::clock_fmt< p1::ns, p1::ms, false >(p1::clock_time< p1::ns >()) << '\n';
    std::cout << p1::clock_fmt< p1::ns, p1::sec, false >(p1::clock_time< p1::ns >()) << '\n';
    std::cout << p1::clock_fmt< p1::sec, p1::sec, false >(p1::clock_time< p1::sec >()) << '\n';
    std::cout << p1::clock_fmt< p1::sec, p1::ns, false >(p1::clock_time< p1::sec >()) << '\n';
    std::cout << p1::clock_fmt< p1::ms, p1::ns, false >(p1::clock_time< p1::ms >()) << '\n';

    return 0;
}
