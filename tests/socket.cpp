/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <cstring>
#include <plusone/net/tcp.hpp>

TEST_CASE("TCP socket")
{
    auto server_socket = plusone::net::tcp::bind("localhost", "31333");

    REQUIRE(server_socket);

    auto client_socket = plusone::net::tcp::connect("localhost", "31333");
    REQUIRE(client_socket);

    auto result = server_socket.accept();
    REQUIRE(result);

    auto socket = result.get();
    REQUIRE(socket);

    std::string message = "hello";

    auto send_result = socket.send(message.data(), message.size());
    REQUIRE(send_result);
    REQUIRE(send_result.bytes() == message.size());

    char buffer[128];
    auto recv_result = client_socket.recv(buffer, sizeof(buffer));
    REQUIRE(recv_result);
    REQUIRE(send_result.bytes() == message.size());

    REQUIRE(std::strncmp(buffer, message.data(), message.size()) == 0);

    client_socket.close();
    REQUIRE(!client_socket);

    socket.close();
    REQUIRE(!socket);

    server_socket.close();
    REQUIRE(!server_socket);

    client_socket = plusone::net::tcp::connect("localhost", "31333");
    REQUIRE(!client_socket);
}

TEST_CASE("net result")
{
    plusone::net::io_result result;

    REQUIRE(!result);
}
