/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_perf_stat_120717233245
#define KSERGEY_perf_stat_120717233245

#include <linux/hw_breakpoint.h>
#include <asm/unistd.h>
#include <sys/ioctl.h>
#include <cerrno>
#include <cstring>

namespace plusone {

template< __u32 Type >
__force_inline void perf_stat::setup(__u64 config)
{
    int fd = create_event(std::make_tuple(config, Type));
    if (group_fd_ == -1) {
        group_fd_ = fd;
    }
}

template< __u32 Type >
__force_inline void perf_stat::setup(std::initializer_list< __u64 > configs)
{
    for (__u64 config: configs) {
        setup< Type >(config);
    }
}

template< __u32 Type >
__force_inline long long perf_stat::value(__u64 config) const
{
    auto found = mapping_.find(std::make_tuple(config, Type));
    if (__unlikely(found == mapping_.end())) {
        throw error{"Entry not exists"};
    }
    return found->second.value;
}

__force_inline void perf_stat::start()
{
    if (__unlikely(group_fd_ == -1)) {
        return;
    }

    ::ioctl(group_fd_, PERF_EVENT_IOC_RESET, 0);
    ::ioctl(group_fd_, PERF_EVENT_IOC_ENABLE, 0);
}

__force_inline void perf_stat::stop()
{
    if (__unlikely(group_fd_ == -1)) {
        return;
    }

    ::ioctl(group_fd_, PERF_EVENT_IOC_DISABLE, 0);

    for (auto& e: mapping_) {
        read(e.second.fd, &e.second.value, sizeof(e.second.value));
    }
}

__force_inline int perf_stat::create_event(const key_type& in)
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

__force_inline long perf_stat::perf_event_open(perf_event_attr* hw_event, pid_t pid,
        int cpu, int group_fd, unsigned long flags)
{
    return syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
}

} /* namespace plusone */

#endif /* KSERGEY_perf_stat_120717233245 */
