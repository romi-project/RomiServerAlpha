#include "std.hpp"
#include "defines.hpp"

int main()
{
    static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;

    plog::init(plog::verbose, &consoleAppender);

    LOGD << "Hello world";

    return 0;
}
