/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_service_impl_250517190652
#define KSERGEY_service_impl_250517190652

#include <unistd.h>

namespace plusone {
namespace net {
namespace curl {

__force_inline service::service(std::size_t max_connections)
{
    epoll_fd_ = ::epoll_create1(EPOLL_CLOEXEC);
    if (__unlikely(epoll_fd_ == -1)) {
        throw error{"Failed to create epoll descriptor"};
    }
    events_.resize(max_connections);

    /* Setup CURL */
    session_.set_option(CURLMOPT_MAX_TOTAL_CONNECTIONS, max_connections);
    session_.set_option(CURLMOPT_MAXCONNECTS, 100); /* Configurable? */
    session_.set_option(CURLMOPT_SOCKETFUNCTION, socket_callback);
    session_.set_option(CURLMOPT_SOCKETDATA, this);
}

__force_inline service::~service() noexcept
{ ::close(epoll_fd_); }

__no_inline int service::socket_callback(CURL* session, curl_socket_t socket, int action,
            void* user_data, void* socket_data)
{
    service* self = static_cast< service* >(user_data);

    switch (action) {
        case CURL_POLL_NONE:
            break;
        case CURL_POLL_IN:
            break;
        case CURL_POLL_OUT:
            break;
        case CURL_POLL_INOUT:
            break;
        case CURL_POLL_REMOVE:
            break;
        default:
            break;
    }
    return 0;
}

} /* namespace curl */
} /* namespace net */
} /* namespace plusone */

#endif /* KSERGEY_service_impl_250517190652 */
