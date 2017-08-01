/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_stream_010817181427
#define KSERGEY_stream_010817181427

#include <plusone/net/socket.hpp>
#include <plusone/net/ssl/context.hpp>

namespace plusone {
namespace net {
namespace ssl {

/** SSL stream */
class stream
{
private:
    plusone::net::socket socket_;

public:
};

} /* namespace ssl */
} /* namespace net */
} /* namespace plusone */

#endif /* KSERGEY_stream_010817181427 */
