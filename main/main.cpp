#include "tracking/tracking.hpp"
#include "loader.hpp"
#pragma comment(linker, "/STACK:4000000")

int main()
{
    initTracking();
    setupTracking();
    return 0;
}