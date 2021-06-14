#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <iostream>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

auto logger = spdlog::stdout_color_mt("Dante");

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int main(int arc, char** argv)
{
    spdlog::set_pattern("[%T.%e][%n][Tid: %t][%^%l%$]: %v");
    spdlog::set_level(spdlog::level::debug);

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
    }
    else
    {
        logger->info("Info example");
        logger->warn("Here's a warning");
        logger->error("IO error");

        logger->critical("FATAL ERROR");


        auto window = SDL_CreateWindow(
            "Dante v0.0.1",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            SCREEN_WIDTH, SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN
        );
        logger->info("Window created");

        SDL_Delay(2000);

        SDL_DestroyWindow(window);

        logger->info("Window destroyed");
    }

    return 0;
}