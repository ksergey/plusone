/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_all_081116000729_MADLIFE
#define MADLIFE_all_081116000729_MADLIFE

#include <iostream>

namespace plusone {
namespace serialization {

struct null_checker;

/** Required field type */
template< class FieldT, class CheckerT = null_checker >
auto required(const std::string& field_name, FieldT& field, CheckerT&& checker = CheckerT{});

/** Optional field type */
template< class FieldT, class FieldDefaultT, class CheckerT = null_checker >
auto optional(const std::string& field_name, FieldT& field,
        FieldDefaultT default_value, CheckerT&& checker = CheckerT{});

/** Convert msg to json formated string */
template< class MsgT >
std::string to_json(const MsgT& msg, int indent = -1);

/** Parse msg from json formated string */
template< class MsgT >
void from_json(const std::string& str, MsgT& msg);

/** Parse msg from json formated string */
template< class MsgT >
MsgT from_json(const std::string& str);

/** Serialize message to stream */
template< class MsgT >
void to_stream(std::ostream& os, const MsgT& msg, int indent = -1);

/** Parse message from stream */
template< class MsgT >
void from_stream(std::istream& is, MsgT& msg);

/** Parse message from stream */
template< class MsgT >
MsgT from_stream(std::istream& is);

} /* namespace serialization */
} /* namespace plusone */

#include "impl/all.ipp"

#endif /* MADLIFE_all_081116000729_MADLIFE */
