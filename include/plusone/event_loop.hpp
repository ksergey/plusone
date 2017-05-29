/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_event_loop_290517101456
#define KSERGEY_event_loop_290517101456

#include <functional>
#include <vector>
#include <map>
#include <sys/epoll.h>
#include "clock.hpp"
#include "exception.hpp"

namespace plusone {

namespace detail {
struct event_loop_timer;
struct event_loop_entry;
} /* namespace detail */

/** Event loop */
class event_loop
{
private:
    using timer_queue = std::multimap< std::uint64_t, detail::event_loop_timer >;
    using retired_vector = std::vector< detail::event_loop_entry* >;

    /* Clock source to use for timers */
    const clockid_t clock_id_;
    /* Timer id counter */
    int next_timer_id_{1};
    /* Timers */
    timer_queue timers_;
    /* EPoll descriptor */
    int poll_fd_;
    /* Retired descriptors */
    retired_vector retired_;
    /* Running flag */
    bool running_{false};

public:
    /** Timer callback */
    using timer_callback = std::function< void (int id) >;
    /** Event in descriptor callback */
    using event_callback = std::function< void (int descriptor, std::uint32_t events) >;
    /** Handler type */
    using handle = void*;

    /** Event loop exception type */
    using error = tagged_exception< event_loop >;

    event_loop(const event_loop&) = delete;
    event_loop& operator=(const event_loop&) = delete;

    explicit event_loop(clockid_t clock_id = CLOCK_MONOTONIC_RAW);
    virtual ~event_loop();

    /**
     * Add timeout to expire
     * @param[in] timeout is timeout in milliseconds
     * @param[in] callback is timer callback
     * @return timer id
     */
    int add_timer(std::uint32_t timeout, const timer_callback& callback);

    /** Cancel the timer by timer id */
    void cancel_timer(int id);

    /** Add descriptor to loop */
    handle add(int descriptor, std::uint32_t events, const event_callback& callback);

    /** Modify handle events */
    void modify(handle h, std::uint32_t events);

    /** Remove handle */
    void remove(handle h);

    /** Start loop */
    void run();

    /** Stop loop */
    void stop();

private:
    int execute_timers();
    detail::event_loop_entry* create_entry();
    void destroy_entry(detail::event_loop_entry* entry);
};

} /* namespace plusone */

#include "event_loop_impl.hpp"

#endif /* KSERGEY_event_loop_290517101456 */
