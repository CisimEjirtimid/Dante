#include "window.h"

namespace dante::sdl
{
    WindowPtr::WindowPtr()
        : std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>(nullptr, SDL_DestroyWindow)
    {
    }

    WindowPtr::WindowPtr(const std::string& title, size_t width, size_t height)
        : WindowPtr()
    {
        reset(SDL_CreateWindow(
            title.c_str(),
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            int(width), int(height),
            SDL_WINDOW_SHOWN));
    }

    HWND WindowPtr::hwnd()
    {
        SDL_SysWMinfo info;
        SDL_GetWindowWMInfo(get(), &info);
        return info.info.win.window;
    }
}