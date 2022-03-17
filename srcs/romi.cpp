#include "std.hpp"

int main()
{
    static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;

    plog::init(plog::debug, &consoleAppender);

    LOGD << "Hello world";

    return 0;
}
