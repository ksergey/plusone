/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_init_010817235145
#define KSERGEY_init_010817235145

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <plusone/expect.hpp>
#include <plusone/compiler.hpp>

namespace plusone {
namespace net {
namespace ssl {

struct init_base::do_init
{
    do_init(const do_init&) = delete;
    do_init& operator=(const do_init&) = delete;

    do_init()
    {
        ::SSL_library_init();
        ::SSL_load_error_strings();
        ::OpenSSL_add_all_algorithms();
    }

    ~do_init()
    {
        ERR_free_strings();
    }
};

__force_inline std::shared_ptr< init_base::do_init > init_base::instance()
{
    static std::shared_ptr< do_init > value(new do_init);
    return value;
}

__force_inline init::init()
    : ref_{instance()}
{
    __expect( ref_ );
}

__force_inline init::~init()
{}

} /* namespace ssl */
} /* namespace net */
} /* namespace plusone */
#endif /* KSERGEY_init_010817235145 */
