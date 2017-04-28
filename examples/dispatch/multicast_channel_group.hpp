/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_multicast_channel_group_280417111330
#define KSERGEY_multicast_channel_group_280417111330

#include <plusone/static_vector.hpp>
#include "multicast_channel.hpp"

namespace dispatch {

/** Group of multicast channels */
class multicast_channel_group final
{
private:
    plusone::static_vector< multicast_channel > channels_;
    bool active_{false};

public:
    multicast_channel_group(const multicast_channel_group&) = delete;
    multicast_channel_group& operator=(const multicast_channel_group&) = delete;
    multicast_channel_group() = default;

    multicast_channel_group(multicast_channel_group&& v)
    { swap(v); }

    multicast_channel_group& operator=(multicast_channel_group&& v)
    {
        if (this != &v) {
            swap(v);
        }
        return *this;
    }

    explicit multicast_channel_group(std::initializer_list< multicast_channel::config >& channels)
        : channels_{channels_.size()}
    {
        for (const auto& c: channels) {
            channels_.emplace_back(c);
        }
    }

    /** Join multicast group */
    __force_inline void join()
    {
        if (!active_) {
            for (auto& ch: channels_) {
                ch.join();
            }
        }
    }

    /** Leave multicast group */
    __force_inline void leave()
    {
        if (active_) {
            for (auto& ch: channels_) {
                ch.leave();
            }
        }
    }

    /** Return true if address matched to one channel in group */
    __force_inline bool contain(const endpoint_v4& ep) const noexcept
    {
        for (const auto& ch: channels_) {
            if (ep == ch.address()) {
                return true;
            }
        }
        return false;
    }

    /** Swap contents with another group */
    __force_inline void swap(multicast_channel_group& v)
    {
        std::swap(v.channels_, channels_);
        std::swap(v.active_, active_);
    }
};

} /* namespace dispatch */

#endif /* KSERGEY_multicast_channel_group_280417111330 */
