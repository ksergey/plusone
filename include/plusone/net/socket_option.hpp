/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_socket_option_061116002548_MADLIFE
#define MADLIFE_socket_option_061116002548_MADLIFE

#include <linux/if_packet.h> /* SOL_PACKET:PACKET_TIMESTAMP */
#include <linux/net_tstamp.h>
#include "detail/socket_option_types.hpp"

namespace plusone {
namespace net {

using tcp_nodelay = detail::boolean_option< IPPROTO_TCP, TCP_NODELAY >;
using reuseaddr = detail::boolean_option< SOL_SOCKET, SO_REUSEADDR >;
using recv_buffer_size = detail::integer_option< SOL_SOCKET, SO_RCVBUF >;
using send_buffer_size = detail::integer_option< SOL_SOCKET, SO_SNDBUF >;
using join_group = detail::multicast_request_v4_option< IP_ADD_MEMBERSHIP >;
using leave_group = detail::multicast_request_v4_option< IP_DROP_MEMBERSHIP >;
using join_source_group = detail::multicast_source_request_v4_option< IP_ADD_SOURCE_MEMBERSHIP >;
using leave_source_group = detail::multicast_source_request_v4_option< IP_DROP_SOURCE_MEMBERSHIP >;
using timestamp = detail::boolean_option< SOL_SOCKET, SO_TIMESTAMP >;
using timestamp_ns = detail::boolean_option< SOL_SOCKET, SO_TIMESTAMPNS >;
using packet_timestamp = detail::integer_option< SOL_PACKET, PACKET_TIMESTAMP >;
using packet_version = detail::integer_option< SOL_PACKET, PACKET_VERSION >;

} /* namespace net */
} /* namespace plusone */

#endif /* MADLIFE_socket_option_061116002548_MADLIFE */
