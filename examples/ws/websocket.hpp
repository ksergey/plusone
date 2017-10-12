/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_websocket_121017225909
#define KSERGEY_websocket_121017225909

#include <plusone/string_view.hpp>
#include "base64.hpp"

namespace app {

using plusone::string_view;

struct websocket
{
    static std::string prepare_upgrade_request(const string_view& path, const string_view& host,
            const string_view& origin);
};

} /* namespace app */

#endif /* KSERGEY_websocket_121017225909 */
