#include "context.h"
#include "fmt/format.h"

namespace dante
{
    namespace constants
    {
        static constexpr size_t SCREEN_WIDTH = 800;
        static constexpr size_t SCREEN_HEIGHT = 600;
    }

    Context::Context(int argc, char** argv)
    {
        _parser.options(
            command_line::Opt<size_t>{ "w, width", "Window width", constants::SCREEN_WIDTH },
            command_line::Opt<size_t>{ "h, height", "Window height", constants::SCREEN_HEIGHT });

        auto result = _parser.parse(argc, argv);

        log->info(_parser.help());

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

        auto width = result.get<size_t>("width");
        auto height = result.get<size_t>("height");

        _window.reset(SDL_CreateWindow(
            "Dante v0.0.1",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            width, height,
            SDL_WINDOW_SHOWN));

        log->info("Context (Log, Window) created");
    }

    Context::~Context()
    {
        log->info("Context destroyed");
    }
}