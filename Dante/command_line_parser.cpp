#include "command_line_parser.h"

namespace dante::command_line
{
    ParsedValues::ParsedValues(cxxopts::ParseResult&& result)
        : _result(std::move(result))
    {
    }

    Parser::Parser(const std::string& name, const std::string& description)\
        : Options("Dante", "Direct3D 12 Capabilities Showroom")
    {
    }
}