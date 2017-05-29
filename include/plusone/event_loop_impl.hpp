/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_event_loop_impl_290517101556
#define KSERGEY_event_loop_impl_290517101556

#include <unistd.h>
#include <cstring>
#include <cerrno>
#include "expect.hpp"

namespace plusone {

namespace detail {

struct event_loop_timer
{
    int id;
    event_loop::timer_callback callback;
};

struct event_loop_entry
{
    int descriptor;
    epoll_event event;
    event_loop::event_callback callback;
};

} /* namespace detail */

inline event_loop::event_loop(clockid_t clock_id)
    : clock_id_{clock_id}
{
    poll_fd_ = ::epoll_create1(EPOLL_CLOEXEC);
    if (__unlikely(poll_fd_ == -1)) {
        throw error{"epoll_create1 error (%s)", ::strerror(errno)};
    }
}

inline event_loop::~event_loop()
{ ::close(poll_fd_); }

__force_inline int event_loop::add_timer(std::uint32_t timeout, const timer_callback& callback)
{
    std::uint64_t expiration = clock_now< ms >(clock_id_) + timeout;
    timers_.emplace(expiration, detail::event_loop_timer{next_timer_id_, callback});
    return next_timer_id_++;
}

__force_inline void event_loop::cancel_timer(int id)
{
    for (auto it = timers_.begin(); it != timers_.end(); ++it) {
        if (it->second.id == id) {
            timers_.erase(it);
            return ;
        }
    }

    __expectm( false, "Timer not found" );
}

__force_inline event_loop::handle event_loop::add(int descriptor, std::uint32_t events, const event_callback& callback)
{
    /* TODO: make an object pool */
    detail::event_loop_entry* entry = create_entry();
    entry->descriptor = descriptor;
    entry->event.events = events;
    entry->event.data.ptr = entry;
    entry->callback = callback;

    int rc = ::epoll_ctl(poll_fd_, EPOLL_CTL_ADD, descriptor, &entry->event);
    if (__unlikely(rc == -1)) {
        destroy_entry(entry);
        throw error{"epoll_ctl ADD error (%s)", ::strerror(errno)};
    }

    return entry;
}

__force_inline void event_loop::modify(handle h, std::uint32_t events)
{
    detail::event_loop_entry* entry = static_cast< detail::event_loop_entry* >(h);
    entry->event.events = events;
    int rc = ::epoll_ctl(poll_fd_, EPOLL_CTL_MOD, entry->descriptor, &entry->event);
    __expect( rc != -1 );
}

__force_inline void event_loop::remove(handle h)
{
    detail::event_loop_entry* entry = static_cast< detail::event_loop_entry* >(h);
    int rc = ::epoll_ctl(poll_fd_, EPOLL_CTL_DEL, entry->descriptor, nullptr);
    __expect( rc != -1 );
    entry->descriptor = -1;
    retired_.push_back(entry);
}

inline void event_loop::run()
{
    __expect( running_ == false );

    running_ = true;

    /* Max events per epoll_wait call */
    static constexpr std::size_t max_io_events = 16;

    epoll_event events_buf[max_io_events];

    while (__likely(running_)) {
        /* Do timers work */
        int timeout = execute_timers();

        /* Wait for events */
        int rc = ::epoll_wait(poll_fd_, events_buf, max_io_events, timeout);
        if (__unlikely(rc == -1)) {
            if (__likely(errno == EINTR)) {
                continue;
            }
            throw error{"epoll_wait error (%s)", ::strerror(errno)};
        }

        for (int i = 0; i < rc; ++i) {
            detail::event_loop_entry* entry
                = static_cast< detail::event_loop_entry* >(events_buf[i].data.ptr);
            if (__unlikely(entry->descriptor == -1)) {
                continue;
            }
            entry->callback(entry->descriptor, events_buf[i].events);
        }

        /* Destroy retired handlers */
        for (auto entry: retired_) {
            destroy_entry(entry);
        }
        retired_.clear();
    }
}

__force_inline void event_loop::stop()
{ running_ = false; }

__force_inline int event_loop::execute_timers()
{
    if (timers_.empty()) {
        return -1;
    }

    std::uint64_t now = clock_now< ms >(clock_id_);
    auto it = timers_.begin();
    while (it != timers_.end()) {
        if (it->first > now) {
            return static_cast< int >(it->first - now);
        }

        /* Timer should be triggered */
        it->second.callback(it->second.id);

        /* Remove timer for list */
        timers_.erase(it++);
    }

    return -1;
}

__force_inline detail::event_loop_entry* event_loop::create_entry()
{ return new detail::event_loop_entry; }

__force_inline void event_loop::destroy_entry(detail::event_loop_entry* entry)
{ delete entry; }

} /* namespace plusone */

#endif /* KSERGEY_event_loop_impl_290517101556 */
