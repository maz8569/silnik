#include "Application.h"

extern GameEngine::Application GameEngine::app;

using namespace GameEngine;

int main()
{
    GameEngine::app.RunLoop();

    return 0;
}