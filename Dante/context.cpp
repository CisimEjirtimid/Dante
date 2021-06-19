#include "context.h"
#include "fmt/format.h"

namespace dante
{
    namespace constants
    {
        static constexpr int SCREEN_WIDTH = 800;
        static constexpr int SCREEN_HEIGHT = 600;
    }

    Context::Context()
    {
        spdlog::set_pattern("[%T.%e][%n][Tid: %t][%^%l%$]: %v");

#ifdef _DEBUG
        spdlog::set_level(spdlog::level::debug);
#else
        spdlog::set_level(spdlog::level::warn);
#endif

        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            auto message = fmt::format("Cannot initialize SDL! Error: {}", SDL_GetError());
            log->critical(message);
            throw std::runtime_error(message);
        }

        _window.reset(SDL_CreateWindow(
            "Dante v0.0.1",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            constants::SCREEN_WIDTH, constants::SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN));

        log->info("Context (Log, Window) created");
    }

    Context::~Context()
    {
        log->info("Context destroyed");
    }
}