#pragma once

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "command_line_parser.h"

namespace dante
{
    class Context
    {
        std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> _window{ nullptr, SDL_DestroyWindow };

        command_line::Parser _parser{ "Dante", "Direct3D 12 Capabilities Showroom" };

    public:
        Context(int argc, char** argv);
        ~Context();

        std::shared_ptr<spdlog::logger> log{ spdlog::stdout_color_mt("Dante") };
    };
}