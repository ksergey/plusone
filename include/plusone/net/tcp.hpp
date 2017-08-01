/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_tcp_061116025202_MADLIFE
#define MADLIFE_tcp_061116025202_MADLIFE

#include <plusone/net/socket.hpp>

namespace plusone {
namespace net {

/** TCP oriented socket creators */
struct tcp
{
    /** Connect socket to host */
    static socket connect(const std::string& node);

    /** Connect socket to host */
    static socket connect(const std::string& node, const std::string& service);

    /** Bind socket */
    static socket bind(const std::string& node);

    /** Bind socket */
    static socket bind(const std::string& node, const std::string& service);
};

} /* namespace net */
} /* namespace plusone */

#include <plusone/net/impl/tcp.ipp>

#endif /* MADLIFE_tcp_061116025202_MADLIFE */
