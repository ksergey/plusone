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

namespace plusone {

/** resource open mode */
enum open_mode { read_only, read_write };

/** tag to indicate that the resource must be only created */
struct create_only_tag {};
static const create_only_tag create_only = {};

/** tag to indicate that the resource must be only opened */
struct open_only_tag {};
static const open_only_tag open_only = {};

/**
 * tag to indicate that the resource must be created
 * if already created, it must be opened
 */
struct open_or_create_tag {};
static const open_or_create_tag open_or_create = {};

/** default file create permissions */
static constexpr const int default_permissions = S_IRUSR | S_IWUSR;

/* file exception class */
using file_error = tagged_exception< struct file_tag >;

/**
 * file class.
 * implement simple operations
 */
class file
{
private:
    /* file descriptor */
    int fd_{-1};

public:
    file(const file&) = delete;
    file& operator=(const file&) = delete;

    /** move constuctor */
    file(file&& other) noexcept
        : fd_(other.fd_)
    { other.fd_ = -1; }

    /** move operator */
    file& operator=(file&& other) noexcept
    {
        std::swap(fd_, other.fd_);
        return *this;
    }

    /**
     * construct file.
     * create only file
     */
    file(const create_only_tag&, const std::string& path, open_mode mode = read_write,
            int perms = default_permissions)
    { init(do_create, path, mode, perms); }

    /**
     * construct file.
     * open only file
     */
    file(const open_only_tag&, const std::string& path, open_mode mode = read_write)
    { init(do_open, path, mode); }

    /**
     * construct file.
     * open or create file
     */
    file(const open_or_create_tag&, const std::string& path, open_mode mode = read_write,
            int perms = default_permissions)
    { init(do_open_or_create, path, mode, perms); }

    /** destructor */
    virtual ~file() noexcept
    {
        if (fd_ >= 0) {
            ::close(fd_);
        }
    }

    /** safe bool cast */
    explicit operator bool() const noexcept
    { return fd_ >= 0; }

    /** cast to !bool */
    bool operator!() const noexcept
    { return fd_ < 0; }

    /** return file handle */
    int handle() const noexcept
    { return fd_; }

    /** return size of file */
    std::size_t size() const
    {
        struct stat st;
        if (::fstat(fd_, &st) == -1) {
            throw file_error("failed to read file size (%s)", std::strerror(errno));
        }
        return st.st_size;
    }

    /** truncate file to required size */
    void truncate(std::size_t size)
    {
        if (::ftruncate(fd_, size) == -1) {
            throw file_error("failed to truncate file (%s)", std::strerror(errno));
        }
    }

private:
    /* indicate what to do */
    enum what_do { do_open, do_create, do_open_or_create };

    /* init buffer */
    void init(what_do what, const std::string& path, open_mode mode,
            int perms = default_permissions)
    {
        /* setup open flags */
        int oflags = 0;
        if (mode == read_only) {
            oflags |= O_RDONLY;
        } else if (mode == read_write) {
            oflags |= O_RDWR;
        } else {
            throw std::system_error(EINVAL, std::system_category(),
                    "unknown open mode");
        }

        /* open file descriptor */
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
            throw std::system_error(errno, std::system_category(),
                    "failed to open file " + path);
        }
    }
};

} /* namespace plusone */

#endif /* KSERGEY_file_050117013737 */
