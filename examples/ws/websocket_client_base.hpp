/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_websocket_client_base_141017160326
#define KSERGEY_websocket_client_base_141017160326

#include <string>
#include <plusone/static_buffer.hpp>
#include <plusone/string_view.hpp>
#include <plusone/compiler.hpp>

namespace app {

using namespace plusone;

struct websocket_client_base
{
    enum parse_result
    {
        not_enought_data,
        success,
        data_error
    };

    std::string prepare_upgrade_request(const string_view& resource, const string_view& host,
            const string_view& origin);

    parse_result check_upgrade_response(static_buffer& b) noexcept;
};

} /* namespace app */

#endif /* KSERGEY_websocket_client_base_141017160326 */
