/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_socket_options_230417133732
#define KSERGEY_socket_options_230417133732

#include <linux/if_packet.h> /* SOL_PACKET:PACKET_TIMESTAMP */
#include <linux/net_tstamp.h>
#include "detail/socket_option_types.hpp"

namespace plusone {
namespace net {
namespace socket_options {

struct socket
{
    using reuse_address = detail::boolean_option< SOL_SOCKET, SO_REUSEADDR >;
    using receive_buffer_size = detail::integer_option< SOL_SOCKET, SO_RCVBUF >;
    using send_buffer_size = detail::integer_option< SOL_SOCKET, SO_SNDBUF >;
    using timestamp = detail::boolean_option< SOL_SOCKET, SO_TIMESTAMP >;
    using timestamp_ns = detail::boolean_option< SOL_SOCKET, SO_TIMESTAMPNS >;
};

struct packet
{
    using timestamp = detail::integer_option< SOL_PACKET, PACKET_TIMESTAMP >;
    using version = detail::integer_option< SOL_PACKET, PACKET_VERSION >;
};

struct ip
{
    struct multicast
    {
        using join_group = detail::multicast_request_v4_option< IP_ADD_MEMBERSHIP >;
        using leave_group = detail::multicast_request_v4_option< IP_DROP_MEMBERSHIP >;
        using join_source_group = detail::multicast_source_request_v4_option< IP_ADD_SOURCE_MEMBERSHIP >;
        using leave_source_group = detail::multicast_source_request_v4_option< IP_DROP_SOURCE_MEMBERSHIP >;
    };

    struct tcp
    {
        using nodelay = detail::boolean_option< IPPROTO_TCP, TCP_NODELAY >;
    };
};

} /* namespace socket_options */
} /* namespace net */
} /* namespace plusone */

#endif /* KSERGEY_socket_options_230417133732 */
