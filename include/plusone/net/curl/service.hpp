/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_service_250517184735
#define KSERGEY_service_250517184735

#include <sys/epoll.h>
#include <vector>
#include "detail/session.hpp"

namespace plusone {
namespace net {
namespace curl {

/** CURL reactor service */
class service
{
private:
    /* CURL multi session */
    detail::multi_session session_;
    /* EPoll descriptor */
    int epoll_fd_{-1};
    /* Events placeholder */
    std::vector< epoll_event > events_;

public:
    service(const service&) = delete;
    service& operator=(const service&) = delete;

    /**
     * Service constructor
     * @param[in] max_connections is max number of total connections
     */
    explicit service(std::size_t max_connections = 16);

    /** Service destructor */
    ~service() noexcept;

private:
    /* CURL internal socket callback */
    static int socket_callback(CURL* session, curl_socket_t socket, int action,
            void* user_data, void* socket_data);
};

} /* namespace curl */
} /* namespace net */
} /* namespace plusone */

#include "service_impl.hpp"

#endif /* KSERGEY_service_250517184735 */
