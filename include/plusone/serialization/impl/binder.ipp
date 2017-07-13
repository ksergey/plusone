/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_binder_130717105424
#define KSERGEY_binder_130717105424

namespace plusone {
namespace serialization {

template< class FieldT, class PolicyT, class CheckerT >
inline binder< FieldT, PolicyT, CheckerT >::binder(const std::string& field_name, FieldT& field,
        PolicyT&& policy, CheckerT&& checker)
    : field_name_(field_name)
    , field_(field)
    , policy_(std::move(policy))
    , checker_(std::move(checker))
{}

template< class FieldT, class PolicyT, class CheckerT >
inline void binder< FieldT, PolicyT, CheckerT >::read_from(const json& object) const
{
    auto found = object.find(field_name_);
    if (found != object.end()) {
        const json& value = found.value();
        if (!value.is_null()) {
            read_value(value, field_);
        } else {
            throw io_error{"value is null"};
        }
    } else {
        policy_.field_not_defined(field_);
    }

    checker_(field_);
}

template< class FieldT, class PolicyT, class CheckerT >
inline void binder< FieldT, PolicyT, CheckerT >::write_to(json& object) const
{
    checker_(field_);

    json node;
    write_value(field_, node);
    object[field_name_] = node;
}

} /* namespace serialization */
} /* namespace plusone */

#endif /* KSERGEY_binder_130717105424 */
