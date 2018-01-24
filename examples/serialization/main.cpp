/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <plusone/serialization.hpp>
#include <plusone/exception.hpp>

namespace s = plusone::serialization;

using plusone::throw_ex;

struct point_config
{
    int x;
    int y;
    int z;
    float n;

    template< class InputOutputT >
    void serialize(InputOutputT& io)
    {
        io
            & s::required("x", x, s::inside(0, 1000))
            & s::required("y", y)
            & s::optional("z", z, 0)
            & s::optional("n", n, 4.0);
    }
};

struct object_config
{
    std::string field1;
    int field2;

    template< class InputOutputT >
    void serialize(InputOutputT& io)
    {
        io
            & s::required("field1", field1, s::not_empty_string())
            & s::optional("field2", field2, 100, s::greater_equal(100));
    }
};

struct sample_config
{
    int value1;
    plusone::static_string< 10 > value2;
    std::vector< int > value3;
    unsigned value4;
    object_config value5;
    plusone::static_vector< point_config > value6;
    std::array< int, 4 > value7;

    template< class InputOutputT >
    void serialize(InputOutputT& io)
    {
        io
            & s::required("value1", value1)
            & s::required("value2", value2, s::not_empty_string())
            & s::required("value3", value3)
            & s::optional("value4", value4, 33, s::one_of({33, 44, 55}))
            & s::required("value5", value5)
            & s::required("value6", value6)
            & s::required("value7", value7);
    }
};

int main(int, char*[])
{
    try {
        sample_config config;

        std::ifstream file{"sample.json"};
        if (!file) {
            throw_ex< std::runtime_error >("Failed to read file");
        }
        s::from_stream(file, config);

        std::cout << s::to_json(config, 4) << '\n';

        config.value6.clear();
        config.value2 = "replaced";

        std::cout << s::to_json(config, 4) << '\n';

    } catch (const std::exception& e) {
        std::cout << "ERROR: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
