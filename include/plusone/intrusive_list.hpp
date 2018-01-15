/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_intrusive_list_150118151708
#define KSERGEY_intrusive_list_150118151708

#include <cassert>
#include <iterator>
#include <type_traits>

// Actual almost copy from
// https://github.com/electronicarts/EASTL/blob/master/include/EASTL/intrusive_list.h

namespace plusone {

struct intrusive_list_node
{
    intrusive_list_node* next;
    intrusive_list_node* prev;
};

static_assert( std::is_pod< intrusive_list_node >(), "" );

namespace detail {

template< class T, class Pointer, class Reference >
class intrusive_list_iterator
    : public std::iterator< std::bidirectional_iterator_tag, T, std::ptrdiff_t, Pointer, Reference >
{
private:
    Pointer node_{nullptr};

public:
    using value_type = T;
    using pointer = Pointer;
    using reference = Reference;

    intrusive_list_iterator() = default;
    ~intrusive_list_iterator() = default;

    template< class Iterator >
    intrusive_list_iterator(const Iterator& it) noexcept
        : node_{&(*it)}
    {
        // Conversation between const/non-const iterators
        static_assert( std::is_same<
                typename std::decay< value_type >::type,
                typename std::decay< typename Iterator::value_type >::type
            >(), "" );
    }

    explicit intrusive_list_iterator(Pointer node) noexcept
        : node_{node}
    {}

    intrusive_list_node& node() noexcept
    {
        return static_cast< intrusive_list_node& >(*node_);
    }

    const intrusive_list_node& node() const noexcept
    {
        return static_cast< const intrusive_list_node& >(*node_);
    }

    reference operator*() const noexcept
    {
        return *node_;
    }

    pointer operator->() const noexcept
    {
        return node_;
    }

    intrusive_list_iterator& operator++() noexcept
    {
        node_ = static_cast< T* >(node_->next);
        return *this;
    }

    intrusive_list_iterator& operator--() noexcept
    {
        node_ = static_cast< T* >(node_->prev);
        return *this;
    }

    intrusive_list_iterator operator++(int) noexcept
    {
        intrusive_list_iterator it{*this};
        node_ = static_cast< T* >(node_->next);
        return it;
    }

    intrusive_list_iterator operator--(int) noexcept
    {
        intrusive_list_iterator it{*this};
        node_ = static_cast< T* >(node_->prev);
        return it;
    }

    friend bool operator==(const intrusive_list_iterator& l, const intrusive_list_iterator& r) noexcept
    {
        return l.node_ == r.node_;
    }

    friend bool operator!=(const intrusive_list_iterator& l, const intrusive_list_iterator& r) noexcept
    {
        return l.node_ != r.node_;
    }
};

} /* namespace detail */

class intrusive_list_base
{
protected:
    intrusive_list_node anchor_;

public:
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    intrusive_list_base()
    {
        anchor_.next = anchor_.prev = &anchor_;
    }

    ~intrusive_list_base() = default;

    /// Calculate size of list (O(n))
    /// @return Number of nodes in list
    size_type size() const noexcept
    {
        auto* p = &anchor_;
        size_type n = static_cast< size_type >(-1);

        do {
            ++n;
            p = p->next;
        } while (p != &anchor_);
        return n;
    }

    /// @return True if list empty
    bool empty() const noexcept
    {
        return anchor_.prev == &anchor_;
    }

    void clear() noexcept
    {
        anchor_.next = anchor_.prev = &anchor_;
    }

    void pop_front() noexcept
    {
        assert( !empty() );

        anchor_.next->next->prev = &anchor_;
        anchor_.next = anchor_.next->next;
    }

    void pop_back() noexcept
    {
        assert( !empty() );

        anchor_.prev->prev->next = &anchor_;
        anchor_.prev = anchor_.prev->prev;
    }
};

// TODO: implement move semantic
// TODO: Make node anchor as free element
template< class T >
class intrusive_list
    : public intrusive_list_base
{
public:
    using intrusive_list_base::size_type;
    using intrusive_list_base::difference_type;
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = detail::intrusive_list_iterator< T, T*, T& >;
    using const_iterator = detail::intrusive_list_iterator< T, const T*, const T& >;
    using reverse_iterator = std::reverse_iterator< iterator >;
    using const_reverse_iterator = std::reverse_iterator< const_iterator >;

    intrusive_list(const intrusive_list&) = delete;
    intrusive_list& operator=(const intrusive_list&) = delete;
    intrusive_list() = default;

    iterator begin() noexcept
    {
        return iterator{static_cast< T* >(anchor_.next)};
    }

    const_iterator begin() const noexcept
    {
        return const_iterator{static_cast< const T* >(anchor_.next)};
    }

    const_iterator cbegin() const noexcept
    {
        return const_iterator{static_cast< const T* >(anchor_.next)};
    }

    iterator end() noexcept
    {
        return iterator{static_cast< T* >(&anchor_)};
    }

    const_iterator end() const noexcept
    {
        return const_iterator{static_cast< const T* >(&anchor_)};
    }

    const_iterator cend() const noexcept
    {
        return const_iterator{static_cast< const T* >(&anchor_)};
    }

    reverse_iterator rbegin() noexcept
    {
        return reverse_iterator{this->begin()};
    }

    const_reverse_iterator rbegin() const noexcept
    {
        return const_reverse_iterator{this->begin()};
    }

    const_reverse_iterator crbegin() const noexcept
    {
        return const_reverse_iterator{this->cbegin()};
    }

    reverse_iterator rend() noexcept
    {
        return reverse_iterator{this->end()};
    }

    const_reverse_iterator rend() const noexcept
    {
        return const_reverse_iterator{this->end()};
    }

    const_reverse_iterator crend() const noexcept
    {
        return const_reverse_iterator{this->cend()};
    }

    reference front() noexcept
    {
        assert( !empty() );
        return *static_cast< T* >(anchor_.next);
    }

    const_reference front() const noexcept
    {
        assert( !empty() );
        return *static_cast< const T* >(anchor_.next);
    }

    reference back() noexcept
    {
        assert( !empty() );
        return *static_cast< T* >(anchor_.prev);
    }

    const_reference back() const noexcept
    {
        assert( !empty() );
        return *static_cast< const T* >(anchor_.prev);
    }

    void push_front(value_type& v) noexcept
    {
        v.next = anchor_.next;
        v.prev = &anchor_;
        anchor_.next = &v;
        v.next->prev = &v;
    }

    void push_back(value_type& v) noexcept
    {
        v.prev = anchor_.prev;
        v.next = &anchor_;
        anchor_.prev = &v;
        v.prev->next = &v;
    }

    iterator insert(const_iterator pos, value_type& v) noexcept
    {
        intrusive_list_node& next = const_cast< T& >(*pos);
        intrusive_list_node& prev = *static_cast< T* >(next.prev);
        prev.next = next.prev = &v;
        v.prev = &prev;
        v.next = &next;

        return iterator{&v};
    }

    iterator erase(const_iterator pos) noexcept
    {
        intrusive_list_node& prev = *static_cast< T* >(pos->prev);
        intrusive_list_node& next = *static_cast< T* >(pos->next);
        prev.next = &next;
        next.prev = &prev;

        return iterator{static_cast< T* >(&next)};
    }

    static void remove(value_type& v) noexcept
    {
        intrusive_list_node& prev = *v.prev;
        intrusive_list_node& next = *v.next;
        prev.next = &next;
        next.prev = &prev;
    }
};

} /* namespace plusone */

#endif /* KSERGEY_intrusive_list_150118151708 */
