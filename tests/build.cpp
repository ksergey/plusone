/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <plusone/buffer_base.hpp>
#include <plusone/cacheline_padded.hpp>
#include <plusone/clock.hpp>
#include <plusone/clock_io.hpp>
#include <plusone/compiler.hpp>
#include <plusone/consuming_buffer.hpp>
#include <plusone/exception.hpp>
#include <plusone/expect.hpp>
#include <plusone/file_descriptor.hpp>
#include <plusone/file.hpp>
#include <plusone/fixed_size_memory_pool.hpp>
#include <plusone/fmt.hpp>
#include <plusone/mapped_region.hpp>
#include <plusone/math.hpp>
#include <plusone/ring_buffer.hpp>
#include <plusone/serialization.hpp>
#include <plusone/signal.hpp>
#include <plusone/static_buffer.hpp>
#include <plusone/static_string.hpp>
#include <plusone/static_vector.hpp>
#include <plusone/string_view.hpp>
#include <plusone/tuple.hpp>
#include <plusone/type_traits.hpp>
#include <plusone/net/address_v4.hpp>
#include <plusone/net/common.hpp>
#include <plusone/net/endpoint.hpp>
#include <plusone/net/mmap_rx.hpp>
#include <plusone/net/protocol.hpp>
#include <plusone/net/resolver.hpp>
#include <plusone/net/result.hpp>
#include <plusone/net/socket.hpp>
#include <plusone/net/socket_options.hpp>
#include <plusone/net/tcp.hpp>

#if 0
#include <plusone/net/ssl/common.hpp>
#include <plusone/net/ssl/context_base.hpp>
#include <plusone/net/ssl/context.hpp>
#include <plusone/net/ssl/init.hpp>
#include <plusone/net/ssl/result.hpp>
#include <plusone/net/ssl/stream.hpp>
#endif

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
