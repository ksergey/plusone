/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <iostream>
#include <plusone/static_vector.hpp>
#include <plusone/cacheline_padded.hpp>

__force_inline void do_check(bool expr, const char* msg, const char* file, int line)
{
    if (__unlikely(!expr)) {
        std::cout << "EXPR \"" << msg << "\" failed at " << file << ':' << line << '\n';
    } else {
        std::cout << "PASS [ " << msg << " ] " << file << ':' << line << '\n';
    }
}

#define CHECK(expr) do_check((expr), #expr, __FILE__, __LINE__)

int main(int argc, char* argv[])
{
    plusone::static_vector< int > v;
    CHECK( v.empty() );
    CHECK( v.size() == 0 );
    CHECK( v.capacity() == 0 );

    v = plusone::static_vector< int >{5};
    CHECK( v.empty() );
    CHECK( v.size() == 0 );
    CHECK( v.capacity() == 5 );

    auto& r = v.emplace_back(99);
    CHECK( !v.empty() );
    CHECK( v.size() == 1 );
    CHECK( r == 99 );
    CHECK( v.front() == 99 );
    CHECK( v.back() == 99 );
    CHECK( v[0] == 99 );
    CHECK( *v.begin() == 99 );

    v.emplace_back(33);
    CHECK( !v.empty() );
    CHECK( v.size() == 2 );
    CHECK( v.front() == 99 );
    CHECK( v.back() == 33 );
    CHECK( v[0] == 99 );
    CHECK( v[1] == 33 );
    CHECK( *v.begin() == 99 );

    plusone::cacheline_padded<
        plusone::static_vector< int >
    > padded_v;

    CHECK( padded_v->size() == 0 );

    return EXIT_SUCCESS;
}
