/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_service_impl_250517190652
#define KSERGEY_service_impl_250517190652

#include <cstring>
#include <cerrno>
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

__force_inline void service::run_once(int timeout_ms)
{
    int rc = epoll_wait(epoll_fd_, events_.data(), events_.size(), timeout_ms);
    if (__unlikely(rc == -1)) {
        if (errno != EINTR) {
            throw error{"epoll_wait error (%s)", ::strerror(errno)};
        } else {
            return;
        }
    }

    for (int i = 0; i < rc; ++i) {
        int mask = 0;
        if (events_[i].events & EPOLLIN) {
            mask |= CURL_CSELECT_IN;
        }
        if (events_[i].events & EPOLLOUT) {
            mask |= CURL_CSELECT_OUT;
        }
        if (events_[i].events & EPOLLHUP || events_[i].events & EPOLLERR) {
            mask |= CURL_CSELECT_ERR;
        }

        curl_multi_socket_action(session_, events_[i].data.fd, mask, nullptr);
    }
}

__force_inline void service::socket_op(curl_socket_t socket, int op, int events)
{
    epoll_event ev;
    ev.data.fd = socket;
    ev.events = EPOLLHUP | EPOLLERR | events;
    int rc = epoll_ctl(epoll_fd_, op, socket, &ev);
    if (__unlikely(rc == -1)) {
        throw error{"epoll_ctl error (%s)", ::strerror(errno)};
    }
}

__no_inline int service::socket_callback(CURL* session, curl_socket_t socket, int what,
            void* user_data, void* socket_data)
{
    service* self = static_cast< service* >(user_data);

    switch (what) {
        case CURL_POLL_NONE:
            {
                self->socket_op(socket, EPOLL_CTL_ADD);
            }
            break;
        case CURL_POLL_IN:
            {
                self->socket_op(socket, EPOLL_CTL_ADD, EPOLLIN);
            }
            break;
        case CURL_POLL_OUT:
            {
                self->socket_op(socket, EPOLL_CTL_ADD, EPOLLOUT);
            }
            break;
        case CURL_POLL_INOUT:
            {
                self->socket_op(socket, EPOLL_CTL_ADD, EPOLLIN | EPOLLOUT);
            }
            break;
        case CURL_POLL_REMOVE:
            {
                self->socket_op(socket, EPOLL_CTL_DEL);
            }
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
