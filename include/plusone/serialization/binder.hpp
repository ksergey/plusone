/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_binder_081116001039_MADLIFE
#define MADLIFE_binder_081116001039_MADLIFE

#include <string>
#include <plusone/serialization/common.hpp>

namespace plusone {
namespace serialization {

template< class FieldT, class PolicyT, class CheckerT >
class binder
{
private:
    /* Field name */
    std::string field_name_;
    /* Field reference */
    FieldT& field_;
    /* Presence policy */
    PolicyT policy_;
    /* Field value checker */
    CheckerT checker_;

public:
    /** Construct the binder */
    binder(const std::string& field_name, FieldT& field,
            PolicyT&& policy, CheckerT&& checker);

    /** Read value from json object */
    void read_from(const json& object) const;

    /** Write value into json object */
    void write_to(json& object) const;
};

} /* namespace serialization */
} /* namespace plusone */

#include <plusone/serialization/impl/binder.ipp>

#endif /* MADLIFE_binder_081116001039_MADLIFE */
