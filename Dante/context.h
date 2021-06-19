#pragma once

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace dante
{
    class Context
    {
        std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> _window{ nullptr, SDL_DestroyWindow };

    public:
        Context();
        ~Context();

        std::shared_ptr<spdlog::logger> log{ spdlog::stdout_color_mt("Dante") };
    };
}