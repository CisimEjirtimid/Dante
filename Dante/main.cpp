#include "context.h"

int main(int arc, char** argv)
{
    try
    {
        dante::Context context;

        context.log->warn("Here's a warning");
        context.log->error("IO error");
        context.log->critical("FATAL ERROR");

        SDL_Delay(1000);
    }
    catch (...)
    {
    }

    return 0;
}