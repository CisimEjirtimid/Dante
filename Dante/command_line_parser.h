#pragma once
#include "cxxopts/cxxopts.hpp"

namespace dante::command_line
{
    template <typename T>
    struct Opt
    {
        std::string flags;
        std::string description;
        std::optional<T> default_value;

        Opt(const std::string& flags_, const std::string& description_, std::optional<T> default_value_ = std::nullopt)
            : flags(flags_)
            , description(description_)
            , default_value(default_value_)
        {}
    };

    class ParsedValues
    {
        cxxopts::ParseResult _result;

    public:
        ParsedValues(cxxopts::ParseResult&& result);

        template <typename T>
        const T& get(const std::string& key) const
        {
            return _result[key].as<T>();
        }
    };

    class Parser : public cxxopts::Options
    {
        template <typename T>
        void option(const Opt<T>& opt)
        {
            if (opt.default_value)
                add_options()
                (opt.flags, opt.description, cxxopts::value<T>()->default_value(std::to_string(*opt.default_value)));
            else
                add_options()
                (opt.flags, opt.description, cxxopts::value<T>());
        }
    public:
        Parser(const std::string& name, const std::string& description);

        template <typename... Ts>
        void options(const Opt<Ts>&... opts)
        {
            (option<Ts>(opts),...);
        }

        ParsedValues parse(int argc, char** argv)
        {
            return ParsedValues{ cxxopts::Options::parse(argc, argv) };
        }
    };
}