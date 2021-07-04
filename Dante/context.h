#pragma once
#include "sdl/window.h"
#include "command_line_parser.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace dante
{
    class Context
    {
        command_line::Parser _parser{ "Dante", "Direct3D 12 Capabilities Showroom" };

    public:
        Context(int argc, char** argv);
        ~Context();

        std::shared_ptr<spdlog::logger> log{ spdlog::stdout_color_mt("Context") };

        // TODO: property-like interface?
        size_t width = 0;
        size_t height = 0;
    };
}