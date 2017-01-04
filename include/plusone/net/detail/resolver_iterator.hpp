/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_resolver_iterator_061116030307_MADLIFE
#define MADLIFE_resolver_iterator_061116030307_MADLIFE

#include <iterator>
#include "resolver_endpoint.hpp"

namespace plusone {
namespace net {
namespace detail {

/** resolver endpoint iterator */
class resolver_iterator final
    : std::iterator< std::forward_iterator_tag, resolver_endpoint >
{
private:
    /* resolver endpoint iterator data */
    resolver_endpoint ep_;

public:
    /** default constructor */
    resolver_iterator() = default;

    /** default destructor */
    ~resolver_iterator() = default;

    /** construct iterator */
    explicit resolver_iterator(const addrinfo* data)
        : ep_(data)
    {}

    /** increment operation ++ */
    __force_inline resolver_iterator& operator++() noexcept
    {
        if (ep_.data_ != nullptr) {
            ep_.data_ = ep_.data_->ai_next;
        }
        return *this;
    }

    /** equal operator */
    __force_inline bool operator==(const resolver_iterator& it) const noexcept
    { return ep_.data_ == it.ep_.data_; }

    /** not equal operator */
    __force_inline bool operator!=(const resolver_iterator& it) const noexcept
    { return ep_.data_ != it.ep_.data_; }

    /** operator derefernce */
    __force_inline const resolver_endpoint& operator*() const noexcept
    { return ep_; }

    /** operator access member */
    __force_inline const resolver_endpoint* operator->() const noexcept
    { return &ep_; }
};

} /* namespace detail */
} /* namespace net */
} /* namespace plusone */

#endif /* MADLIFE_resolver_iterator_061116030307_MADLIFE */
