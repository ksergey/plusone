/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_file_050117013737
#define KSERGEY_file_050117013737

#include <fcntl.h>
#include <string>
#include <plusone/file_descriptor.hpp>
#include <plusone/exception.hpp>

namespace plusone {

/** Resource open mode */
enum open_mode
{
    read_only,
    read_write
};

/** Tag to indicate that the resource must be only created */
struct create_only_tag {};
static const create_only_tag create_only = {};

/** Tag to indicate that the resource must be only opened */
struct open_only_tag {};
static const open_only_tag open_only = {};

/**
 * Tag to indicate that the resource must be created
 * if already created, it must be opened
 */
struct open_or_create_tag {};
static const open_or_create_tag open_or_create = {};

/** Default file create permissions */
static constexpr const int default_permissions = S_IRUSR | S_IWUSR;

/** File exception class */
using file_error = tagged_exception< struct file_tag >;

/**
 * File class
 * implements simple operations
 */
class file
{
private:
    file_descriptor fd_;

public:
    /** Move constuctor */
    file(file&& other) noexcept;

    /** Move operator */
    file& operator=(file&& other) noexcept;

    /** Default constructor */
    file() = default;

    /** Construct file */
    file(const create_only_tag&, const std::string& path, open_mode mode = read_write,
            int perms = default_permissions);

    /** Construct file */
    file(const open_only_tag&, const std::string& path, open_mode mode = read_write);

    /** Construct file */
    file(const open_or_create_tag&, const std::string& path, open_mode mode = read_write,
            int perms = default_permissions);

    /** Destructor */
    virtual ~file() noexcept = default;

    /** Safe bool cast */
    explicit operator bool() const noexcept;

    /** Cast to !bool */
    bool operator!() const noexcept;

    /** Return file handle */
    const file_descriptor& descriptor() const noexcept;

    /** Return size of file */
    std::size_t size() const;

    /** Truncate file to required size */
    void truncate(std::size_t size);

    /** Swap two files */
    void swap(file& v) noexcept;

    /** Lock file, place an exclusive lock */
    void lock();

    /** Try to lock file */
    bool try_lock();

    /** Unlock file */
    void unlock();

    /** Lock file, place a shared lock */
    void lock_shared();

    /** Lock file, place a shared lock */
    bool try_lock_shared();

    /** Unlock file */
    void unlock_shared();

    /** Duplicate file descriptor */
    file dup() const;

    /** Create temporary file */
    static file temporary();

private:
    file(file_descriptor fd);

    enum what_do
    {
        do_open,
        do_create,
        do_open_or_create
    };

    void init(what_do what, const std::string& path, open_mode mode,
            int perms = default_permissions);
    void do_lock(int op);
    bool do_try_lock(int op);
};

} /* namespace plusone */

#include <plusone/impl/file.ipp>

#endif /* KSERGEY_file_050117013737 */
