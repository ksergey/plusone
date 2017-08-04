/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_common_051116230336_MADLIFE
#define MADLIFE_common_051116230336_MADLIFE

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <plusone/compiler.hpp>
#include <plusone/exception.hpp>
#include <plusone/signal.hpp>

namespace plusone {
namespace net {

/** Socket error types */
using socket_error = tagged_exception< struct socket_tag >;
using address_error = tagged_exception< struct address_tag >;
using option_error = tagged_exception< struct option_tag >;

/** Invalid socket descriptor value */
static constexpr const int invalid_socket = -1;

namespace detail {

/** Prevent process termination during SIGPIPE signal */
struct sigpipe_initializer final
{
    sigpipe_initializer()
    {
        signal::set_handler(SIGPIPE, SIG_IGN);
    }
};

/* Trick */
static const sigpipe_initializer sigpipe_initialized{};

} /* namespace detail */

} /* namespace net */
} /* namespace plusone */

#endif /* MADLIFE_common_051116230336_MADLIFE */
