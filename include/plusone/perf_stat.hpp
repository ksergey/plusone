/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_perf_stat_230617002140
#define KSERGEY_perf_stat_230617002140

#include <linux/perf_event.h>
#include <unistd.h>
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
    void setup(__u64 config);

    /** Setup many events */
    template< __u32 Type = PERF_TYPE_HARDWARE >
    void setup(std::initializer_list< __u64 > configs);

    /** Get event value */
    template< __u32 Type = PERF_TYPE_HARDWARE >
    long long value(__u64 config) const;

    /** Start recording stats */
    void start();

    /** Stop recording stats */
    void stop();

private:
    /*
     * Create event
     * return file descriptor of created event
     */
    int create_event(const key_type& in);

    /* Syscall wrapper */
    static long perf_event_open(perf_event_attr* hw_event, pid_t pid,
            int cpu, int group_fd, unsigned long flags);
};

} /* namespace plusone */

#include "impl/perf_stat.ipp"

#endif /* KSERGEY_perf_stat_230617002140 */
