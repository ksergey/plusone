/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_init_010817234902
#define KSERGEY_init_010817234902

#include <memory>

namespace plusone {
namespace net {
namespace ssl {

class init_base
{
protected:
    class do_init;

    static std::shared_ptr< do_init > instance();
};

/** OpenSSL library initializator */
class init
    : private init_base
{
private:
    std::shared_ptr< do_init > ref_;

public:
    init();
    ~init();
};

} /* namespace ssl */
} /* namespace net */
} /* namespace plusone */

#include <plusone/net/ssl/impl/init.ipp>

#endif /* KSERGEY_init_010817234902 */
