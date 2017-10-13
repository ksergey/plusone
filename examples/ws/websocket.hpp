/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_websocket_121017225909
#define KSERGEY_websocket_121017225909

#include <array>
#include <plusone/string_view.hpp>
#include <plusone/compiler.hpp>
#include <plusone/expect.hpp>
#include <plusone/consuming_buffer.hpp>
#include <plusone/static_buffer.hpp>
#include <plusone/net/tcp.hpp>
#include "base64.hpp"

namespace app {

using plusone::string_view;

class websocket
{
private:
    const std::string host_;
    const std::string service_;
    const std::string origin_;

    plusone::net::socket socket_;

    std::array< char, 1024 * 1024 * 4 > storage_;
    plusone::static_buffer buffer_{storage_.data(), storage_.size()};

    bool wait_upgrade_{false};

public:
    websocket(const string_view& host, const string_view& service, const string_view& origin);

    bool open();
    bool send_upgrade(const string_view& path = "/");
    bool poll();

private:
    void parse_buffer();
    void parse_upgrade();
    void parse_frame();
};

} /* namespace app */

#endif /* KSERGEY_websocket_121017225909 */
