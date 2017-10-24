/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_queue_241017180003
#define KSERGEY_queue_241017180003

#include <plusone/type_traits.hpp>
#include <plusone/compiler.hpp>

namespace plusone {
namespace detail {

template< class T, class = void >
struct is_queue_entry
    : std::false_type
{};
template< class T >
struct is_queue_entry< T, void_t< std::is_same< decltype(std::declval< T >().link), T >() > >
    : std::true_type
{};

template< class T >
class queue
{
private:
    T head_;
    T* tail_{&head_};

public:
    static_assert( is_queue_entry< T >(), "T not met queue entry requirements" );

    using value_type = T;
    using pointer = T*;

    queue(const queue&) = delete;
    queue& operator=(const queue&) = delete;

    queue()
    {
        head_.link = nullptr;
    }

    __force_inline bool empty() const noexcept
    {
        return head_.link == nullptr;
    }

    __force_inline void push(pointer v) noexcept
    {
        v->link = nullptr;
        tail_->link = v;
        tail_ = v;
    }

    __force_inline pointer pop() noexcept
    {
        pointer result = head_.link;
        if (__likely(result)) {
            head_.link = result->link;
            result->link = nullptr;
            if (__unlikely(head_.link == nullptr)) {
                tail_ = &head_;
            }
        }
        return result;
    }
};

} /* namespace detail */
} /* namespace plusone */

#endif /* KSERGEY_queue_241017180003 */
