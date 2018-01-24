/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_value_io_071116181212_MADLIFE
#define MADLIFE_value_io_071116181212_MADLIFE

#include <string>
#include <vector>
#include <array>
#include <plusone/serialization/meta.hpp>
#include <plusone/static_string.hpp>
#include <plusone/static_vector.hpp>

namespace plusone {
namespace serialization {

/** Input object */
class input final
{
private:
    const json& object_;

public:
    input(const json& object)
        : object_(object)
    {}

    template< class BinderT >
    input& operator&(const BinderT& binder)
    {
        binder.read_from(object_);
        return *this;
    }
};

/** Output object */
class output final
{
private:
    json& object_;

public:
    output(json& object)
        : object_(object)
    {}

    template< class BinderT >
    output& operator&(const BinderT& binder)
    {
        binder.write_to(object_);
        return *this;
    }
};

/* Arithmetic types */

template< class T >
__force_inline void read_value(const json& object, T& value, if_arithmetic_type< T >* = 0)
{
    value = object;
}

template< class T >
__force_inline void write_value(T value, json& object, if_arithmetic_type< T >* = 0)
{
    object = value;
}

/* std::string */

__force_inline void read_value(const json& object, std::string& value)
{
    value = object;
}

__force_inline void write_value(const std::string& value, json& object)
{
    object = value;
}

/* static_string */

template< std::size_t N >
__force_inline void read_value(const json& object, plusone::static_string< N >& value)
{
    value = object;
}

template< std::size_t N >
__force_inline void write_value(const plusone::static_string< N >& value, json& object)
{
    object = value.to_string();
}

/* tree read/write */

template< class IO, class DTO >
__force_inline void serialize(IO& io, DTO& dto)
{
    dto.serialize(io);
}

template< class T >
__force_inline void read_value(const json& object, T& value, if_serializable_type< T, input >* = 0)
{
    input in{object};
    serialize(in, value);
}

template< class T >
__force_inline void write_value(const T& value, json& object, if_serializable_type< T, output >* = 0)
{
    output out{object};
    serialize(out, const_cast< T& >(value));
}

/* std::vector */

template< class T, class AllocT >
__force_inline void read_value(const json& object, std::vector< T, AllocT >& vec)
{
    if (object.is_array()) {
        vec.clear();
        vec.reserve(object.size());
        for (auto& node: object) {
            vec.emplace_back();
            read_value(node, vec.back());
        }
    } else {
        throw_ex< io_error >("Value is not an array");
    }
}

template< class T, class AllocT >
__force_inline void write_value(const std::vector< T, AllocT >& vec, json& object)
{
    object = json::array();

    for (auto& item: vec) {
        json node;
        write_value(item, node);
        object.push_back(node);
    }
}

/* static_vector */

template< class T >
__force_inline void read_value(const json& object, plusone::static_vector< T >& vec)
{
    if (object.is_array()) {
        vec = plusone::static_vector< T >{object.size()};
        for (auto& node: object) {
            read_value(node, vec.emplace_back());
        }
    } else {
        throw_ex< io_error >("Value is not an array");
    }
}

template< class T >
__force_inline void write_value(const plusone::static_vector< T >& vec, json& object)
{
    object = json::array();

    for (auto& item: vec) {
        json node;
        write_value(item, node);
        object.push_back(node);
    }
}

/* std::array */

template< class T, std::size_t N >
__force_inline void read_value(const json& object, std::array< T, N >& v)
{
    if (object.is_array()) {
        if (__unlikely(N != object.size())) {
            throw_ex< io_error >("Expect array of size {}", N);
        }
        for (std::size_t i = 0; i < N; ++i) {
            read_value(object[i], v[i]);
        }
    } else {
        throw_ex< io_error >("Value is not an array");
    }
}

template< class T, std::size_t N >
__force_inline void write_value(const std::array< T, N >& v, json& object)
{
    object = json::array();

    for (std::size_t i = 0; i < N; ++i) {
        json node;
        write_value(v[i], node);
        object.push_back(node);
    }
}

} /* namespace serialization */
} /* namespace plusone */

#endif /* MADLIFE_value_io_071116181212_MADLIFE */
