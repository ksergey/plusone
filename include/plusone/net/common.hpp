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
#include <csignal>
#include <cerrno>
#include <cstring>
#include <stdexcept>
#include "../compiler.hpp"
#include "../exception.hpp"

namespace plusone {
namespace net {

/** socket descriptor type */
typedef int sock_t;

class socket;
typedef tagged_exception< socket > exception;

/** invalid socket descriptor value */
static constexpr const sock_t invalid_socket = -1;

namespace detail {

/** prevent process termination during SIGPIPE signal */
struct sigpipe_initializer final
{
    sigpipe_initializer()
    { std::signal(SIGPIPE, SIG_IGN); }
};

/* trick */
static const sigpipe_initializer sigpipe_initialized{};

} /* namespace detail */

} /* namespace net */
} /* namespace plusone */

#endif /* MADLIFE_common_051116230336_MADLIFE */
