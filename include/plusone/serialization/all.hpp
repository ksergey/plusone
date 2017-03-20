/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_all_081116000729_MADLIFE
#define MADLIFE_all_081116000729_MADLIFE

#include <iostream>
#include "value_io.hpp"
#include "binder.hpp"
#include "checkers.hpp"

namespace plusone {
namespace serialization {

namespace policy {

template< class FieldT, class FieldDefaultT >
void set_default_value(FieldT& field, FieldDefaultT&& def)
{ field = std::move(def); }

/** Required field policy */
struct required
{
    template< class FieldT >
    __force_inline void field_not_defined(FieldT&) const
    { throw policy_error{"mandatory field not exists"}; }

    template< class FieldT >
    __force_inline constexpr bool is_default_value(FieldT&) const
    { return false; }
};

/**
 * Optional field policy.
 * if value not exists in json object use default value
 */
template< class FieldDefaultT >
class optional
{
private:
    FieldDefaultT default_value_;

public:
    explicit optional(FieldDefaultT default_value)
        : default_value_(default_value)
    {}

    template< class FieldT >
    __force_inline void field_not_defined(FieldT& field) const
    { set_default_value(field, std::move(default_value_)); }
};

} /* namespace policy */

/** Default checker */
struct null_checker final
{
    template< class FieldT >
    constexpr void operator()(const FieldT&) const
    {}
};

/** Required field type */
template< class FieldT, class CheckerT = null_checker >
auto required(const std::string& field_name, FieldT& field,
        CheckerT&& checker = CheckerT{})
{
    using binder_type = binder< FieldT, policy::required, CheckerT >;
    return binder_type{field_name, field, policy::required{}, std::move(checker)};
}

/** Optional field type */
template< class FieldT, class FieldDefaultT, class CheckerT = null_checker >
auto optional(const std::string& field_name, FieldT& field,
        FieldDefaultT default_value, CheckerT&& checker = CheckerT{})
{
    using policy_type = policy::optional< FieldDefaultT >;
    using binder_type = binder< FieldT, policy_type, CheckerT >;
    return binder_type{field_name, field, policy_type{std::move(default_value)}, std::move(checker)};
}

/** Convert msg to json formated string */
template< class MsgT >
__force_inline std::string to_json(const MsgT& msg, int indent = -1)
{
    json object;
    output out{object};
    serialize(out, const_cast< MsgT& >(msg));
    return object.dump(indent);
}

/** Parse msg from json formated string */
template< class MsgT >
__force_inline void from_json(const std::string& str, MsgT& msg)
{
    auto object = json::parse(str);
    input in{object};
    serialize(in, msg);
}

/** Parse msg from json formated string */
template< class MsgT >
__force_inline MsgT from_json(const std::string& str)
{
    MsgT result;
    from_json(str, result);
    return result;
}

/** Serialize message to stream */
template< class MsgT >
__force_inline void to_stream(std::ostream& os, const MsgT& msg, int indent = -1)
{ os << to_json(msg, indent); }

/** Parse message from stream */
template< class MsgT >
__force_inline void from_stream(std::istream& is, MsgT& msg)
{
    json object;
    is >> object;
    input in{object};
    serialize(in, msg);
}

/** Parse message from stream */
template< class MsgT >
__force_inline MsgT from_stream(std::istream& is)
{
    MsgT result;
    from_stream(is, result);
    return result;
}

} /* namespace serialization */
} /* namespace plusone */

#endif /* MADLIFE_all_081116000729_MADLIFE */
