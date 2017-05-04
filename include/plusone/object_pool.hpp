/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_object_pool_070317130629
#define KSERGEY_object_pool_070317130629

#include "compiler.hpp"
#include "static_vector.hpp"

namespace plusone {

template< class T >
class linked_object
    : public T
{
private:
    using this_type = linked_object< T >;

    /* Link to other object */
    this_type* link_{nullptr};

public:
    using value_type = T;

    /* Using parent constructor */
    using T::T;

    /** Return pointer to linked object */
    __force_inline this_type* link() const noexcept
    { return link_; }

    /** Change linked object */
    __force_inline void link(this_type* value) noexcept
    { link_ = value; }

    /** Unlink linked object */
    __force_inline void unlink() noexcept
    { link(nullptr); }
};

/** Object linked queue */
template< class T >
class object_queue final
{
public:
    using value_type = linked_object< T >;

private:
    value_type head_;
    value_type* tail_{&head_};

public:
    object_queue(const object_queue&) = delete;
    object_queue& operator=(const object_queue&) = delete;

    /** Construct empty queue */
    object_queue() = default;

    /** Return true if queue empty */
    __force_inline bool empty() const noexcept
    { return head_.link() == nullptr; }

    /** Push back value */
    __force_inline void push(value_type* value) noexcept
    {
        value->unlink();
        tail_->link(value);
        tail_ = value;
    }

    /** Pop front value */
    __force_inline value_type* pop() noexcept
    {
        value_type* result = head_.link();
        if (__likely(result)) {
            head_.link(result->link());
            result->unlink();
            if (__unlikely(head_.link() == nullptr)) {
                tail_ = &head_;
            }
        }
        return result;
    }

    /** Get queue top value without pop */
    __force_inline value_type* peek() const noexcept
    { return head_.link(); }
};

/** Object pool */
template< class T >
class object_pool final
{
public:
    using value_type = linked_object< T >;

private:
    static_vector< value_type > storage_;
    object_queue< value_type > queue_;

public:
    object_pool(const object_pool&) = delete;
    object_pool& operator=(const object_pool&) = delete;

    /** Construct pool with _count_ available packets */
    object_pool(std::size_t count = 16)
        : storage_{count}
    {
        for (std::size_t i = 0; i < storage_.capacity(); ++i) {
            storage_.emplace_back();
        }
        release_all();
    }

    /** Return first available value without changing owner */
    __force_inline value_type* peek() noexcept
    { return queue_.peek(); }

    /** Return first available value to new owner */
    __force_inline value_type* acquire() noexcept
    { return queue_.pop(); }

    /** Return value to pool */
    __force_inline void release(value_type* value) noexcept
    { queue_.push(value); }

    /** Release all packets in storage */
    __force_inline void release_all()
    {
        /* Clear available values */
        while (queue_.pop()) {}
        /* Append all values in storage to queue */
        for (std::size_t i = 0; i < storage_.capacity(); ++i) {
            queue_.push(&storage_[i]);
        }
    }
};

} /* namespace plusone */

#endif /* KSERGEY_object_pool_070317130629 */
