/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_signal_101116205803_MADLIFE
#define MADLIFE_signal_101116205803_MADLIFE

#include <sys/signalfd.h>
#include <csignal>
#include <cstring>
#include <functional>
#include <initializer_list>
#include "event_loop.hpp"

namespace plusone {

class signal
{
private:
    using callback_type = std::function< void (int sig) >;

    /* Callback function */
    static callback_type global_callback_;

    event_loop& loop_;
    sigset_t old_mask_;
    int fd_{-1};
    callback_type callback_;
    event_loop::handle handle_{nullptr};

public:
    /** Signal exception type */
    using error = tagged_exception< signal >;

    signal(const signal&) = delete;
    signal& operator=(const signal&) = delete;

    /** Construct signal listener */
    template< class Callback >
    signal(event_loop& loop, std::initializer_list< int > sigs, Callback&& callback)
        : loop_{loop}
        , callback_{callback}
    {
        sigset_t mask;
        sigemptyset(&mask);
        for (auto sig: sigs) {
            sigaddset(&mask, sig);
        }

        sigprocmask(SIG_BLOCK, &mask, &old_mask_);

        fd_ = signalfd(-1, &mask, 0);
        if (__unlikely(fd_ == -1)) {
            throw error{"Failed to create signalfd"};
        }

        /* Add handler to loop */
        handle_ = loop_.add(fd_, EPOLLIN, [this](int descriptor, std::uint32_t events) {
            __expect( events & EPOLLIN );
            signalfd_siginfo data;
            int rc = ::read(fd_, &data, sizeof(data));
            if (__unlikely(rc != sizeof(data))) {
                throw error{"Internal error (%s:%d)", __FILE__, __LINE__};
            }
            callback_(data.ssi_signo);
        });
    }

    /** Destroy signal handler */
    __force_inline ~signal() noexcept
    {
        loop_.remove(handle_);
        ::close(fd_);
        sigprocmask(SIG_SETMASK, &old_mask_, nullptr);
    }

    /** Setup signal handler */
    template< class Callback >
    static __force_inline void setup(std::initializer_list< int > sigs, Callback&& callback)
    {
        global_callback_ = callback;

        for (auto sig: sigs) {
            struct sigaction sa{};
            std::memset(&sa, 0, sizeof(sa));
            sa.sa_handler = native_signal_handler;
            ::sigaction(sig, &sa, nullptr);
        }
    }

    /** Setup handler for SIGINT and SIGTERM signals */
    template< class Callback >
    static __force_inline void setup(Callback&& callback)
    { setup({SIGINT, SIGTERM}, std::forward< Callback >(callback)); }

private:
    /* Native signal handler func */
    static void native_signal_handler(int sig)
    {
        if (global_callback_) {
            global_callback_(sig);
        }
    }
};

signal::callback_type signal::global_callback_{};

} /* namespace plusone */

#endif /* MADLIFE_signal_101116205803_MADLIFE */
