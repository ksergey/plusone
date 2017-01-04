/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_binder_081116001039_MADLIFE
#define MADLIFE_binder_081116001039_MADLIFE

#include <string>
#include <stdexcept>
#include "meta.hpp"

namespace plusone {
namespace serialization {

template< class FieldT, class PolicyT, class CheckerT >
class binder final
{
private:
    /* field name */
    std::string field_name_;
    /* field reference */
    FieldT& field_;
    /* presence policy */
    PolicyT policy_;
    /* field value checker */
    CheckerT checker_;

public:
    /** construct the binder */
    binder(const std::string& field_name, FieldT& field,
            PolicyT&& policy, CheckerT&& checker)
        : field_name_(field_name)
        , field_(field)
        , policy_(std::move(policy))
        , checker_(std::move(checker))
    {}

    /** read value from json object */
    void read_from(const json& object) const
    {
        auto found = object.find(field_name_);
        if (__likely(found != object.end())) {
            const json& value = found.value();
            if (!value.is_null()) {
                read_value(value, field_);
            } else {
                throw std::runtime_error("value is null");
            }
        } else {
            policy_.field_not_defined(field_);
        }

        checker_(field_);
    }

    /** write value into json object */
    void write_to(json& object) const
    {
        checker_(field_);

        json node;
        write_value(field_, node);
        object[field_name_] = node;
    }
};

} /* namespace serialization */
} /* namespace plusone */

#endif /* MADLIFE_binder_081116001039_MADLIFE */
