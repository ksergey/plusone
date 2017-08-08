/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <vector>
#include <plusone/static_buffer_base.hpp>

class test
    : public plusone::static_buffer_base
{
private:
    std::vector< char > storage_;

public:
    explicit test(std::size_t size)
    {
        storage_.resize(size);
        reset(storage_.data(), storage_.size());
    }
};

TEST_CASE("static_buffer_base")
{
    char storage[64];

    plusone::static_buffer_base buffer{storage, sizeof(storage)};

    REQUIRE(buffer.capacity() == 64);
    REQUIRE(buffer.size() == 0);
    REQUIRE(buffer.data().size == 0);

    auto b = buffer.prepare();
    REQUIRE(b.size == 64);

    int sz = std::snprintf(b.data, b.size, "fuck you");
    buffer.commit(sz);

    REQUIRE(buffer.size() == sz);
    REQUIRE(buffer.data().size == sz);

    auto data = buffer.data();
    REQUIRE(data.data[0] == 'f');
    REQUIRE(data.data[1] == 'u');
    REQUIRE(data.data[2] == 'c');
    REQUIRE(data.data[3] == 'k');

    b = buffer.prepare();
    REQUIRE(b.size == (64 - sz));

    buffer.consume(5);
    REQUIRE(buffer.size() == (sz - 5));

    data = buffer.data();
    REQUIRE(data.data[0] == 'y');
    REQUIRE(data.data[1] == 'o');
    REQUIRE(data.data[2] == 'u');
    REQUIRE(data.size == 3);

    b = buffer.prepare();
    REQUIRE(b.size == 61);

    buffer.consume(3);
    b = buffer.prepare();
    REQUIRE(b.size == 64);
}

TEST_CASE("static_buffer_base custom")
{
    test buffer{64};

    REQUIRE(buffer.capacity() == 64);
    REQUIRE(buffer.size() == 0);
    REQUIRE(buffer.data().size == 0);

    auto b = buffer.prepare();
    REQUIRE(b.size == 64);

    int sz = std::snprintf(b.data, b.size, "fuck you");
    buffer.commit(sz);

    REQUIRE(buffer.size() == sz);
    REQUIRE(buffer.data().size == sz);

    auto data = buffer.data();
    REQUIRE(data.data[0] == 'f');
    REQUIRE(data.data[1] == 'u');
    REQUIRE(data.data[2] == 'c');
    REQUIRE(data.data[3] == 'k');

    b = buffer.prepare();
    REQUIRE(b.size == (64 - sz));

    buffer.consume(5);
    REQUIRE(buffer.size() == (sz - 5));

    data = buffer.data();
    REQUIRE(data.data[0] == 'y');
    REQUIRE(data.data[1] == 'o');
    REQUIRE(data.data[2] == 'u');
    REQUIRE(data.size == 3);

    b = buffer.prepare();
    REQUIRE(b.size == 61);

    buffer.consume(3);
    b = buffer.prepare();
    REQUIRE(b.size == 64);
}
