#include "application.h"
#include "timer.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace dante
{
    namespace
    {
        auto logger = spdlog::stdout_color_mt("Application");
    }

    Application::Application(const Context& context)
    {
        // do something with context (e.g. get command line args with a nice interface, query device capabilities, check for some extensions etc)

        _window = sdl::WindowPtr{ "Dante v0.0.1" , 512, 512 };
    }

    Application::~Application()
    {

    }


    void Application::run() {
        logger->info("Dante is up and running!");

        Timer<std::milli> updateTimer;
        double timeSinceLastUpdate = 0.0;
        while (!_window.shouldClose())
        {
            const double timeDelta = updateTimer.restart() / 1000.0;
            timeSinceLastUpdate += timeDelta;

            handleEvents();

            while (timeSinceLastUpdate > UpdateDelta)
            {
                update(UpdateDelta);
                timeSinceLastUpdate -= UpdateDelta;
            }

            // TODO: Use whatever is left in timeSinceLastUpdate to smoothly extrapolate into the next frame
            // E.g. cameras, transform matrices etc
            render();
        }

        //_renderer->finish();
    }


    void Application::handleEvents() {
        _window.pollEvents();
        //_gui->processDelayedEvents();
    }

    void Application::update(double dt) {
        //_gui->update(dt);
        //_scene->update(dt);
    }

    void Application::render() {
        //_renderer->renderFrame();
    }
}