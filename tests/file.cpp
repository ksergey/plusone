/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <plusone/file.hpp>

const std::string test_file = "test.file";

TEST_CASE("file default constructor")
{
    plusone::file file;
    REQUIRE(!file);
    REQUIRE(!file.handle());
    REQUIRE(file.handle().get() == -1);
}

TEST_CASE("file create/truncate/reopen")
{
    ::unlink(test_file.c_str());
    plusone::file file{plusone::create_only, test_file};
    REQUIRE(file);
    REQUIRE(file.handle());
    REQUIRE(file.handle().get() != -1);
    REQUIRE(file.size() == 0);

    file.truncate(512);
    REQUIRE(file.size() == 512);

    file = plusone::file{};
    REQUIRE(!file);

    file = plusone::file{plusone::open_only, test_file};
    REQUIRE(file);
    REQUIRE(file.size() == 512);
}

void open_file(plusone::file& result)
{
    result = plusone::file{plusone::open_only, test_file};
}

TEST_CASE("file open/size/truncate error")
{
    ::unlink(test_file.c_str());
    plusone::file file;

    CHECK_THROWS_AS(open_file(file), const std::exception&);
    REQUIRE(!file);
    CHECK_THROWS_AS(file.size(), const std::exception&);
    CHECK_THROWS_AS(file.truncate(128), const std::exception&);
}
