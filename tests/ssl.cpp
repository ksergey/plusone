/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <plusone/net/ssl/context.hpp>

TEST_CASE("SSL context")
{
    plusone::net::ssl::context context;
    REQUIRE(!context);
    REQUIRE(context.native_handle() == nullptr);
}
