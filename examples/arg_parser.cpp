/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <memory>
#include <experimental/optional>

namespace plusone {

using std::experimental::optional;
using std::experimental::make_optional;

class argument_value
{
public:
    virtual ~argument_value() = default;

    virtual bool parse(const char* value) = 0;
};

template< class T >
class value final
    : public argument_value
{
private:
    T value_{};

public:
    value() = default;

    /* add default value here */

    bool parse(const char* value) override
    {
        std::istringstream is(value);
        is >> value_;
        return is.good();
    }
};

class argument_definition
{
private:
    std::string description_;
    std::string short_name_;
    std::string long_name_;
    std::string value_name_;
    std::unique_ptr< argument_value > value_;
    bool required_{false};

public:
    argument_definition() = default;

    const std::string& description() const noexcept
    { return description_; }

    argument_definition& description(const std::string& value)
    { description_ = value; return *this; }

    const std::string& long_name() const noexcept
    { return long_name_; }

    argument_definition& long_name(const std::string& value)
    { long_name_ = value; return *this; }

    const std::string& short_name() const noexcept
    { return short_name_; }

    argument_definition& short_name(const std::string& value)
    { short_name_ = value; return *this; }

    const std::string& value_name() const noexcept
    { return value_name_; }

    argument_definition& name(const std::string& short_value, const std::string& long_value = std::string{})
    {
        short_name_ = short_value;
        long_name_ = long_value;
        return *this;
    }

    bool required() const noexcept
    { return required_; }

    argument_definition& required(bool value) noexcept
    { required_ = value; return *this; }

    template< class T >
    plusone::value< T >& value(const std::string& name = std::string{})
    {
        using value_type = plusone::value< T >;
        value_ = std::make_unique< value_type >();
        value_name_ = name;
        return *reinterpret_cast< value_type* >(value_.get());
    }

    void reset_value()
    {
        value_.reset();
        value_name_.clear();
    }
};

class argument_parser
{
private:
    std::string name_;
    std::string description_;
    std::vector< argument_definition > defs_;

public:
    argument_parser(const std::string& name, const std::string& description = std::string{})
        : name_{name}
        , description_{description}
    {}

    argument_definition& add()
    {
        defs_.emplace_back();
        return defs_.back();
    }

    void run(const char* argv[], int argc)
    {
        /* TODO: implement me */
    }

    void print_help(std::ostream& os = std::cout) const
    {
        os << "Usage: " << name_ << " [OPTIONS]...\n";
        if (!description_.empty()) {
            os << description_ << '\n';
        }
        os << '\n';

        for (const auto& e: defs_) {
            bool has_short = false;
            if (!e.short_name().empty()) {
                os << "  -" << e.short_name();
                has_short = true;
            } else {
                os << "    ";
            }

            if (!e.long_name().empty()) {
                if (has_short) {
                    os << ", ";
                } else {
                    os << "  ";
                }
                os << "--" << e.long_name();
            }
            if (!e.value_name().empty()) {
                os << "=" << e.value_name();
            }

            os << "\t\t" << e.description() << "\n";
        }
    }
};

} /* namespace plusone */

int main(int argc, char* argv[])
{
    plusone::argument_parser parser("arg-parser", "Command line parsing framework");
    parser.add()
        .name("h", "help")
        .description("Show help and exit");
    parser.add()
        .name("c", "count")
        .description("Show count number")
        .value< unsigned >("NUM");
    parser.add()
        .long_name("value")
        .description("Set new value")
        .value< unsigned >("VALUE");

    parser.print_help(std::cout);
    return 0;
}
