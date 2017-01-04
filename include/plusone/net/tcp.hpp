/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_tcp_061116025202_MADLIFE
#define MADLIFE_tcp_061116025202_MADLIFE

#include "detail/tcp.hpp"

namespace plusone {
namespace net {

/** tcp oriented socket creators */
struct tcp
{
    /** connect socket to host */
    __force_inline static socket connect(const std::string& node)
    { return detail::connect(resolver(tcp_any, node)); }

    /** connect socket to host */
    __force_inline static socket connect(const std::string& node, const std::string& service)
    { return detail::connect(resolver(tcp_any, node, service)); }

    /** bind socket */
    __force_inline static socket bind(const std::string& node)
    { return detail::bind(resolver(tcp_any, node, AI_PASSIVE)); }

    /** bind socket */
    __force_inline static socket bind(const std::string& node, const std::string& service)
    { return detail::bind(resolver(tcp_any, node, service, AI_PASSIVE)); }
};

} /* namespace net */
} /* namespace plusone */

#endif /* MADLIFE_tcp_061116025202_MADLIFE */
