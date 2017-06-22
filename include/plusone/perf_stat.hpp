/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_perf_stat_230617002140
#define KSERGEY_perf_stat_230617002140

#include <linux/hw_breakpoint.h>
#include <linux/perf_event.h>
#include <asm/unistd.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <map>
#include <tuple>
#include "file_descriptor.hpp"
#include "exception.hpp"

namespace plusone {

/**
 * Perf stat counters
 */
class perf_stat
{
private:
    /* Config+type */
    using key_type = std::tuple< __u64, __u32 >;

    /* Entry */
    struct entry
    {
        file_descriptor fd;
        long long value{0};
    };

    /* Group descriptor */
    int group_fd_{-1};

    /* Mapping Config+Type -> Entry */
    std::map< key_type, entry > mapping_;

public:
    using error = tagged_exception< perf_stat >;

    perf_stat(const perf_stat&) = delete;
    perf_stat& operator=(const perf_stat&) = delete;
    perf_stat() = default;

    /** Setup perf stat entry */
    template< __u32 Type = PERF_TYPE_HARDWARE >
    __force_inline void setup(__u64 config)
    {
        int fd = create_event(std::make_tuple(config, Type));
        if (group_fd_ == -1) {
            group_fd_ = fd;
        }
    }

    /** Setup many events */
    template< __u32 Type = PERF_TYPE_HARDWARE >
    __force_inline void setup(std::initializer_list< __u64 > configs)
    {
        for (__u64 config: configs) {
            setup< Type >(config);
        }
    }

    /** Get event value */
    template< __u32 Type = PERF_TYPE_HARDWARE >
    __force_inline long long value(__u64 config) const
    {
        auto found = mapping_.find(std::make_tuple(config, Type));
        if (__unlikely(found == mapping_.end())) {
            throw error{"Entry not exists"};
        }
        return found->second.value;
    }

    /** Start recording stats */
    __force_inline void start()
    {
        if (__unlikely(group_fd_ == -1)) {
            return;
        }

        ioctl(group_fd_, PERF_EVENT_IOC_RESET, 0);
        ioctl(group_fd_, PERF_EVENT_IOC_ENABLE, 0);
    }

    /** Stop recording stats */
    __force_inline void stop()
    {
        if (__unlikely(group_fd_ == -1)) {
            return;
        }

        ioctl(group_fd_, PERF_EVENT_IOC_DISABLE, 0);

        for (auto& e: mapping_) {
            read(e.second.fd, &e.second.value, sizeof(e.second.value));
        }
    }

private:
    /*
     * Create event
     * return file descriptor of created event
     */
    inline int create_event(const key_type& in)
    {
        perf_event_attr attr{0};
        attr.type = std::get< 1 >(in);
        attr.size = sizeof(perf_event_attr);
        attr.config = std::get< 0 >(in);
        attr.exclude_kernel = 1;
        attr.exclude_hv = 1;
        attr.disabled = (group_fd_ == -1) ? 1 : 0;

        file_descriptor fd = perf_event_open(&attr, 0, -1, group_fd_, 0);
        if (__unlikely(!fd)) {
            throw error{"Perf event error (%s)", ::strerror(errno)};
        }

        auto result = mapping_.emplace(in, entry{});
        if (__unlikely(!result.second)) {
            throw error{"Entry already exists"};
        }

        auto& entry = result.first->second;
        entry.fd = std::move(fd);
        return entry.fd.get();
    }

    /* Syscall wrapper */
    static long perf_event_open(perf_event_attr* hw_event, pid_t pid, int cpu, int group_fd, unsigned long flags)
    { return syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags); }
};

} /* namespace plusone */

#endif /* KSERGEY_perf_stat_230617002140 */
