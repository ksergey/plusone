/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_protocol_051116233028_MADLIFE
#define MADLIFE_protocol_051116233028_MADLIFE

#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>

namespace plusone {
namespace net {

/** Protocol description */
struct protocol
{
    /** Protocol domain (ie AF_INET, AF_INET6) */
    int domain;
    /** Protocol type (ie SOCK_STREAM) */
    int type;
    /** Protocol id (almost always 0) */
    int proto;
};

static constexpr const protocol tcp_any = protocol{AF_UNSPEC,  SOCK_STREAM, 0};
static constexpr const protocol tcp_v4  = protocol{AF_INET,    SOCK_STREAM, 0};
static constexpr const protocol tcp_v6  = protocol{AF_INET6,   SOCK_STREAM, 0};
static constexpr const protocol udp_any = protocol{AF_UNSPEC,  SOCK_DGRAM,  0};
static constexpr const protocol udp_v4  = protocol{AF_INET,    SOCK_DGRAM,  0};
static constexpr const protocol udp_v6  = protocol{AF_INET6,   SOCK_DGRAM,  0};

} /* namespace net */
} /* namespace plusone */

#endif /* MADLIFE_protocol_051116233028_MADLIFE */
