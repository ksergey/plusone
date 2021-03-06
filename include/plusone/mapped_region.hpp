/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_mapped_region_050117020149
#define KSERGEY_mapped_region_050117020149

#include <plusone/exception.hpp>
#include <plusone/file.hpp>

namespace plusone {

/** Mmap error type */
using mmap_error = tagged_exception< struct mmap_tag >;

/** Memory mapped region */
class mapped_region
{
private:
    /* Pointer to mapped data */
    char* data_{nullptr};
    /* Size of mapped data */
    std::size_t size_{0};

public:
    struct options
    {
        /** Shared or private mapping flag */
        bool shared{true};
        /** Mapping mode */
        open_mode mode{read_write};
        /** Populate page tables */
        bool prefault{false};
        /** Lock the pages of the mapped region into memory */
        bool locked{false};
        /** Fix map at this adress, if not nullptr */
        void* address{nullptr};

        options()
        {}
    };

    /** Move constructor */
    mapped_region(mapped_region&& other) noexcept;

    /** Move operator */
    mapped_region& operator=(mapped_region&& other) noexcept;

    /** Construct non-initialized mapped region */
    mapped_region() = default;

    /** Construct the mapped region from file descriptor */
    mapped_region(int fd, std::size_t file_size, const options& opts = options());

    /** Construct the mapped region from file descriptor */
    explicit mapped_region(int fd, const options& opts = options());

    /** Construct the mapped region from file object */
    mapped_region(file& f, std::size_t file_size, const options& opts = options());

    /** Construct the mapped region from file object */
    explicit mapped_region(file& f, const options& opts = options());

    /** Destructor */
    virtual ~mapped_region() noexcept;

    /** Swap content with another object */
    void swap(mapped_region& other) noexcept;

    /** Return true if mapped region initialized */
    explicit operator bool() const noexcept
    {
        return data_ != nullptr;
    }

    /** Return true if mapped region non-initialized */
    bool operator!() const noexcept
    {
        return data_ == nullptr;
    }

    /** Return pointer to mapped region data */
    const char* data() const noexcept
    {
        return data_;
    }

    /** Return pointer to mapped region data */
    char* data() noexcept
    {
        return data_;
    }

    /** Return region size */
    std::size_t size() const noexcept
    {
        return size_;
    }

    /** Fill region memory with a constant byte */
    void fill(int c = 0);
};

} /* namespace plusone */

#include <plusone/impl/mapped_region.ipp>

#endif /* KSERGEY_mapped_region_050117020149 */
