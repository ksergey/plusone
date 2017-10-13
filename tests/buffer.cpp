/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <plusone/static_buffer.hpp>
#include <plusone/consuming_buffer.hpp>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <vector>

class test
    : public plusone::static_buffer
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

TEST_CASE("static_buffer")
{
    using plusone::buffer_size;
    using plusone::buffer_cast;

    char storage[64];

    plusone::static_buffer buffer{storage, sizeof(storage)};

    REQUIRE(buffer.capacity() == 64);
    REQUIRE(buffer.size() == 0);
    REQUIRE(buffer_size(buffer.data()) == 0);

    auto b = buffer.prepare();
    REQUIRE(buffer_size(b) == 64);

    int sz = std::snprintf(buffer_cast< char* >(b), buffer_size(b), "fuck you");
    buffer.commit(sz);

    REQUIRE(buffer.size() == sz);

    auto data = buffer.data();
    REQUIRE(buffer_size(data) == sz);
    REQUIRE(buffer_cast< const char* >(data)[0] == 'f');
    REQUIRE(buffer_cast< const char* >(data)[1] == 'u');
    REQUIRE(buffer_cast< const char* >(data)[2] == 'c');
    REQUIRE(buffer_cast< const char* >(data)[3] == 'k');

    b = buffer.prepare();
    REQUIRE(buffer_size(b) == (64 - sz));

    buffer.consume(5);
    REQUIRE(buffer.size() == (sz - 5));

    data = buffer.data();
    REQUIRE(buffer_size(data) == 3);
    REQUIRE(buffer_cast< const char* >(data)[0] == 'y');
    REQUIRE(buffer_cast< const char* >(data)[1] == 'o');
    REQUIRE(buffer_cast< const char* >(data)[2] == 'u');

    b = buffer.prepare();
    REQUIRE(buffer_size(b) == 61);

    buffer.consume(3);
    b = buffer.prepare();
    REQUIRE(buffer_size(b) == 64);
}

TEST_CASE("static_buffer custom")
{
    using plusone::buffer_size;
    using plusone::buffer_cast;

    test buffer{64};

    REQUIRE(buffer.capacity() == 64);
    REQUIRE(buffer.size() == 0);
    REQUIRE(buffer_size(buffer.data()) == 0);

    auto b = buffer.prepare();
    REQUIRE(buffer_size(b) == 64);

    int sz = std::snprintf(buffer_cast< char* >(b), buffer_size(b), "fuck you");
    buffer.commit(sz);

    REQUIRE(buffer.size() == sz);

    auto data = buffer.data();
    REQUIRE(buffer_size(data) == sz);
    REQUIRE(buffer_cast< const char* >(data)[0] == 'f');
    REQUIRE(buffer_cast< const char* >(data)[1] == 'u');
    REQUIRE(buffer_cast< const char* >(data)[2] == 'c');
    REQUIRE(buffer_cast< const char* >(data)[3] == 'k');

    b = buffer.prepare();
    REQUIRE(buffer_size(b) == (64 - sz));

    buffer.consume(5);
    REQUIRE(buffer.size() == (sz - 5));

    data = buffer.data();
    REQUIRE(buffer_size(data) == 3);
    REQUIRE(buffer_cast< const char* >(data)[0] == 'y');
    REQUIRE(buffer_cast< const char* >(data)[1] == 'o');
    REQUIRE(buffer_cast< const char* >(data)[2] == 'u');

    b = buffer.prepare();
    REQUIRE(buffer_size(b) == 61);

    buffer.consume(3);
    b = buffer.prepare();
    REQUIRE(buffer_size(b) == 64);
}

TEST_CASE("Consuming buffer")
{
    using plusone::buffer_size;
    using plusone::buffer_cast;

    std::string message = "hello";

    plusone::consuming_buffer buffer{message.data(), message.size()};
    REQUIRE(buffer.size() == 5);

    auto d = buffer.data();
    REQUIRE(buffer_size(d) == 5);
    REQUIRE(buffer_cast< const char* >(d)[0] == 'h');
    REQUIRE(buffer_cast< const char* >(d)[1] == 'e');
    REQUIRE(buffer_cast< const char* >(d)[2] == 'l');
    REQUIRE(buffer_cast< const char* >(d)[3] == 'l');
    REQUIRE(buffer_cast< const char* >(d)[4] == 'o');

    buffer.consume(2);
    REQUIRE(buffer.size() == 3);

    d = buffer.data();
    REQUIRE(buffer_size(d) == 3);
    REQUIRE(buffer_cast< const char* >(d)[0] == 'l');
    REQUIRE(buffer_cast< const char* >(d)[1] == 'l');
    REQUIRE(buffer_cast< const char* >(d)[2] == 'o');
}
