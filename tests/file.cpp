/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <plusone/file.hpp>

const std::string test_file = "test.file";

TEST_CASE("File default constructor")
{
    plusone::file file;
    REQUIRE(!file);
    REQUIRE(!file.descriptor());
    REQUIRE(file.descriptor().get() == -1);
}

TEST_CASE("File create/truncate/reopen")
{
    ::unlink(test_file.c_str());
    plusone::file file{plusone::create_only, test_file};
    REQUIRE(file);
    REQUIRE(file.descriptor());
    REQUIRE(file.descriptor().get() != -1);
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

TEST_CASE("File open/size/truncate error")
{
    ::unlink(test_file.c_str());
    plusone::file file;

    CHECK_THROWS_AS(open_file(file), const std::exception&);
    REQUIRE(!file);
    CHECK_THROWS_AS(file.size(), const std::exception&);
    CHECK_THROWS_AS(file.truncate(128), const std::exception&);
}

TEST_CASE("Lock file")
{
    ::unlink(test_file.c_str());
    plusone::file file1{plusone::open_or_create, test_file};

    REQUIRE(file1);
    REQUIRE(file1.try_lock());

    plusone::file file2{plusone::open_only, test_file};
    REQUIRE(file2);
    REQUIRE(!file2.try_lock());

    file1.unlock();

    REQUIRE(file2.try_lock());
}

TEST_CASE("File dup")
{
    ::unlink(test_file.c_str());
    plusone::file file1{plusone::create_only, test_file};
    REQUIRE(file1);
    REQUIRE(file1.descriptor());
    REQUIRE(file1.descriptor().get() != -1);

    auto file2 = file1.dup();
    REQUIRE(file2);
    REQUIRE(file2.descriptor());
    REQUIRE(file2.descriptor().get() != -1);

    REQUIRE(file1.descriptor().get() != file2.descriptor().get());
    REQUIRE(file1.size() == 0);
    REQUIRE(file2.size() == 0);

    file1.truncate(128);
    REQUIRE(file1.size() == 128);
    REQUIRE(file2.size() == 128);
}
