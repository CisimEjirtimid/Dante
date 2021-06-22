#pragma once

#include <memory>

#include "sdl/window.h"

namespace dante
{
    class Context;
    class Renderer
    {
        // TODO and not here..
    };

    class Application
    {
    public:
        Application(const Context& context);
        ~Application();


        void run();


    private:
        static constexpr uint32_t UpdateFps = 144;
        static constexpr double UpdateDelta = 1.0 / UpdateFps;

        void handleEvents();
        void update(double dt);
        void render();

        sdl::WindowPtr _window;
        std::unique_ptr<Renderer> _renderer;
    };
}