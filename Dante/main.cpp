#include "context.h"
#include "application.h"

int main(int argc, char** argv)
{
    try
    {
        dante::Context context{ argc, argv };

        context.log->warn("Here's a warning");
        context.log->error("IO error");
        context.log->critical("FATAL ERROR");

        dante::Application app(context);
        app.run();
    }
    catch (...)
    {
    }

    return 0;
}