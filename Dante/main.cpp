#include "context.h"
#include "application.h"
#include "dx/exception.h"
#include "sdl/exception.h"

int main(int argc, char** argv)
{
    auto log = spdlog::stdout_color_mt("Main");

    try
    {
        dante::Context context{ argc, argv };

        context.log->warn("Here's a warning");
        context.log->error("IO error");
        context.log->critical("FATAL ERROR");

        dante::Application app(context);
        app.run();
    }
    catch (dante::dx::COMException com)
    {
        auto msg = std::string{ "COM Exception: " } + com.what();
        log->critical(msg);
    }
    catch (dante::sdl::SDLException sdl)
    {
        auto msg = std::string{ "SDL Exception: " } + sdl.what();
        log->critical(msg);
    }
    catch (std::exception e)
    {
        auto msg = std::string{ "General Exception: " } + e.what();
        log->critical(msg);
    }

    return 0;
}