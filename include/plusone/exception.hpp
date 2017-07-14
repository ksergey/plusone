/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_exception_051216171739
#define KSERGEY_exception_051216171739

#include <string>
#include <exception>

namespace plusone {

/** Exception class */
class exception
    : public std::exception
{
private:
    std::string what_;

public:
    /** Empty string constructor */
    exception() = default;

    /** Printf-like message formatter */
    template< class... Args >
    explicit exception(const char* fmt, const Args&... args);

    /** Destructor */
    virtual ~exception() noexcept = default;

    /** Return exception description */
    virtual const char* what() const noexcept override;
};

/** Make unique exception class */
template< class... Tags >
struct tagged_exception
    : exception
{
    using exception::exception;
};

} /* namespace plusone */

#include "impl/exception.ipp"

#endif /* KSERGEY_exception_051216171739 */
