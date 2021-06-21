#pragma once
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <memory>
#include <string>

namespace dante::sdl
{
    class WindowPtr : public std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>
    {
    public:
        WindowPtr();
        WindowPtr(const std::string& title, size_t width, size_t height);

        HWND hwnd();
    };
}