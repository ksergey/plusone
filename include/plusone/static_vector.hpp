/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_static_vector_030217004526
#define KSERGEY_static_vector_030217004526

#include <memory>
#include <plusone/static_vector_base.hpp>

namespace plusone {

/** Vector without reallocations and erasings. */
template< class T, class Allocator = std::allocator< T > >
class static_vector
    : public static_vector_base< T >
{
private:
    using base = static_vector_base< T >;

    Allocator allocator_;

public:
    using allocator_type = Allocator;

    /** Default constructor */
    explicit static_vector(const Allocator& alloc = Allocator())
        : allocator_{alloc}
    {}

    /** Construct vector of specific capacity */
    explicit static_vector(std::size_t capacity, const Allocator& alloc = Allocator())
        : allocator_{alloc}
    {
        auto ptr = allocator_.allocate(capacity);
        if (!ptr) {
            throw_ex< std::bad_alloc >();
        }
        base::reset(ptr, capacity);
    }

    /** Move constructor */
    static_vector(static_vector&& v) noexcept
    {
        swap(v);
    }

    /** Move operator */
    static_vector& operator=(static_vector&& v) noexcept
    {
        if (__likely(this != &v)) {
            swap(v);
        }
        return *this;
    }

    /** Destructor */
    virtual ~static_vector()
    {
        /*
         * Clear first because of destructor of static_vector_base.
         * Need to call destructor for each stored element.
         */
        base::clear();
        /* Deallocate memory */
        allocator_.deallocate(base::data(), base::capacity());
    }

    /** Swap two vectors. */
    void swap(static_vector& other) noexcept
    {
        base::swap(other);
        std::swap(allocator_, other.allocator_);
    }
};

} /* namespace plusone */

#endif /* KSERGEY_static_vector_030217004526 */
