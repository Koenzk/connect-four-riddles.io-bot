// main.cpp
// Aswin van Woudenberg

#include "c4bot.h"
#include <cstdlib>
#include <ctime>

int main() {
    srand(static_cast<unsigned int>(std::time(0)));
    C4Bot bot;
    bot.run();

    return 0;
}