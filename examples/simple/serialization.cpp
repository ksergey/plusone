/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <string>
#include <iostream>
#include <plusone/serialization/all.hpp>

namespace f = plusone::serialization;

struct item
{
    int key;
    int value;

    template< class Archive >
    void serialize(Archive& ar)
    {
        ar
            & f::required("key", key, f::inside(0, 10))
            & f::optional("value", value, -1, [](auto v) { if (v > 100) { throw std::runtime_error("bad val"); } });
    }
};

struct message
{
    std::string from;
    std::string text;
    int id;
    std::vector< item > data;

    template< class Archive >
    void serialize(Archive& ar)
    {
        ar
            & f::required("from", from, f::not_empty_string())
            & f::optional("text", text, "oOoOoOoOoOoO")
            & f::required("id", id, f::one_of({ 1, 2, 10, 3, 18, 32, 15 }))
            & f::required("data", data);
    }
};

void parse(const std::string& str);

int main(int argc, char* argv[])
{
    message msg;
    msg.from = "Bill";
    msg.text = "Shitty";
    msg.id = 15;
    msg.data = {
        { 0, 1 },
        { 1, 5 },
        { 2, 11 },
        { 3, 9 }
    };

    f::json object;
    f::output out{object};
    f::to_stream(std::cout, msg, 3);
    std::cout << '\n';

    parse(f::to_json(msg));

    return 0;
}

void parse(const std::string& str)
{
    auto msg = f::from_json< message >(str);
    std::cout << msg.id << '\n';
    std::cout << msg.from << '\n';
    std::cout << msg.text << '\n';
    for (auto& it: msg.data) {
        std::cout << ' ' << it.key << '=' << it.value << '\n';
    }
}
