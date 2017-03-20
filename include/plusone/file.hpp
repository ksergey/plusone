/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_file_050117013737
#define KSERGEY_file_050117013737

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <string>
#include <utility>
#include "exception.hpp"
#include "compiler.hpp"

namespace plusone {

/** Resource open mode */
enum open_mode { read_only, read_write };

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

/* File exception class */
using file_error = tagged_exception< struct file_tag >;

/**
 * File class
 * implements simple operations
 */
class file
{
private:
    /* File descriptor */
    int fd_{-1};

public:
    file(const file&) = delete;
    file& operator=(const file&) = delete;

    /** Move constuctor */
    __force_inline file(file&& other) noexcept
        : fd_(other.fd_)
    { other.fd_ = -1; }

    /** Move operator */
    __force_inline file& operator=(file&& other) noexcept
    { std::swap(fd_, other.fd_); return *this; }

    /** Construct file */
    file(const create_only_tag&, const std::string& path, open_mode mode = read_write,
            int perms = default_permissions)
    { init(do_create, path, mode, perms); }

    /** Construct file */
    file(const open_only_tag&, const std::string& path, open_mode mode = read_write)
    { init(do_open, path, mode); }

    /** Construct file */
    file(const open_or_create_tag&, const std::string& path, open_mode mode = read_write,
            int perms = default_permissions)
    { init(do_open_or_create, path, mode, perms); }

    /** Destructor */
    virtual ~file() noexcept
    {
        if (fd_ >= 0) {
            ::close(fd_);
        }
    }

    /** Safe bool cast */
    __force_inline explicit operator bool() const noexcept
    { return fd_ >= 0; }

    /** Cast to !bool */
    __force_inline bool operator!() const noexcept
    { return fd_ < 0; }

    /** Return file handle */
    __force_inline int handle() const noexcept
    { return fd_; }

    /** Return size of file */
    __force_inline std::size_t size() const
    {
        struct stat st;
        if (::fstat(fd_, &st) == -1) {
            throw file_error{"Read file size error (%s)", std::strerror(errno)};
        }
        return st.st_size;
    }

    /** Truncate file to required size */
    __force_inline void truncate(std::size_t size)
    {
        if (::ftruncate(fd_, size) == -1) {
            throw file_error{"Truncate file error(%s)", std::strerror(errno)};
        }
    }

private:
    enum what_do { do_open, do_create, do_open_or_create };

    __force_inline void init(what_do what, const std::string& path, open_mode mode,
            int perms = default_permissions)
    {
        /* Setup open flags */
        int oflags = 0;
        if (mode == read_only) {
            oflags |= O_RDONLY;
        } else if (mode == read_write) {
            oflags |= O_RDWR;
        } else {
            throw file_error{"Unknown open mode (%s)", std::strerror(EINVAL)};
        }

        /* Open file */
        switch (what) {
            case do_open:
                {
                    fd_ = ::open(path.c_str(), oflags, perms);
                }
                break;
            case do_create:
                {
                    oflags |= (O_CREAT | O_EXCL);
                    fd_ = ::open(path.c_str(), oflags, perms);
                    if (fd_ >= 0) {
                        ::fchmod(fd_, perms);
                    }
                }
                break;
            case do_open_or_create:
                {
                    while (true) {
                        fd_ = ::open(path.c_str(), oflags | O_CREAT | O_EXCL, perms);
                        if (fd_ >= 0) {
                            ::fchmod(fd_, perms);
                        } else if (errno == EEXIST) {
                            fd_ = ::open(path.c_str(), oflags, perms);
                            if (fd_ < 0 && errno == ENOENT) {
                                continue;
                            }
                        }
                        break;
                    }
                }
                break;
            default:
                break;
        }

        if (fd_ < 0) {
            throw file_error{"Open file \"%s\" error (%s)}",
                path.c_str(), std::strerror(errno)};
        }
    }
};

} /* namespace plusone */

#endif /* KSERGEY_file_050117013737 */
