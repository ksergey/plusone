/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_all_130717113814
#define KSERGEY_all_130717113814

#include "../value_io.hpp"
#include "../binder.hpp"
#include "../checkers.hpp"

namespace plusone {
namespace serialization {

namespace policy {

template< class FieldT, class FieldDefaultT >
inline void set_default_value(FieldT& field, FieldDefaultT&& def)
{
    field = std::move(def);
}

/** Required field policy */
struct required
{
    template< class FieldT >
    void field_not_defined(FieldT&) const
    {
        throw_ex< policy_error >("Mandatory field not exists");
    }

    template< class FieldT >
    constexpr bool is_default_value(FieldT&) const
    {
        return false;
    }
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
    void field_not_defined(FieldT& field) const
    {
        set_default_value(field, std::move(default_value_));
    }
};

} /* namespace policy */

/** Default checker */
struct null_checker
{
    template< class FieldT >
    constexpr void operator()(const FieldT&) const
    {}
};

template< class FieldT, class CheckerT >
inline auto required(const std::string& field_name, FieldT& field, CheckerT&& checker)
{
    using binder_type = binder< FieldT, policy::required, CheckerT >;
    return binder_type{field_name, field, policy::required{}, std::move(checker)};
}

template< class FieldT, class FieldDefaultT, class CheckerT >
inline auto optional(const std::string& field_name, FieldT& field,
        FieldDefaultT default_value, CheckerT&& checker)
{
    using policy_type = policy::optional< FieldDefaultT >;
    using binder_type = binder< FieldT, policy_type, CheckerT >;
    return binder_type{field_name, field, policy_type{std::move(default_value)}, std::move(checker)};
}

template< class MsgT >
inline std::string to_json(const MsgT& msg, int indent)
{
    json object;
    output out{object};
    serialize(out, const_cast< MsgT& >(msg));
    return object.dump(indent);
}

template< class MsgT >
inline void from_json(const std::string& str, MsgT& msg)
{
    auto object = json::parse(str);
    input in{object};
    serialize(in, msg);
}

template< class MsgT >
inline MsgT from_json(const std::string& str)
{
    MsgT result;
    from_json(str, result);
    return result;
}

template< class MsgT >
inline void to_stream(std::ostream& os, const MsgT& msg, int indent)
{
    os << to_json(msg, indent);
}

template< class MsgT >
inline void from_stream(std::istream& is, MsgT& msg)
{
    json object;
    is >> object;
    input in{object};
    serialize(in, msg);
}

template< class MsgT >
inline MsgT from_stream(std::istream& is)
{
    MsgT result;
    from_stream(is, result);
    return result;
}

} /* namespace serialization */
} /* namespace plusone */

#endif /* KSERGEY_all_130717113814 */
