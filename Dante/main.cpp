#include "context.h"

int main(int argc, char** argv)
{
    try
    {
        dante::Context context{ argc, argv };

        context.log->warn("Here's a warning");
        context.log->error("IO error");
        context.log->critical("FATAL ERROR");

        SDL_Delay(5000);
    }
    catch (...)
    {
    }

    return 0;
}