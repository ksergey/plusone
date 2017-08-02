/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <plusone/net/ssl/context.hpp>
#include <plusone/net/ssl/stream.hpp>

TEST_CASE("SSL context")
{
    plusone::net::ssl::context context1;
    REQUIRE(!context1);
    REQUIRE(context1.native_handle() == nullptr);

    plusone::net::ssl::context context2{plusone::net::ssl::context::sslv23_client};
    REQUIRE(context2);

    context1 = std::move(context2);
    REQUIRE(context1);
    REQUIRE(!context2);
}
