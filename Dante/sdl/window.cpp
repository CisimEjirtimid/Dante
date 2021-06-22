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

    bool WindowPtr::pollEvents()
    {
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            switch (ev.type) {
            case SDL_WINDOWEVENT: {
                switch (ev.window.event) {
                case SDL_WINDOWEVENT_CLOSE: {
                    _shouldClose = true;
                    break;
                }
                }
                break;
            }
            }
        }
        return false;
    }

    bool WindowPtr::shouldClose() const {
        return _shouldClose;
    }
}