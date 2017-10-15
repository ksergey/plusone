/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_websocket_client_141017160632
#define KSERGEY_websocket_client_141017160632

#include <vector>
#include <plusone/consuming_buffer.hpp>
#include <plusone/static_buffer.hpp>
#include "websocket_client_base.hpp"

namespace app {

using namespace plusone;

struct open
{};

struct close
{};

struct message
{};

template< class Stream >
class websocket_client
    : public websocket_client_base
{
private:
    /* Underlyin stream. */
    Stream stream_;

    /* Indicates wait response on upgrade request. */
    bool waiting_upgrade_response_{false};

    std::vector< std::uint8_t > storage_;
    static_buffer buffer_;

public:
    websocket_client()
        : storage_(1024 * 1024)
        , buffer_{storage_.data(), storage_.size()}
    {}

    /* TODO: return start_result ? */
    bool start(Stream&& stream, const string_view& host, const string_view& resource = "/",
            const string_view& protocol = {});

    template< class Handler >
    bool poll(Handler&& h);

private:
    bool send_all(const void* data, std::size_t size);

    template< class Handler >
    parse_result parse_buffer(Handler&& h);
};

template< class Stream >
__force_inline bool websocket_client< Stream >::start(Stream&& stream, const string_view& host, const string_view& resource,
            const string_view& protocol)
{
    stream_ = std::move(stream);
    __expect( stream_ );
    waiting_upgrade_response_ = true;
    auto upgrade = prepare_upgrade_request(resource, host, host);
    return send_all(upgrade.data(), upgrade.size());
}

template< class Stream >
template< class Handler >
__force_inline bool websocket_client< Stream >::poll(Handler&& h)
{
    __expect( stream_ );

    auto rc = stream_.recv(buffer_.prepare());
    if (__likely(rc)) {
        buffer_.commit(rc.bytes());

        while (buffer_.size() > 0) {
            if (__likely(parse_buffer(std::forward< Handler >(h)) != success)) {
                break;
            }
        }

        return true;
    } else {
        if (__unlikely(!rc.again())) {
            stream_.close();
            h(close{});
        }
        return false;
    }
    return false;
}

template< class Stream >
__force_inline bool websocket_client< Stream >::send_all(const void* data, std::size_t size)
{
    plusone::consuming_buffer buffer{data, size};
    while (buffer.size() > 0) {
        auto rc = stream_.send(buffer.data());
        if (__likely(rc)) {
            buffer.consume(rc.bytes());
        } else if (__unlikely(!rc.again())) {
            break;
        }
    }
    return buffer.size() == 0;
}

template< class Stream >
template< class Handler >
__force_inline websocket_client_base::parse_result websocket_client< Stream >::parse_buffer(Handler&& h)
{
    if (__likely(!waiting_upgrade_response_)) {
        /* parse headers */
    } else {
        if (check_upgrade_response(buffer_) == success) {
            waiting_upgrade_response_ = false;
            h(open{});
            return success;
        }
    }

    return not_enought_data;
}

} /* namespace app */

#endif /* KSERGEY_websocket_client_141017160632 */
